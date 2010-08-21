#include "object.hpp"
#include "vf/format.hpp"
#include "vf/vertex_view.hpp"
#include "vf/position.hpp"
#include "vf/packed_position.hpp"
#include "vf/texcoord.hpp"
#include "vf/packed_texcoord.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/dim3.hpp"
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/index_buffer.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/scoped_index_lock.hpp>
#include <sge/renderer/scoped_texture.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/indexed_primitive_type.hpp>
#include <sge/renderer/primitive_count.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/first_index.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/model/index.hpp>
#include <sge/model/object.hpp>
#include <sge/model/index_sequence.hpp>
#include <sge/model/vertex_sequence.hpp>
#include <sge/model/texcoord.hpp>
#include <sge/model/texcoord_sequence.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/box/extend_bounding_box.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <type_traits>
#include <algorithm>

// DEBUG
#include <fcppt/math/vector/output.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>

namespace
{
struct index_visitor
{
public:
	typedef 
	void 
	result_type;

	explicit
	index_visitor(
		sge::model::index_sequence const &indices)
	:
		indices(
			indices)
	{
	}

	template<typename T>
	result_type
	operator()(
		T const &t) const
	{
		typedef typename 
		T::value_type 
		value_type;

		typename T::iterator it = t.begin();

		std::transform(
			indices.begin(),
			indices.end(),
			it,
			[](sge::model::index const &i) { return static_cast<value_type>(i); });
	}
private:
	sge::model::index_sequence const &indices;
};
}

insula::model::object::object(
	graphics::camera::object const &_camera,
	sge::model::object_ptr const model,
	sge::renderer::device_ptr const _renderer,
	graphics::shader &_shader,
	sge::renderer::texture_ptr const _texture,
	fcppt::optional<fcppt::string> const &_part)
:
	raw_(
		model),
	camera_(
		_camera),
	renderer_(
		_renderer),
	shader_(
		_shader),
	texture_(
		_texture),
	bounding_box_()
{
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		FCPPT_TEXT("texture"),
		0);

	FCPPT_ASSERT(
		!model->part_names().empty());

	fcppt::string const part = 
		_part 
			? 
				*_part 
			: 
				model->part_names().front();

	sge::model::vertex_sequence const vertices = 
		model->vertices(
			part);

	FCPPT_ASSERT(
		model->texcoords(
			part));

	sge::model::texcoord_sequence const texcoords = 
		*(model->texcoords(part));

	static_assert(
		std::is_same
		<
			sge::model::vertex_sequence::size_type,
			sge::model::texcoord_sequence::size_type
		>::value,
		"vertex_sequence und texcoord_sequence have to have the same size_type");

	FCPPT_ASSERT(
		vertices.size() == texcoords.size());

	vb_ = 
		renderer_->create_vertex_buffer(
	 		sge::renderer::vf::dynamic::make_format<vf::format>(),
	 		static_cast<sge::renderer::size_type>(
	 			vertices.size()),
	 		sge::renderer::resource_flags::none);

	 sge::renderer::scoped_vertex_lock const vblock(
	 	vb_,
	 	sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices_view(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices_view.begin());

	FCPPT_ASSERT(
		vertices.size());

	bounding_box_.pos(
		vertices[0]);
	bounding_box_.dimension(
		graphics::dim3::null());

	for (
		sge::model::vertex_sequence::size_type i = 0; 
		i < vertices.size(); 
		++i)
	{
		sge::model::position real_pos = 
			vertices[i];

		bounding_box_ = 
			fcppt::math::box::extend_bounding_box(
				bounding_box_,
				real_pos);

		vb_it->set<vf::position>(
			vf::packed_position(
				real_pos));

		vb_it->set<vf::texcoord>(
			fcppt::math::vector::structure_cast<vf::packed_texcoord>(
				texcoords[i]));
		vb_it++;
	}

	sge::model::index_sequence const indices = 
		model->indices(
			part);

	ib_ = 
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			static_cast<sge::renderer::size_type>(
				indices.size()),
			sge::renderer::resource_flags::none);

	fcppt::variant::apply_unary(
		index_visitor(
			indices),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());
}

void
insula::model::object::render(
	graphics::mat4 const &transform)
{
	sge::renderer::state::scoped const sstate(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::cull_mode::back)
			(sge::renderer::state::depth_func::less));

	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		camera_.perspective() * 
		camera_.rotation() * 
		camera_.translation() * 
		transform);

	sge::renderer::scoped_texture scoped_tex(
		renderer_,
		texture_,
		0);

	renderer_->render(
		ib_,
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::indexed_primitive_type::triangle,
		sge::renderer::primitive_count(
			ib_->size() / 3),
		sge::renderer::first_index(
			0));
}

insula::graphics::box const
insula::model::object::bounding_box() const
{
	return bounding_box_;
}

insula::model::object::~object() {}

#include "object.hpp"
#include "vf/packed_normal.hpp"
#include "vf/packed_position.hpp"
#include "vf/format.hpp"
#include "vf/height_and_gradient.hpp"
#include "vf/packed_height_and_gradient.hpp"
#include "vf/normal.hpp"
#include "vf/vertex_view.hpp"
#include "scalar.hpp"
#include "array.hpp"
#include "vec2.hpp"
#include "normalize_and_stretch.hpp"
#include "generate_gradient.hpp"
#include "calculate_index_cell.hpp"
#include "calculate_normal.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera.hpp"
#include "../media_path.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/index_buffer.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/index/dynamic/format.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/indexed_primitive_type.hpp>
#include <sge/renderer/primitive_count.hpp>
#include <sge/renderer/first_index.hpp>
#include <sge/renderer/index/i32.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_index_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/image/file.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <boost/foreach.hpp>
#include <type_traits>
#include <cmath>
#include <algorithm>

namespace
{
struct index_visitor
{
public:
	typedef 
	void 
	result_type;

	typedef 
	insula::height_map::array::size_type
	size_type;

	index_visitor(
		size_type const _w,
		size_type const _h)
	:
		w(
			_w),
		h(
			_h)
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

		for (size_type y = 0; y < static_cast<size_type>(h-1); ++y)
			for (size_type x = 0; x < static_cast<size_type>(w-1); ++x)
				BOOST_FOREACH(
					value_type const i,
					insula::height_map::calculate_index_cell<value_type>(
						static_cast<value_type>(
							x),
						static_cast<value_type>(
							y),
						static_cast<value_type>(
							w),
						static_cast<value_type>(
							h)))
					*it++ = i;
	}
private:
	size_type w,h;
};
}

insula::height_map::object::object(
	graphics::camera const &_camera,
	sge::renderer::device_ptr const _renderer,
	sge::console::object &_console,
	height_map::array const &raw,
	graphics::vec2 const &cell_sizes,
	graphics::scalar const height_scaling,
	graphics::vec3 const &sun_direction,
	graphics::scalar const ambient_light,
	graphics::scalar const texture_scaling,
	sge::image::file_ptr const &gradient_texture_image,
	sge::image::file_ptr const &lower_texture_image,
	sge::image::file_ptr const &upper_texture_image)
:
	camera_(
		_camera),
	renderer_(
		_renderer),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("vertex.glsl"),
		media_path()/FCPPT_TEXT("fragment.glsl")),
	shader_console_(
		FCPPT_TEXT("terrain"),
		shader_,
		_console)
{
	regenerate(
		cell_sizes,
		height_scaling,
		raw);

	shader_.activate();
	
	shader_.set_uniform(
		FCPPT_TEXT("sand"),
		0);

	shader_.set_uniform(
		FCPPT_TEXT("rock"),
		1);

	shader_.set_uniform(
		FCPPT_TEXT("grass"),
		2);
	
	shader_.set_uniform(
		FCPPT_TEXT("sun_direction"),
		sun_direction);

	shader_.set_uniform(
		FCPPT_TEXT("ambient_light"),
		ambient_light);

	shader_.set_uniform(
		FCPPT_TEXT("texture_scaling"),
		texture_scaling);
	
	shader_.set_uniform(
		FCPPT_TEXT("grid_size"),
		graphics::vec2(
			static_cast<graphics::scalar>(
				raw.shape()[0]),
			static_cast<graphics::scalar>(
				raw.shape()[1])) * 
		cell_sizes);
	
	lower_texture_ = 
		renderer_->create_texture(
			lower_texture_image->view(),
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none);
	
	upper_texture_ = 
		renderer_->create_texture(
			upper_texture_image->view(),
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none);
	
	gradient_texture_ = 
		renderer_->create_texture(
			gradient_texture_image->view(),
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none);
}

void
insula::height_map::object::render()
{
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);
	
	shader_.activate();

	shader_.set_uniform(
		FCPPT_TEXT("world"),
		fcppt::math::matrix::transpose(
			camera_.world()));

	shader_.set_uniform(
		FCPPT_TEXT("perspective"),
		fcppt::math::matrix::transpose(
			camera_.perspective()));
	
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(sge::renderer::state::bool_::enable_lighting = false)
		 	(sge::renderer::state::cull_mode::front)
		 	(sge::renderer::state::depth_func::less));

	renderer_->texture(
		lower_texture_,
		0);

	renderer_->texture(
		gradient_texture_,
		1);

	renderer_->texture(
		upper_texture_,
		2);

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

	renderer_->texture(
		sge::renderer::device::no_texture,
		0);

	renderer_->texture(
		sge::renderer::device::no_texture,
		1);

	renderer_->texture(
		sge::renderer::device::no_texture,
		2);
	
	shader_.deactivate();
}



void
insula::height_map::object::regenerate(
	graphics::vec2 const &cell_sizes,
	graphics::scalar const height_scaling,
	array const &raw)
{
	array stretched(
		raw);
	
	normalize_and_stretch(
		stretched);
	
	array gradient(
		generate_gradient(
			stretched));
	
	normalize_and_stretch(
		gradient);

	std::transform(
		gradient.data(),
		gradient.data() + gradient.num_elements(),
		gradient.data(),
		[](array::element const s) { return std::sin(s); });
	
	regenerate_buffers(
		cell_sizes,
		height_scaling,
		raw,
		stretched,
		gradient);
}

void
insula::height_map::object::regenerate_buffers(
	graphics::vec2 const &cell_sizes,
	graphics::scalar const height_scaling,
	array const &raw,
	array const &stretched,
	array const &gradient)
{
	shader_.activate();

	vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				raw.num_elements()),
			sge::renderer::resource_flags::none);

	FCPPT_ASSERT_MESSAGE(
		vb_->size() < 
		static_cast<sge::renderer::size_type>(
			std::numeric_limits<sge::renderer::index::i32>::max()),
		FCPPT_TEXT("The heightmap is too big for a 16 bit index!"));

	ib_ = 
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			// q = (n-1)*(m-1) Quads
			// 2*q Tris
			// 3*2*q Indices
			static_cast<sge::renderer::size_type>(
				3*2*((raw.shape()[0]-1)*(raw.shape()[1]-1))),
			sge::renderer::resource_flags::none);
	
	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	for (array::size_type y = 0; y < raw.shape()[0]; ++y)
	{
		for (array::size_type x = 0; x < raw.shape()[1]; ++x)
		{
			vf::packed_position p(
				static_cast<graphics::scalar>(
					static_cast<scalar>(
						x) * 
					cell_sizes.x()),
				static_cast<graphics::scalar>(
					static_cast<scalar>(
						height_scaling) * 
					raw[y][x]),
				static_cast<graphics::scalar>(
					static_cast<scalar>(
						y) * 
					cell_sizes.y()));
			// Just to be safe, we put the 0 here
			(*vb_it).set<vf::position>(
				p);

			(*vb_it).set<vf::normal>(
				calculate_normal(
					raw,
					height_scaling,
					cell_sizes,
					x,
					y));

			(*vb_it).set<vf::height_and_gradient>(
				vf::packed_height_and_gradient(
					stretched[y][x],
					gradient[y][x]));
			
			vb_it++;
		}
	}
	
	fcppt::variant::apply_unary(
		index_visitor(
			raw.shape()[0],
			raw.shape()[1]),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());
}

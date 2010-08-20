#include "debug_drawer.hpp"
#include "world.hpp"
#include "bullet_to_vec3.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/camera/object.hpp"
#include "../media_path.hpp"
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/foreach.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

namespace
{
namespace vf_tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(tip_color)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		insula::graphics::scalar,
		3
	>,
	vf_tags::position
> 
vf_position;

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		insula::graphics::scalar,
		3
	>,
	vf_tags::tip_color
> 
vf_color;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		vf_position,
		vf_color
	>
> 
vertex_format;

typedef 
sge::renderer::vf::view
<
	vertex_format
> 
vertex_view;
}

insula::physics::debug_drawer::debug_drawer(
	world &_world,
	sge::renderer::device_ptr const _renderer,
	graphics::camera::object &_camera)
:
	world_(
		_world),
	renderer_(
		_renderer),
	camera_(
		_camera),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("debug_vertex.glsl"),
		media_path()/FCPPT_TEXT("debug_fragment.glsl")),
	debug_mode_(
		btIDebugDraw::DBG_NoDebug) // should be zero
{
	world_.handle().setDebugDrawer(
		this);

	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());
}

// @override
void
insula::physics::debug_drawer::drawLine(
	btVector3 const &from,
	btVector3 const &to,
	btVector3 const &color)
{
	drawLine(
		from,
		to,
		color,
		color);
}

// @override
void
insula::physics::debug_drawer::drawLine(
	btVector3 const &from,
	btVector3 const &to,
	btVector3 const &from_color,
	btVector3 const &to_color)
{
	lines_.push_back(
		std::make_tuple(
			fcppt::math::vector::structure_cast<graphics::vec3>(
				bullet_to_vec3(
					from)),
			fcppt::math::vector::structure_cast<graphics::vec3>(
				bullet_to_vec3(
					from_color))));

	lines_.push_back(
		std::make_tuple(
			fcppt::math::vector::structure_cast<graphics::vec3>(
				bullet_to_vec3(
					to)),
			fcppt::math::vector::structure_cast<graphics::vec3>(
				bullet_to_vec3(
					to_color))));
	
}

void
insula::physics::debug_drawer::drawContactPoint(
	btVector3 const &point,
	btVector3 const &normal,
	btScalar const distance,
	int const /*lifeTime*/,
	btVector3 const &color)
{
	drawLine(
		point,
		point + normal * distance,
		color);
}

void	
insula::physics::debug_drawer::draw3dText(
	btVector3 const&,
	char const*)
{
}

void
insula::physics::debug_drawer::reportErrorWarning(
	char const* warningString)
{
	fcppt::io::cerr 
		<< FCPPT_TEXT("Debug drawer warning: ") 
		<< fcppt::from_std_string(warningString) 
		<< FCPPT_TEXT("\n");
}

void
insula::physics::debug_drawer::setDebugMode(
	int const _debug_mode)
{
	debug_mode_ = _debug_mode;
}

int 
insula::physics::debug_drawer::getDebugMode() const
{
	return debug_mode_;
}

void
insula::physics::debug_drawer::render()
{
	lines_.clear();
	world_.handle().debugDrawWorld();

	if (lines_.empty())
		return;

	FCPPT_ASSERT(
		lines_.size() % 2 == 0);

	sge::renderer::state::scoped const sstate(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::off));

	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		camera_.perspective() * camera_.world());

	vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vertex_format>(),
			static_cast<sge::renderer::size_type>(
				lines_.size()),
			sge::renderer::resource_flags::none);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	{
		sge::renderer::scoped_vertex_lock const vblock(
			vb_,
			sge::renderer::lock_mode::writeonly);

		vertex_view const vertices(
			vblock.value());

		vertex_view::iterator vb_it(
			vertices.begin());

		BOOST_FOREACH(line_cache::const_reference l,lines_)
		{
			// Technically, we'd have to cast to vf_position::packed_type
			// here, but screw that
			(vb_it)->set<vf_position>(
				std::get<0>(l));
			(vb_it++)->set<vf_color>(
				std::get<1>(l));
		}
	}

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::line);
}

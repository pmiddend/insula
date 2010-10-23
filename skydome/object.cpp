#include "object.hpp"
#include "parameters.hpp"
#include "index_visitor.hpp"
#include "vf/packed_position.hpp"
#include "vf/format.hpp"
#include "vf/vertex_view.hpp"
#include "../json/find_member.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../media_path.hpp"
#include "../timed_output.hpp"
#include "../math/sphere_point.hpp"
#include "../shadow/object.hpp"
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
#include <sge/renderer/state/color.hpp>
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
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_index_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba32f.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/file.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/math/rad_to_deg.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <boost/foreach.hpp>
#include <functional>
#include <type_traits>
#include <cmath>
#include <algorithm>

insula::skydome::object::object(
	parameters const &params)
:
	camera_(
		params.camera),
	renderer_(
		params.systems.renderer()),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("skydome_vertex.glsl"),
		media_path()/FCPPT_TEXT("skydome_fragment.glsl"),
		graphics::shader::vf_to_string<vf::format>(),
		{
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4()),
			graphics::shader::variable(
				"sun_position",
				graphics::shader::variable_type::uniform,
				math::sphere_point(
					static_cast<graphics::scalar>(1),
					fcppt::math::deg_to_rad(
						static_cast<graphics::scalar>(20)),
					fcppt::math::deg_to_rad(
						static_cast<graphics::scalar>(0)))),
			graphics::shader::variable(
				"color0",
				graphics::shader::variable_type::const_,
				json::find_member<graphics::vec3>(
					params.config_file,
					FCPPT_TEXT("skydome/color0"))),
			graphics::shader::variable(
				"color1",
				graphics::shader::variable_type::const_,
				json::find_member<graphics::vec3>(
					params.config_file,
					FCPPT_TEXT("skydome/color1")))
		},
		graphics::shader::sampler_sequence()),
	perspective_(
		fcppt::math::matrix::perspective(
			camera_.aspect(),
			camera_.fov(),
			0.1f,
			3.0f)),
	backend_(
		params.scene_manager,
		{"water","normal"},
		scene::backend_priority::high),
	render_connection_(
		backend_.register_callback(
			std::bind(
				&object::render,
				this,
				std::placeholders::_1))),
	shadow_update_connection_(
		params.shadow.register_callback(
			std::bind(
				&object::shadow_update,
				this,
				std::placeholders::_1,
				std::placeholders::_2)))
{
	graphics::vec3 const color0 = 	
		json::find_member<graphics::vec3>(
			params.config_file,
			FCPPT_TEXT("skydome/color0"));

	// _Permanently_ change the renderer's clear color (this could be a
	// scoped_state, too)
	renderer_->state(
		sge::renderer::state::list
			(sge::renderer::state::color::clear_color = 
				sge::image::color::rgba32f(
					(sge::image::color::init::red %= color0[0])
					(sge::image::color::init::green %= color0[1])
					(sge::image::color::init::blue %= color0[2])
					(sge::image::color::init::alpha %= 1.0))));

	using graphics::scalar; 

	scalar const radius = 
		static_cast<scalar>(
			1);

	size_type const 
		lats = 
			json::find_member<size_type>(
				params.config_file,
				FCPPT_TEXT("skydome/lats")),
		lons = 
			json::find_member<size_type>(
				params.config_file,
				FCPPT_TEXT("skydome/lons"));

	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				// The top
				1 + 
				// The inner rings minus the two tops
				lats * lons),
			sge::renderer::resource_flags::none);

	FCPPT_ASSERT_MESSAGE(
		vb_->size() < 
		static_cast<sge::renderer::size_type>(
			std::numeric_limits<sge::renderer::index::i32>::max()),
		FCPPT_TEXT("The skydome is too big for a 32 bit index!"));

	ib_ = 
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			// For each pair of rings we have "it_lon" quads. We have
			// "it_lat-1" rings and "it_lat-2" pairs of rings.  This makes for
			// "it_lat-2" quads, each having 2 triangles and each triangle has
			// 3 indices.
			static_cast<sge::renderer::size_type>(
				// the quads
				(lats-1) * lons * 2 * 3 +
				// the triangles
				lons * 3), 
			sge::renderer::resource_flags::none);

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	scalar const halfpi = 
		fcppt::math::pi<scalar>()/
		static_cast<scalar>(2);

	// The top
	vb_it->set<vf::position>(
		math::sphere_point(
			radius,
			static_cast<scalar>(0),
			static_cast<scalar>(0)));
	
	vb_it++;

	vf::vertex_view::size_type vertex_count = 
		1;

	scalar const 
		increment_lat = 
			halfpi / static_cast<scalar>(lats),
		increment_lon = 
			fcppt::math::twopi<scalar>()/
			static_cast<scalar>(lons);

	scalar lat = increment_lat;
	// We make one less iteration because we don't want the top
	// NOTE: We use size_type here FOR A REASON! floating point loops are inaccurate
	for (size_type i = 0; i < lats; ++i)
	{
		scalar lon = 
			static_cast<scalar>(0);
		for (size_type j = 0; j < lons; ++j)
		{
			++vertex_count;

	//		fcppt::io::cout << sphere_point(radius,lat,lon).y() << "\n";

			vb_it->set<vf::position>(
				math::sphere_point(
					radius,
					lat,
					lon));

			++vb_it;
			lon += increment_lon;
		}
		lat += increment_lat;
	}
	
	FCPPT_ASSERT(vertex_count <= vb_->size());

	fcppt::variant::apply_unary(
		index_visitor(
			static_cast<size_type>(
				ib_->size()),
			lats,
			lons),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());
}

insula::skydome::object::~object() {}

void
insula::skydome::object::render(
	scene::render_pass::object const &)
{
	// FIRST the shader, THEN the vertex buffer
	graphics::shader::scoped scoped_shader_(
		shader_);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	// We have to set our own perspective matrix here because near and far
	// might be ill-chosen by the user (at least for the skydome)
	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		perspective_ * camera_.rotation());

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(sge::renderer::state::cull_mode::off)
		 	(sge::renderer::state::depth_func::off));

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

void
insula::skydome::object::shadow_update(
	graphics::scalar,
	graphics::gizmo const &sun_gizmo)
{
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		"sun_position",
		fcppt::math::vector::normalize(
			sun_gizmo.position()));
}

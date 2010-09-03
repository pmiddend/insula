#include "game_inner.hpp"
#include "../timed_output.hpp"
#include "../get_option.hpp"
#include "../create_path.hpp"
#include "../media_path.hpp"
#include "../height_map/object.hpp"
#include "../height_map/vec2.hpp"
#include "../height_map/height_for_point.hpp"
#include "../physics/box.hpp"
#include "../physics/vec3.hpp"
#include "../physics/shape_from_model.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../graphics/camera/object.hpp"
#include "../model/scoped.hpp"
#include "../model/vf/format.hpp"
#include "../events/vehicle_nugget_collision.hpp"
// vehicle begin
#include "../vehicle/cli_factory.hpp"
#include "../vehicle/object.hpp"
#include "../height_map/height_for_point.hpp"
#include "../height_map/vec2.hpp"
// vehicle end
#include <sge/console/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/texture.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/string.hpp>
#include <boost/foreach.hpp>
#include <functional>

insula::states::game_inner::game_inner(
	my_context ctx)
:
	my_base(ctx),
	current_player_(
		context<game_outer>().next_player()),
	physics_world_(
		context<machine>().camera(),
		fcppt::math::box::structure_cast<physics::box>(
			context<game_outer>().height_map().extents()),
		get_option<physics::vec3>(
			context<machine>().cli_variables(),
			"physics-gravity")),
	physics_height_map_(
		physics_world_,
		context<game_outer>().height_map().heights(),
		context<game_outer>().height_map().cell_size(),
		context<game_outer>().height_map().height_scaling()),
	physics_debug_drawer_(
		physics_world_,
		context<machine>().systems().renderer(),
		context<machine>().camera()),
	physics_debug_(
		false),
	toggle_physics_debug_(
		context<machine>().console().model().insert(
			FCPPT_TEXT("debug_physics"),
			[&physics_debug_](
				sge::console::arg_list const &,
				sge::console::object &)
			{
				physics_debug_ = !physics_debug_;
			},
			FCPPT_TEXT("Toggle the physics debug drawer"))),
	nugget_shader_(
		context<machine>().systems().renderer(),
		media_path()/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("model_fragment.glsl"),
		graphics::shader::vf_to_string<model::vf::format>(),
		fcppt::assign::make_container<graphics::shader::variable_sequence>
		(
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4())),
		fcppt::assign::make_container<graphics::shader::sampler_sequence>
		(
		graphics::shader::sampler(
			"texture",
			sge::renderer::texture_ptr()))),
	nugget_texture_(
		sge::image::create_texture(
			create_path(
				get_option<fcppt::string>(
					context<machine>().cli_variables(),
					"game-nugget-texture"),
				FCPPT_TEXT("textures")),
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	nugget_model_(
		context<machine>().systems().md3_loader()->load(
			create_path(
				get_option<fcppt::string>(
					context<machine>().cli_variables(),
					"game-nugget-model"),
				FCPPT_TEXT("models")),
			sge::model::load_flags::switch_yz),
			context<machine>().systems().renderer()),
	vehicle_(
		insula::vehicle::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().systems(),
			context<machine>().camera(),
			nugget_shader_,
			physics_world_,
			physics::vec3(
				static_cast<physics::scalar>(
					context<game_outer>().vehicle_position().x()),
				static_cast<physics::scalar>(
					height_map::height_for_point(
						context<game_outer>().height_map().heights(),
						context<game_outer>().height_map().cell_size(),
						fcppt::math::vector::structure_cast<height_map::vec2>(
							context<game_outer>().vehicle_position())) * 
						context<game_outer>().height_map().height_scaling() + 
						static_cast<height_map::scalar>(
							10)),
				static_cast<physics::scalar>(
					context<game_outer>().vehicle_position().y())),
			context<machine>().input_delegator(),
			context<machine>().console())),
	vehicle_static_connection_(
		context<game_inner>().physics_world().register_vehicle_static_callback(
			std::bind(
				&game_inner::vehicle_static_callback,
				this,
				std::placeholders::_1,
				std::placeholders::_2)))
{
	// stdlib::map doesn't work here
	BOOST_FOREACH(
		graphics::vec2 const &v,
		context<game_outer>().nugget_positions())
	{
		nugget_models_.push_back(
			new physics::static_model(
				physics_world_,
				physics::vec3(
					v.x(),
					height_map::height_for_point(
						context<game_outer>().height_map().heights(),
						context<game_outer>().height_map().cell_size(),
						fcppt::math::vector::structure_cast<height_map::vec2>(
							v)) * context<game_outer>().height_map().height_scaling() + 
					static_cast<physics::scalar>(nugget_model_.bounding_box().h())/**
					static_cast<physics::scalar>(1.5)*/,
					v.y()),
				physics::shape_from_model(
					nugget_model_,
					physics::model_approximation(
						physics::model_approximation::box,
						static_cast<physics::scalar>(1))),
				physics::solidity::nonsolid));
	}
}

void
insula::states::game_inner::react(
	events::tick const &)
{
	BOOST_FOREACH(physics::static_model *m,to_delete_)
	{
		fcppt::algorithm::ptr_container_erase(
			nugget_models_,
			m);
		context<machine>().sounds().play(
			FCPPT_TEXT("score"));
	}
	to_delete_.clear();
	physics_world_.update_visibility();
}

void
insula::states::game_inner::react(
	events::render const &)
{
	{
		// FIRST update texture, THEN scope the shader!
		nugget_shader_.update_texture(
			"texture",
			nugget_texture_);

	//	fcppt::io::cout << "scoping shader\n";
		graphics::shader::scoped scoped_shader(
			nugget_shader_);

	//	fcppt::io::cout << "scoping model\n";
		model::scoped scoped_model(
			context<machine>().systems().renderer(),
			nugget_model_);

		unsigned ticker = 0;

		BOOST_FOREACH(
			physics::static_model &m,
			nugget_models_)
		{
			if (m.last_seen() != physics_world_.current_iteration())
				continue;

			ticker++;

			//if (m.last_seen() == physics_world_.current_iteration())
				//timed_output() << ticker++ << "Seeing something!\n";
				//fcppt::io::cout << "Saw something!\n";

			nugget_shader_.set_uniform(
				"mvp",
				context<machine>().camera().perspective() * 
				context<machine>().camera().world() * 
				fcppt::math::matrix::structure_cast<graphics::mat4>(
					m.world_transform()));

			nugget_model_.render();
		}

		timed_output() << "Drew " << ticker << " static models\n";
	}
	
	if (physics_debug_)
	{
		physics_debug_drawer_.setDebugMode(
			btIDebugDraw::DBG_DrawWireframe);
		physics_debug_drawer_.render(); 
	}

	vehicle_.render();
}

insula::vehicle::object &
insula::states::game_inner::vehicle()
{
	return vehicle_;
}

insula::player const &
insula::states::game_inner::current_player() const
{
	return current_player_;
}

insula::turn_timer &
insula::states::game_inner::turn_timer()
{
	return turn_timer_;
}

insula::turn_timer const &
insula::states::game_inner::turn_timer() const
{
	return turn_timer_;
}

insula::physics::world &
insula::states::game_inner::physics_world()
{
	return physics_world_;
}

void
insula::states::game_inner::erase_nugget(
	physics::static_model &m)
{
	// Let's hope it's really a nugget!
	FCPPT_ASSERT(
		std::find_if(
			nugget_models_.begin(),
			nugget_models_.end(),
			[&m](physics::static_model const &m2) 
			{
				return &m == &m2;
			}) != nugget_models_.end());

	// If so, schedule for deletion (lazy deletion!)
	to_delete_.insert(
		&m);
}

std::size_t
insula::states::game_inner::nugget_count() const
{
	return nugget_models_.size();
}

insula::states::game_inner::~game_inner()
{
}

void
insula::states::game_inner::vehicle_static_callback(
	physics::vehicle::object &v,
	physics::static_model &s)
{
	post_event(
		events::vehicle_nugget_collision(
			v,
			s));
}

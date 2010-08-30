#include "game_inner.hpp"
#include "../get_option.hpp"
#include "../create_path.hpp"
#include "../media_path.hpp"
#include "../height_map/object.hpp"
#include "../height_map/vec2.hpp"
#include "../height_map/height_for_point.hpp"
#include "../physics/box.hpp"
#include "../physics/vec3.hpp"
#include "../graphics/vec2.hpp"
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
#include <fcppt/text.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
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
		media_path()/FCPPT_TEXT("model_fragment.glsl")),
	nugget_model_(
		context<machine>().camera(),
		context<machine>().systems().md3_loader()->load(
			create_path(
				get_option<fcppt::string>(
					context<machine>().cli_variables(),
					"game-nugget-model"),
				FCPPT_TEXT("models"))),
			context<machine>().systems().renderer(),
			nugget_shader_,
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
	vehicle_(
		insula::vehicle::cli_factory(
			context<machine>().cli_variables(),
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
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader(),
			context<machine>().systems().md3_loader(),
			nugget_shader_,
			context<machine>().camera(),
			context<machine>().input_delegator(),
			context<machine>().console(),
			context<machine>().systems().audio_loader(),
			context<machine>().systems().audio_player())),
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
				nugget_model_,
				physics::model_approximation(
					physics::model_approximation::box,
					static_cast<physics::scalar>(1)),
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
}

void
insula::states::game_inner::react(
	events::render const &)
{
	BOOST_FOREACH(physics::static_model &m,nugget_models_)
		m.render();
	
	if (physics_debug_)
	{
		physics_debug_drawer_.setDebugMode(
			btIDebugDraw::DBG_DrawWireframe);
		physics_debug_drawer_.render(); 
	}

	vehicle_->render();
}

insula::vehicle::object &
insula::states::game_inner::vehicle()
{
	return *vehicle_;
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

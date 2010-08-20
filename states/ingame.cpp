#include "ingame.hpp"
#include "../get_option.hpp"
#include "../machine.hpp"
#include "../height_map/cli_factory.hpp"
#include "../height_map/object.hpp"
#include "../skydome/cli_factory.hpp"
#include "../skydome/object.hpp"
#include "../water/cli_factory.hpp"
#include "../water/object.hpp"
#include "../physics/box.hpp"
#include "../physics/vec3.hpp"
#include "../graphics/box.hpp"
#include "../console/object.hpp"
#include <sge/console/object.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/text.hpp>

insula::states::ingame::ingame(
	my_context ctx)
:
	my_base(
		ctx),
	height_map_(
		height_map::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().camera(),
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader())),
	height_map_console_(
		*height_map_,
		context<machine>().console().model(),
		context<machine>().systems().image_loader()),
	physics_world_(
		fcppt::math::box::structure_cast<physics::box>(
			height_map_->extents()),
		get_option<physics::vec3>(
			context<machine>().cli_variables(),
			"physics-gravity")),
	physics_height_map_(
		physics_world_,
		height_map_->heights(),
		height_map_->cell_size(),
		height_map_->height_scaling()),
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
	skydome_(
		skydome::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().camera(),
			context<machine>().systems().renderer())),
	skydome_console_(
		*skydome_,
		context<machine>().console().model()),
	water_(
		water::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().systems().renderer(),
			context<machine>().camera(),
			fcppt::math::box::structure_cast<graphics::box>(
				height_map_->extents()),
			context<machine>().systems().image_loader())),
	water_console_(
		*water_,
		context<machine>().console().model())
{
}

boost::statechart::result
insula::states::ingame::react(
	events::tick const &)
{
	water_->update_reflection(
		[&skydome_,&height_map_,&water_]()
		{
			skydome_->render();
			height_map_->render(
				sge::renderer::state::cull_mode::off,
				water_->water_level());
		});

	return discard_event();
}

boost::statechart::result
insula::states::ingame::react(
	events::render const &)
{
	skydome_->render();
	height_map_->render(
		sge::renderer::state::cull_mode::back);
	water_->render();

	if (toggle_physics_debug_)
	{
		physics_debug_drawer_.setDebugMode(
			btIDebugDraw::DBG_DrawWireframe);
		physics_debug_drawer_.render(); 
	}

	return discard_event();
}

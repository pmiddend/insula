#include "game_inner.hpp"
#include "../get_option.hpp"
#include "../create_path.hpp"
#include "../media_path.hpp"
#include "../height_map/object.hpp"
#include "../height_map/vec2.hpp"
#include "../physics/box.hpp"
#include "../physics/vec3.hpp"
#include "../physics/shape_from_model.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../graphics/camera/object.hpp"
#include "../model/scoped.hpp"
#include "../model/vf/format.hpp"
#include "../player/parameters.hpp"
#include "../nugget/instance.hpp"
#include "../events/nuggets_empty.hpp"
#include <sge/console/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/io/cout.hpp>
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
	current_player_name_(
		context<game_outer>().next_player()),
	physics_world_(
		fcppt::math::box::structure_cast<physics::box>(
			context<game_outer>().height_map().extents()),
		get_option<physics::vec3>(
			context<machine>().cli_variables(),
			"physics-gravity"),
		context<game_outer>().broadphase_manager()),
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
	props_(
		context<game_outer>().prop_manager().instantiate(
			physics_world_)),
	player_(
		player::parameters(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				context<machine>().config_file().members,
				FCPPT_TEXT("player")),
			context<game_outer>().height_map(),
			context<game_outer>().water_level(),
			context<game_outer>().player_position_rng(),
			context<machine>().camera(),
			context<machine>().input_delegator(),
			physics_world_))
{
}

void
insula::states::game_inner::react(
	events::tick const &)
{
	//nuggets_->update();
}

void
insula::states::game_inner::react(
	events::render const &)
{
	context<game_outer>().scene_manager().render();
	
	if (physics_debug_)
	{
		physics_debug_drawer_.setDebugMode(
			btIDebugDraw::DBG_DrawWireframe);
		physics_debug_drawer_.render(); 
	}
}

fcppt::string const &
insula::states::game_inner::current_player_name() const
{
	return current_player_name_;
}

insula::physics::world &
insula::states::game_inner::physics_world()
{
	return physics_world_;
}

/*
insula::nugget::instance &
insula::states::game_inner::nugget_instance()
{
	return *nuggets_;
}

insula::nugget::instance const &
insula::states::game_inner::nugget_instance() const
{
	return *nuggets_;
}
*/

insula::player::object &
insula::states::game_inner::player()
{
	return player_;
}

insula::player::object const &
insula::states::game_inner::player() const
{
	return player_;
}

insula::states::game_inner::~game_inner()
{
}

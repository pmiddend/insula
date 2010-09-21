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
#include "../nugget/instance.hpp"
#include "../events/nuggets_empty.hpp"
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
	current_player_(
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
	nuggets_(
		context<game_outer>().nugget_manager().instantiate(
			physics_world_)),
	nugget_empty_connection_(
		nuggets_->register_empty_callback(
			[this]() 
			{
				this->post_event(
					events::nuggets_empty());
			})),
	vehicle_(
		insula::vehicle::cli_factory(
			context<machine>().cli_variables(),
			context<game_outer>().scene_manager(),
			context<machine>().systems(),
			context<machine>().camera(),
			context<game_outer>().model_shader(),
			physics_world_,
			physics::vec3(
				static_cast<physics::scalar>(
					context<game_outer>().vehicle_position().x()),
				static_cast<physics::scalar>(
					context<game_outer>().height_map().project(
						fcppt::math::vector::structure_cast<height_map::vec2>(
							context<game_outer>().vehicle_position())) + 
						static_cast<height_map::scalar>(
							3)),
				static_cast<physics::scalar>(
					context<game_outer>().vehicle_position().y())),
			context<machine>().input_delegator(),
			context<machine>().console())),
	props_(
		context<game_outer>().prop_manager().instantiate(
			physics_world_))
{
}

void
insula::states::game_inner::react(
	events::tick const &)
{
	nuggets_->update();
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

insula::states::game_inner::~game_inner()
{
	fcppt::io::cout << "~game_inner\n";
}

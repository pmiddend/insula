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
#include <sge/console/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/texture.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <boost/foreach.hpp>

insula::states::game_inner::game_inner(
	my_context ctx)
:
	my_base(ctx),
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
				sge::renderer::resource_flags::none))
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
					static_cast<physics::scalar>(nugget_model_.bounding_box().h())*
					static_cast<physics::scalar>(1.5),
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
}

insula::states::game_inner::~game_inner()
{
}

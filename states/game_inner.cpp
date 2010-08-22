#include "game_inner.hpp"
#include "../get_option.hpp"
#include "../create_path.hpp"
#include "../media_path.hpp"
#include "../height_map/object.hpp"
#include "../physics/box.hpp"
#include "../physics/vec3.hpp"
#include <sge/console/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/texture.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

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

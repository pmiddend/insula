#include "game_outer.hpp"
#include "../generate_nuggets.hpp"
#include "../get_option.hpp"
#include "../height_map/cli_factory.hpp"
#include "../height_map/object.hpp"
#include "../skydome/cli_factory.hpp"
#include "../skydome/object.hpp"
#include "../water/cli_factory.hpp"
#include "../water/object.hpp"
#include <fcppt/math/box/structure_cast.hpp>
#include <sge/renderer/state/cull_mode.hpp>

insula::states::game_outer::game_outer(
	my_context ctx)
:
	my_base(
		ctx),
	height_map_(
		insula::height_map::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().camera(),
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader())),
	height_map_console_(
		*height_map_,
		context<machine>().console().model(),
		context<machine>().systems().image_loader()),
	skydome_(
		skydome::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().camera(),
			context<machine>().systems().renderer())),
	skydome_console_(
		*skydome_,
		context<machine>().console().model()),
	water_(
		insula::water::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().systems().renderer(),
			context<machine>().camera(),
			fcppt::math::box::structure_cast<graphics::box>(
				height_map_->extents()),
			context<machine>().systems().image_loader())),
	water_console_(
		*water_,
		context<machine>().console().model()),
	nugget_positions_(
		generate_nuggets(
			*height_map_,
			water_->water_level(),
			get_option<nugget_sequence::size_type>(
				context<machine>().cli_variables(),
				"game-nuggets")))
{
}

insula::height_map::object &
insula::states::game_outer::height_map()
{
	return *height_map_;
}

insula::height_map::object const &
insula::states::game_outer::height_map() const
{
	return *height_map_;
}

void
insula::states::game_outer::react(
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
}

void
insula::states::game_outer::react(
	events::render const &)
{
	skydome_->render();
	height_map_->render(
		sge::renderer::state::cull_mode::back);
	water_->render();
}

insula::states::game_outer::~game_outer()
{
}
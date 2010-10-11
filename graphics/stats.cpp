#include "stats.hpp"
#include <sge/renderer/device.hpp>
#include <sge/font/system.hpp>
#include <sge/config/media_path.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/drawer_3d.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/dim.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/draw_text.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/font/text_part.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/make_shared_ptr.hpp>

insula::graphics::stats::stats(
	sge::renderer::device_ptr const _renderer,
	sge::font::system_ptr const fs)
:
	renderer_(
		_renderer),
	metrics_(
		fs->create_font(
			sge::config::media_path()/FCPPT_TEXT("fonts")/FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(15))),
	drawer_(
		fcppt::make_shared_ptr<sge::font::drawer_3d>(
			renderer_,
			sge::image::colors::white()))
{
}

void
insula::graphics::stats::update_and_render(
	fcppt::string const &additional_stats)
{
	counter_.update();

	sge::font::draw_text(
		metrics_,
		drawer_,
		FCPPT_TEXT("FPS: ")+counter_.frames_str()+FCPPT_TEXT("\n")+additional_stats,
		sge::font::pos::null(),
		fcppt::math::dim::structure_cast<sge::font::dim>(
			renderer_->screen_size()),
		sge::font::align_h::left,
		sge::font::align_v::top,
		sge::font::flags::none);
}

insula::graphics::stats::~stats() {}

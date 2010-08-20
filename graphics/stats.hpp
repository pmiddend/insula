#ifndef INSULA_GRAPHICS_STATS_HPP_INCLUDED
#define INSULA_GRAPHICS_STATS_HPP_INCLUDED

#include <sge/time/frames_counter.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/drawer_ptr.hpp>
#include <sge/font/system_ptr.hpp>
#include <fcppt/text.hpp>

namespace insula
{
namespace graphics
{
/// The stats class currently has no well-defined
/// functionality. Originally, this was just the frame counter plus a
/// font for it. But then I wanted to display the current vehicle
/// speed, so I invented this class. It might get deprecated later on
/// (or extended).
class stats
{
public:
	stats(stats const &) = delete;
	stats &operator=(stats const &) = delete;
	
	explicit
	stats(
		sge::renderer::device_ptr,
		sge::font::system_ptr);

	void
	update_and_render(
		fcppt::string const &additional_stats = FCPPT_TEXT(""));
private:
	sge::time::frames_counter counter_;
	sge::renderer::device_ptr const renderer_;
	sge::font::metrics_ptr metrics_;
	sge::font::drawer_ptr drawer_;
};
}
}

#endif

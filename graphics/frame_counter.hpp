#ifndef INSULA_GRAPHICS_FRAME_COUNTER_HPP_INCLUDED
#define INSULA_GRAPHICS_FRAME_COUNTER_HPP_INCLUDED

#include <sge/time/frames_counter.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/drawer_ptr.hpp>
#include <sge/font/system_ptr.hpp>

namespace insula
{
namespace graphics
{
class frame_counter
{
public:
	frame_counter(frame_counter const &) = delete;
	frame_counter &operator=(frame_counter const &) = delete;
	
	explicit
	frame_counter(
		sge::renderer::device_ptr,
		sge::font::system_ptr);

	void
	update_and_render();
private:
	sge::time::frames_counter counter_;
	sge::renderer::device_ptr const renderer_;
	sge::font::metrics_ptr metrics_;
	sge::font::drawer_ptr drawer_;
};
}
}

#endif

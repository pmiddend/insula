#ifndef INSULA_GRAPHICS_SHADERS_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADERS_HPP_INCLUDED

#include "mat4.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>

namespace insula
{
namespace graphics
{
class shaders
{
FCPPT_NONCOPYABLE(shaders)
public:
	explicit
	shaders(
		sge::renderer::device_ptr,
		fcppt::filesystem::path const &vertex,
		fcppt::filesystem::path const &fragment);
	
	void
	mvp(
		mat4 const &);
private:
	sge::renderer::glsl::uniform::variable_ptr mvp_;
};
}
}

#endif

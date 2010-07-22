#ifndef INSULA_GRAPHICS_SHADERS_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADERS_HPP_INCLUDED

#include "mat4.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/program_ptr.hpp>

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
	
	sge::renderer::glsl::program_ptr const
	program();
	
	void
	world(
		mat4 const &);

	void
	perspective(
		mat4 const &);
	
	~shaders();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::glsl::program_ptr program_;
	sge::renderer::glsl::uniform::variable_ptr world_,perspective_;
	sge::renderer::glsl::uniform::variable_ptr sand_,rock_,grass_;
};
}
}

#endif

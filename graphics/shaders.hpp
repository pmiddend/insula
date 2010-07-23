#ifndef INSULA_GRAPHICS_SHADERS_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADERS_HPP_INCLUDED

#include "mat4.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/uniform/single_value.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/renderer/glsl/program_ptr.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/container/map.hpp>
#include <map>

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
	
	template<typename T>
	void
	set_uniform(
		fcppt::string const &name,
		T const &t)
	{
		if (uniforms_.find(name) == uniforms_.end())
			uniforms_.insert(
				name,
				program_->uniform(name));

		sge::renderer::glsl::uniform::single_value(
			uniforms_[name],
			t);
	}
private:
	typedef
	fcppt::container::map
	<
		std::map
		<
			fcppt::string,
			sge::renderer::glsl::uniform::variable_ptr
		>
	>
	uniform_map;

	sge::renderer::device_ptr const renderer_;
	sge::renderer::glsl::program_ptr program_;
	uniform_map uniforms_;
};
}
}

#endif

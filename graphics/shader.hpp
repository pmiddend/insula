#ifndef INSULA_GRAPHICS_SHADER_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_HPP_INCLUDED

#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/uniform/single_value.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/renderer/glsl/program_ptr.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/container/map.hpp>
#include <map>

namespace insula
{
namespace graphics
{
class shader
{
public:
	explicit
	shader(
		sge::renderer::device_ptr,
		fcppt::filesystem::path const &vertex,
		fcppt::filesystem::path const &fragment);
	
	shader &operator=(
		shader const &) = delete;

	shader(
		shader const&) = delete;

	
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

	void
	activate();

	void
	deactivate();
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

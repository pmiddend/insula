#ifndef INSULA_GRAPHICS_SHADER_OBJECT_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_OBJECT_HPP_INCLUDED

#include "variable.hpp"
#include "variable_sequence.hpp"
#include "sampler_sequence.hpp"
#include "sampler.hpp"
#include "value_type.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/glsl/program_ptr.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <vector>
#include <unordered_map>

namespace insula
{
namespace graphics
{
namespace shader
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit
	object(
		sge::renderer::device_ptr,
		fcppt::filesystem::path const &vertex,
		fcppt::filesystem::path const &fragment,
		fcppt::string const &format_declaration,
		variable_sequence const &variables,
		sampler_sequence const &samplers);

	void
	set_uniform(
		sge::renderer::glsl::string const &name,
		value_type const &);

	void
	update_texture(
		sge::renderer::glsl::string const &name,
		sge::renderer::texture_ptr);

	sge::renderer::glsl::program_ptr const
	program();

	void
	activate();

	void
	deactivate();

	~object();
private:
	typedef
	std::unordered_map
	<
		sge::renderer::glsl::string,
		sge::renderer::glsl::uniform::variable_ptr
	>
	uniform_map;

	sge::renderer::device_ptr renderer_;
	sge::renderer::glsl::program_ptr program_;
	uniform_map uniforms_;
	sampler_sequence samplers_;
};
}
}
}

#endif

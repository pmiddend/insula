#ifndef INSULA_GRAPHICS_SHADER_OBJECT_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_OBJECT_HPP_INCLUDED

#include "variable.hpp"
#include "sampler.hpp"
#include "value_type.hpp"
#include <sge/renderer/device_ptr.hpp>
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
	typedef
	std::vector<variable>
	variable_sequence;

	typedef
	std::vector<sampler>
	sampler_sequence;


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
		fcppt::string const &name,
		value_type const &);

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

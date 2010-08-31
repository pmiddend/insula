#ifndef INSULA_GRAPHICS_SHADER_SAMPLER_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_SAMPLER_HPP_INCLUDED

#include <sge/renderer/glsl/string.hpp>
#include <sge/renderer/texture_ptr.hpp>

namespace insula
{
namespace graphics
{
namespace shader
{
class sampler
{
public:
	typedef
	int
	texture_unit_type;

	explicit
	sampler(
		sge::renderer::glsl::string const &name,
		sge::renderer::texture_ptr);

	sge::renderer::glsl::string const
	name() const;

	sge::renderer::glsl::string const
	declaration() const;

	// The shader sets this
	void
	texture_unit(
		texture_unit_type);

	// This is needed on shader activation
	texture_unit_type
	texture_unit() const;

	// This is needed on shader activation
	sge::renderer::texture_ptr const
	texture() const;
	
	~sampler();
private:
	sge::renderer::glsl::string name_,declaration_;
	sge::renderer::texture_ptr texture_;
	texture_unit_type texture_unit_;
};
}
}
}

#endif

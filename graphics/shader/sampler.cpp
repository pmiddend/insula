#include "sampler.hpp"
#include <fcppt/text.hpp>
#include <limits>

insula::graphics::shader::sampler::sampler(
	sge::renderer::glsl::string const &_name,
	sge::renderer::texture_ptr const _texture)
:
	name_(
		_name),
	declaration_(
		FCPPT_TEXT("uniform sampler2D ")+_name+FCPPT_TEXT(";")),
	texture_(
		_texture),
	texture_unit_(
		std::numeric_limits<texture_unit_type>::max())
{
}

sge::renderer::glsl::string const
insula::graphics::shader::sampler::name() const
{
	return name_;
}

sge::renderer::glsl::string const
insula::graphics::shader::sampler::declaration() const
{
	return declaration_;
}

void
insula::graphics::shader::sampler::texture_unit(
	texture_unit_type const _texture_unit)
{
	texture_unit_ = _texture_unit;
}

// This is needed on shader activation
insula::graphics::shader::sampler::texture_unit_type
insula::graphics::shader::sampler::texture_unit() const
{
	return texture_unit_;
}

// This is needed on shader activation
sge::renderer::texture_ptr const
insula::graphics::shader::sampler::texture() const
{
	return texture_;
}

insula::graphics::shader::sampler::~sampler()
{
}

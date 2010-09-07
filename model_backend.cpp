#include "model_backend.hpp"
#include "graphics/shader/object.hpp"
#include <fcppt/assert.hpp>
#include <boost/foreach.hpp>

insula::model_backend::model_backend(
	sge::renderer::device_ptr _renderer,
	graphics::camera::object &_camera,
	graphics::shader::object &_shader,
	texture_map const &_textures,
	model::object &_model)
:
	renderer_(
		_renderer),
	camera_(
		_camera),
	shader_(
		_shader),
	textures_(
		_textures),
	model_(
		_model)
{
}

void
insula::model_backend::begin()
{
	FCPPT_ASSERT(!model_scope_ && !shader_scope_);

	BOOST_FOREACH(
		texture_map::const_reference r,
		textures_)
		shader_.update_texture(
			r.first,
			r.second);

	shader_scope_.reset(
		new graphics::shader::scoped(
			shader_));

	model_scope_.reset(
		new model::scoped(
			renderer_,
			model_));
}

void 
insula::model_backend::end()
{
	model_scope_.reset();
	shader_scope_.reset();
}

insula::graphics::camera::object &
insula::model_backend::camera() const
{
	return camera_;
}

insula::graphics::shader::object &
insula::model_backend::shader() const
{
	return shader_;
}

insula::model::object &
insula::model_backend::model() const
{
	return model_;
}

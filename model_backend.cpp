#include "model_backend.hpp"
#include "model/object.hpp"
#include "graphics/shader/object.hpp"
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/source_blend_func.hpp>
#include <sge/renderer/state/dest_blend_func.hpp>
#include <sge/renderer/device.hpp>
#include <fcppt/assert.hpp>
#include <boost/foreach.hpp>

insula::model_backend::model_backend(
	bool _is_transparent,
	sge::renderer::device_ptr _renderer,
	graphics::camera::object &_camera,
	graphics::shader::object &_shader,
	texture_map const &_textures,
	model::object &_model)
:
	is_transparent_(
		_is_transparent),
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
	// Update the shader textures (before scoping it)
	BOOST_FOREACH(
		texture_map::const_reference r,
		textures_)
		shader_.update_texture(
			r.first,
			r.second);

	shader_.activate();
	renderer_->set_vertex_buffer(
		model_.vb());

	sge::renderer::state::list const 
		nontransparent_list = 
			sge::renderer::state::list
				(sge::renderer::state::cull_mode::back)
				(sge::renderer::state::depth_func::less),
		transparent_list = 
			sge::renderer::state::list
				(sge::renderer::state::cull_mode::back)
				(sge::renderer::state::depth_func::off)
				(sge::renderer::state::bool_::enable_alpha_blending = true)
				(sge::renderer::state::source_blend_func::src_alpha)
				(sge::renderer::state::dest_blend_func::inv_src_alpha);
	
	FCPPT_ASSERT(
		!state_scope_);
	state_scope_.reset(
		new sge::renderer::state::scoped(
			renderer_,
			is_transparent_ 
			? 
				transparent_list 
			: 
				nontransparent_list));
}

void 
insula::model_backend::end()
{
	// Deactivate everything in reverse order of activation
	state_scope_.reset();
	renderer_->unset_vertex_buffer(
		model_.vb());
	shader_.deactivate();
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

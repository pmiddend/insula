#include "backend.hpp"
#include "object.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/camera/object.hpp"
#include "../scene/render_pass/object.hpp"
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/source_blend_func.hpp>
#include <sge/renderer/state/dest_blend_func.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/assert.hpp>
#include <boost/foreach.hpp>

insula::model::backend::backend(
	scene::manager &manager,
	bool _has_transparency,
	sge::renderer::device_ptr _renderer,
	graphics::camera::object const &_camera,
	graphics::shader::object &_shader,
	graphics::shader::object &_shadow_shader,
	texture_map const &_textures,
	model::shared_object_ptr _model)
:
	scene::backend(
		manager,
		{"water","normal","shadow"}),
	has_transparency_(
		_has_transparency),
	renderer_(
		_renderer),
	camera_(
		_camera),
	shader_(
		_shader),
	shadow_shader_(
		_shadow_shader),
	textures_(
		_textures),
	model_(
		_model)
{
}

bool
insula::model::backend::has_transparency() const
{
	return has_transparency_;
}

void
insula::model::backend::begin(
	scene::render_pass::object const &rp)
{
	current_pass_ = rp.name;

	graphics::shader::object &this_shader = 
		rp.name == FCPPT_TEXT("shadow")
		?
			shadow_shader_
		:
			shader_;

	// Update the shader textures (before scoping it)
	BOOST_FOREACH(
		texture_map::const_reference r,
		textures_)
		this_shader.update_texture(
			r.first,
			r.second);

	this_shader.activate();

	renderer_->set_vertex_buffer(
		model_->vb());

	sge::renderer::state::list const 
		nontransparent_list = 
			sge::renderer::state::list
				// for now, cull both
				//(rp.name == FCPPT_TEXT("shadow") ? sge::renderer::state::cull_mode::back : sge::renderer::state::cull_mode::front)
				(sge::renderer::state::cull_mode::front)
				(sge::renderer::state::bool_::enable_alpha_blending = false)
				(sge::renderer::state::depth_func::less),
		transparent_list = 
			sge::renderer::state::list
				// for now, cull both
				(sge::renderer::state::cull_mode::off)
				(sge::renderer::state::depth_func::less)
				(sge::renderer::state::bool_::enable_alpha_blending = true)
				(sge::renderer::state::source_blend_func::src_alpha)
				(sge::renderer::state::dest_blend_func::inv_src_alpha);
	
	FCPPT_ASSERT(
		!state_scope_);
	state_scope_.reset(
		new sge::renderer::state::scoped(
			renderer_,
			has_transparency_ 
			? 
				transparent_list 
			: 
				nontransparent_list));
}

void 
insula::model::backend::end(
	scene::render_pass::object const &rp)
{
	// Deactivate everything in reverse order of activation
	state_scope_.reset();
	renderer_->unset_vertex_buffer(
		model_->vb());
	if (rp.name == FCPPT_TEXT("shadow"))
		shadow_shader_.deactivate();
	else
		shader_.deactivate();
}

insula::graphics::camera::object const &
insula::model::backend::camera() const
{
	return camera_;
}

/*
insula::graphics::shader::object &
insula::model::backend::shader() const
{
	return shader_;
}
*/
void
insula::model::backend::modelview(
	graphics::mat4 const &m)
{
	if (current_pass_ == FCPPT_TEXT("shadow"))
	{
		shadow_shader_.set_uniform(
			"mvp",
			camera().perspective() *
			m);
	}
	else
	{
		shader_.set_uniform(
			"mvp",
			camera().perspective() *
			m);
		shader_.set_uniform(
			"normal_matrix",
			fcppt::math::matrix::transpose(
				fcppt::math::matrix::inverse(
					m)));
	}
}

insula::model::object &
insula::model::backend::model() const
{
	return *model_;
}

insula::model::backend::~backend()
{
}

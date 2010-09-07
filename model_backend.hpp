#ifndef INSULA_MODEL_BACKEND_HPP_INCLUDED
#define INSULA_MODEL_BACKEND_HPP_INCLUDED

#include "scene/backend.hpp"
#include "graphics/shader/object_fwd.hpp"
#include "graphics/camera/object_fwd.hpp"
#include "graphics/shader/scoped.hpp"
#include "model/scoped.hpp"
#include <sge/renderer/glsl/string.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <unordered_map>
#include <memory>

namespace insula
{
class model_backend
:
	public scene::backend
{
public:
	typedef 
	std::unordered_map
	<
		sge::renderer::glsl::string,
		sge::renderer::texture_ptr
	>
	texture_map;

	explicit
	model_backend(
		sge::renderer::device_ptr,
		graphics::camera::object &,
		graphics::shader::object &,
		texture_map const &,
		model::object &);

	void
	begin();

	void 
	end();

	graphics::camera::object &
	camera() const;

	graphics::shader::object &
	shader() const;

	insula::model::object &
	model() const;
private:
	sge::renderer::device_ptr renderer_;
	graphics::camera::object &camera_;
	graphics::shader::object &shader_;
	texture_map textures_;
	model::object &model_;
	std::unique_ptr<model::scoped> model_scope_;
	std::unique_ptr<graphics::shader::scoped> shader_scope_;
};
}

#endif

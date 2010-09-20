#ifndef INSULA_MODEL_BACKEND_HPP_INCLUDED
#define INSULA_MODEL_BACKEND_HPP_INCLUDED

#include "../scene/backend.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "shared_object_ptr.hpp"
#include <sge/renderer/glsl/string.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <unordered_map>
#include <memory>

namespace insula
{
namespace model
{
class backend
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
	backend(
		bool has_transparency,
		sge::renderer::device_ptr,
		graphics::camera::object &,
		graphics::shader::object &,
		texture_map const &,
		model::shared_object_ptr);

	bool 
	has_transparency() const;

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

	~backend();
private:
	/// If this flag is true, we activate the neccessary states in the
	/// renderer in begin
	bool const has_transparency_;
	/// See has_transparency_ for the reason that's here
	sge::renderer::device_ptr renderer_;
	/// This is here so the model_instances can get the mvp matrix
	graphics::camera::object &camera_;
	graphics::shader::object &shader_;
	/// Usually we want the same shader for multiple backends, but with
	/// different textures
	texture_map textures_;
	shared_object_ptr model_;
	std::unique_ptr<sge::renderer::state::scoped> state_scope_;
};
}
}

#endif
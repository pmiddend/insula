#include "create_shader.hpp"
#include "../media_path.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/variable.hpp"
#include "../graphics/shader/variable_type.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/shader/sampler.hpp"
#include "vf/format.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/texture.hpp>
#include <fcppt/text.hpp>
#include <initializer_list>

insula::graphics::shader::shared_object_ptr const
insula::model::create_shader(
	sge::renderer::device_ptr const renderer,
	graphics::vec3 const &light_source)
{
	return 
		graphics::shader::shared_object_ptr(
			// make_shared has problems with the initializer_list
			new graphics::shader::object(
				renderer,
				media_path()/FCPPT_TEXT("model_vertex.glsl"),
				media_path()/FCPPT_TEXT("model_fragment.glsl"),
				graphics::shader::vf_to_string<vf::format>(),
				{
					graphics::shader::variable(
						"mvp",
						graphics::shader::variable_type::uniform,
						graphics::mat4()),
					/*
					graphics::shader::variable(
						"mv",
						graphics::shader::variable_type::uniform,
						graphics::mat4()),*/
					graphics::shader::variable(
						"normal_matrix",
						graphics::shader::variable_type::uniform,
						graphics::mat4()), 
					graphics::shader::variable(
						"light_source",
						graphics::shader::variable_type::const_,
						light_source) 
				},
				{
					graphics::shader::sampler(
						"texture",
						sge::renderer::texture_ptr())
				}));
}

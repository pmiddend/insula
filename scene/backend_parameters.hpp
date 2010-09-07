#ifndef INSULA_SCENE_BACKEND_PARAMETERS_HPP_INCLUDED
#define INSULA_SCENE_BACKEND_PARAMETERS_HPP_INCLUDED

namespace insula
{
namespace scene
{
class backend_parameters
{
public:
	explicit
	backend_parameters();

	backend_parameters &
	shader(
		graphics::shader::object &);

	backend_parameters &
	textures(
		texture_map);

	backend_parameters &
	vb(
		sge::renderer::vertex_buffer_ptr);
private:
	graphics::shader::object *shader_;
	texture_map textures_;
	sge::renderer::vertex_buffer_ptr vb_;
};
}
}

#endif

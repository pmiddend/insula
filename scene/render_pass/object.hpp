#ifndef INSULA_SCENE_RENDER_PASS_OBJECT_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_OBJECT_HPP_INCLUDED

#include "identifier.hpp"
#include "callbacks/camera.hpp"
#include "callbacks/viewport.hpp"
#include "callbacks/target.hpp"

namespace insula
{
namespace scene
{
namespace render_pass
{
class object
{
public:
	explicit
	object(
		identifier const &name,
		callbacks::camera const &camera,
		callbacks::viewport const &viewport,
		callbacks::target const &target)
	:
		name(name),
		camera(camera),
		viewport(viewport),
		target(target)
	{
	}
	
	identifier name;
	callbacks::camera camera;
	callbacks::viewport viewport;
	callbacks::target target;
};
}
}
}

#endif

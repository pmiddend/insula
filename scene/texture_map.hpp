#ifndef INSULA_SCENE_TEXTURE_MAP_HPP_INCLUDED
#define INSULA_SCENE_TEXTURE_MAP_HPP_INCLUDED


namespace insula
{
namespace scene
{
typedef
std::map
<
	sge::renderer::glsl::string,
	sge::renderer::texture_ptr
>
texture_map;
}
}

#endif

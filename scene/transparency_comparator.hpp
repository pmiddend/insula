#ifndef INSULA_SCENE_TRANSPARENCY_COMPARATOR_HPP_INCLUDED
#define INSULA_SCENE_TRANSPARENCY_COMPARATOR_HPP_INCLUDED

#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"
#include "transparent_instance.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace scene
{
// This is intentionally implemented inline. It should be fast.
class transparency_comparator
{
public:
	// This is needed by ptr_map
	typedef	
	bool
	result_type;

	explicit
	transparency_comparator(
		graphics::vec3 const &cam_pos)
	:
		cam_pos(cam_pos)
	{
	}

	// It's used with a std::set of pointers currently, so it gets
	// pointers
	result_type 
	operator()(
		transparent_instance const *a,
		transparent_instance const *b) const
	{
		return a->distance_to(cam_pos) > b->distance_to(cam_pos);
	}
private:
	graphics::vec3 const cam_pos;
};
}
}

#endif

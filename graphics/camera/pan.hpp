#ifndef INSULA_GRAPHICS_CAMERA_PAN_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_PAN_HPP_INCLUDED

#include "../scalar.hpp"
#include "../gizmo.hpp"
#include "../../time_delta.hpp"

namespace insula
{
namespace graphics
{
namespace camera
{
// This class manages a camera pan from one position + direction to
// another. The position is interpolated "smoothly" via:
//
// new_position = old_position + (target - old_position) / speed
// 
// The direction is slerped with 
//
// |current_position - target|
// ---------------------------
// |start_position - target|
// 
// As the running parameter (in [0,1])
//
// Note that this function is not dependant upon the camera. The user
// is responsible for delivering the current gizmo to the camera
// (which shouldn't be hard).
class pan
{
public:
	pan(pan const &) = delete;
	pan &operator=(pan const &) = delete;

	// Note that the speed argument really is an _inverse_ speed. See
	// the formula above for an explanation
	explicit 
	pan(
		gizmo const &start,
		gizmo const &end,
		scalar speed,
		scalar threshold);

	void
	update(
		time_delta);

	// This is true if the distance from the camera to the target is
	// almost zero.
	bool
	finished() const;

	gizmo const
	current() const;
private:
	gizmo start_,end_,current_;
	scalar const speed_;
	scalar const threshold_;
};
}
}
}

#endif

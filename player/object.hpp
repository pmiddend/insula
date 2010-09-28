#ifndef INSULA_PLAYER_OBJECT_HPP_INCLUDED
#define INSULA_PLAYER_OBJECT_HPP_INCLUDED

#include "../physics/character_controller.hpp"
#include "../physics/gizmo.hpp"
#include "parameters_fwd.hpp"

namespace insula
{
namespace player
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit
	object(
		parameters const &);

	physics::gizmo const
	gizmo() const;
private:
	physics::character_controller character_controller_;
};
}
}

#endif

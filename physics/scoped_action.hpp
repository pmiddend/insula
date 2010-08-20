#ifndef INSULA_PHYSICS_SCOPED_ACTION_HPP_INCLUDED
#define INSULA_PHYSICS_SCOPED_ACTION_HPP_INCLUDED

#include "world_fwd.hpp"

class btActionInterface;

namespace insula
{
namespace physics
{
class scoped_action
{
public:
	scoped_action(scoped_action const &) = delete;
	scoped_action &operator=(scoped_action const &) = delete;

	explicit
	scoped_action(
		world &);

	void
	set(
		btActionInterface &);

	~scoped_action();
private:
	world &w;
	btActionInterface *a;
};
}
}

#endif

#ifndef INSULA_PHYSICS_SCOPED_CONSTRAINT_HPP_INCLUDED
#define INSULA_PHYSICS_SCOPED_CONSTRAINT_HPP_INCLUDED

#include "world_fwd.hpp"

class btTypedConstraint;

namespace insula
{
namespace physics
{
class scoped_constraint
{
public:
	scoped_constraint(scoped_constraint const &) = delete;
	scoped_constraint &operator=(scoped_constraint const &) = delete;

	explicit
	scoped_constraint(
		world &,
		btTypedConstraint &);

	~scoped_constraint();
private:
	world &world_;
	btTypedConstraint &constraint_;
};
}
}

#endif

#ifndef INSULA_PHYSICS_SCOPED_BODY_HPP_INCLUDED
#define INSULA_PHYSICS_SCOPED_BODY_HPP_INCLUDED

#include "world_fwd.hpp"

class btRigidBody;

namespace insula
{
namespace physics
{
class scoped_body
{
public:
	scoped_body(scoped_body const &) = delete;
	scoped_body &operator=(scoped_body const &) = delete;

	explicit
	scoped_body(
		world &,
		btRigidBody &);

	~scoped_body();
private:
	world &w;
	btRigidBody &b;
};
}
}

#endif

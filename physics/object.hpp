#ifndef INSULA_PHYSICS_OBJECT_HPP_INCLUDED
#define INSULA_PHYSICS_OBJECT_HPP_INCLUDED

#include "iteration.hpp"
#include "object_type.hpp"

namespace insula
{
namespace physics
{
class object
{
protected:
	explicit
	object(
		object_type::type);

public:
	virtual ~object();

	void
	last_seen(
		iteration const &);

	iteration 
	last_seen() const;

	object_type::type
	type() const;

private:
	// This flag is set by the frustum culling and it denotes in which
	// iteration the object was last seen. By comparing this to the
	// current iteration of the world, one can deduce if an object is
	// visible
	iteration last_seen_;
	object_type::type const type_;
};
}
}

#endif

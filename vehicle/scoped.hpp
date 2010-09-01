#ifndef INSULA_VEHICLE_SCOPED_HPP_INCLUDED
#define INSULA_VEHICLE_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"

namespace insula
{
namespace vehicle
{
class scoped
{
public:
	scoped(scoped const &) = delete;
	scoped &operator=(scoped const &) = delete;

	explicit
	scoped(
		object &);

	~scoped();
private:
	object &object_;
};
}
}

#endif

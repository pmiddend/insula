#ifndef INSULA_PHYSICS_APPROXIMATION_VARIANT_HPP_INCLUDED
#define INSULA_PHYSICS_APPROXIMATION_VARIANT_HPP_INCLUDED

#include "sphere.hpp"
#include "box.hpp"
#include <fcppt/variant/object.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace physics
{
namespace approximation
{
typedef
fcppt::variant::object
<
	boost::mpl::vector2
	<
		sphere,
		box
	>
>
variant;
}
}
}

#endif

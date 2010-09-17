#ifndef INSULA_PHYSICS_TRAMPOLINE_HPP_INCLUDED
#define INSULA_PHYSICS_TRAMPOLINE_HPP_INCLUDED

#include "object_fwd.hpp"
#include <functional>

namespace insula
{
namespace physics
{
// This could be abstracted in two ways:
// - call it binary_trampoline and pull it out of physics
// - make an arbitrary trampoline out of it
template<typename A,typename B>
class trampoline
{
public:
	typedef std::function<void (A &,B &)> function_type;
	typedef void result_type;

	function_type f;

	explicit
	trampoline(
		function_type const &f)
	:
		f(f)
	{
	}

	result_type
	operator()(
		object &a,
		object &b) const
	{
		return f(dynamic_cast<A &>(a),dynamic_cast<B &>(b));
	}
};
}
}

#endif

#ifndef INSULA_STDLIB_GRID_AVERAGE_CONVOLVE_HPP_INCLUDED
#define INSULA_STDLIB_GRID_AVERAGE_CONVOLVE_HPP_INCLUDED

#include "convolve_3x3_no_borders.hpp"
#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace stdlib
{
namespace grid
{
template<typename T>
fcppt::container::grid::object<T,2> const
average_convolve(
	fcppt::container::grid::object<T,2> const &a)
{
	T const t = 
		static_cast<T>(1)/static_cast<T>(9);
	return 
		convolve_3x3_no_borders(
			a,
			typename fcppt::math::matrix::static_<T,3,3>::type(
				t,t,t,
				t,t,t,
				t,t,t));
}
}
}
}

#endif

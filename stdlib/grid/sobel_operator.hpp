#ifndef INSULA_STDLIB_GRID_SOBEL_OPERATOR_HPP_INCLUDED
#define INSULA_STDLIB_GRID_SOBEL_OPERATOR_HPP_INCLUDED

#include "../transform.hpp"
#include "convolve_3x3_no_borders.hpp"
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/container/grid/object.hpp>
#include <cmath>

namespace insula
{
namespace stdlib
{
namespace grid
{
template<typename T>
fcppt::container::grid::object<T,2> const
sobel_operator(
	fcppt::container::grid::object<T,2> const &c)
{
	typedef
	fcppt::container::grid::object<T,2>
	grid;

	typedef typename
	fcppt::math::matrix::static_<T,3,3>::type
	matrix;

	matrix const sobel_horizontal(
		static_cast<T>(-1),static_cast<T>(0),static_cast<T>(1),
		static_cast<T>(-2),static_cast<T>(0),static_cast<T>(2),
		static_cast<T>(-1),static_cast<T>(0),static_cast<T>(1));

	return 
		insula::stdlib::transform<grid>(
			convolve_3x3_no_borders(
					c,
					sobel_horizontal),
			convolve_3x3_no_borders(
				c,
				transpose(
					sobel_horizontal)),
			grid(
				c.dimension()),
			[](T const &h,T const &v) 
			{ 
				return std::sqrt(h*h + v*v); 
			});
}
}
}
}

#endif

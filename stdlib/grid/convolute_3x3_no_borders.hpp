#ifndef INSULA_STDLIB_GRID_CONVOLUTE_3X3_NO_BORDERS_HPP_INCLUDED
#define INSULA_STDLIB_GRID_CONVOLUTE_3X3_NO_BORDERS_HPP_INCLUDED

#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace stdlib
{
namespace grid
{
namespace detail
{
template<typename T>
T
convolute_cell(
	fcppt::container::grid::object<T,2> const &a,
	// FIXME: Don't use static here
	typename fcppt::math::matrix::static_<T,3,3>::type const &c,
	typename fcppt::container::grid::object<T,2>::size_type const cx,
	typename fcppt::container::grid::object<T,2>::size_type const cy)
{
	typedef 
	fcppt::container::grid::object<T,2>
	grid;

	typedef typename
	fcppt::math::matrix::static_<T,3,3>::type
	matrix;

	typedef typename
	grid::size_type
	size_type;

	typedef typename
	matrix::size_type
	msize_type;

	T sum = 
		static_cast<T>(0);
	for (
		size_type x = cx; 
		x < static_cast<size_type>(cx+c.dimension()[0]); 
		++x)
		for (
			size_type y = cy; 
			y < static_cast<size_type>(cy+c.dimension()[1]); 
			++y)
		{
			sum = 
				static_cast<T>(
					sum + 
					c[static_cast<msize_type>(x-cx)][static_cast<msize_type>(y-cy)] * 
					a[typename grid::dim(x,y)]);
		}
	return sum;
}
}

template<typename T>
fcppt::container::grid::object<T,2> const
convolute_3x3_no_borders(
	fcppt::container::grid::object<T,2> const &a,
	// FIXME: Don't use static here
	typename fcppt::math::matrix::static_<T,3,3>::type const &c)
{
	typedef 
	fcppt::container::grid::object<T,2>
	grid;

	typedef typename
	grid::size_type
	size_type;

	grid result(
		a.dimension());

	size_type const 
		whalf = c.dimension().w()/2,
		hhalf = c.dimension().h()/2;

	for(
		size_type x = whalf;
		x < static_cast<size_type>(a.dimension().w()-whalf);
		++x)
	{
		for(
			size_type y = hhalf; 
			y < static_cast<size_type>(a.dimension().h()-hhalf); 
			++y)
		{
			size_type 
				start_x = 
					static_cast<size_type>(x-whalf),
				start_y =
					static_cast<size_type>(y-hhalf);
				
			result[typename grid::dim(x,y)] = 
				detail::convolute_cell(
					a,
					c,
					start_x,
					start_y);
		}
	}
	
	return result;
}
}
}
}

#endif

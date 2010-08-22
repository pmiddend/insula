#ifndef INSULA_DISPLAY_2D_GRID_HPP_INCLUDED
#define INSULA_DISPLAY_2D_GRID_HPP_INCLUDED

#include <fcppt/container/grid/object.hpp>
#include <fcppt/io/ostream.hpp>
#include <fcppt/text.hpp>
#include <ostream>

namespace insula
{
template<typename T>
void
display_2d_grid(
	fcppt::io::ostream &s,
	fcppt::container::grid::object<T,2> const &a)
{
	typedef 
	fcppt::container::grid::object<T,2>
	array_type;

	typedef typename
	fcppt::container::grid::object<T,2>::dim
	dim;

	typedef typename
	fcppt::container::grid::object<T,2>::size_type
	size_type;

	for (size_type r = 0; r < a.dimension().h(); ++r)
	{
		for (size_type c = 0; c < a.dimension().w(); ++c)
			s << a[dim(c,r)] << FCPPT_TEXT(", ");
		s << FCPPT_TEXT('\n');
	}
}
}

#endif

#include "../display_2d_grid.hpp"
#include <boost/test/unit_test.hpp>
#include <fcppt/io/ostringstream.hpp>
#include <fcppt/text.hpp>

BOOST_AUTO_TEST_CASE(display_2d_grid)
{
	fcppt::io::ostringstream s;

	typedef
	fcppt::container::grid::object<int,2>
	array;

	array a(array::dim(2,3));

	/*
	 * 1 2
	 * 3 4
	 * 5 6
	 */

	a[array::dim(0,0)] = 1;
	a[array::dim(1,0)] = 2;
	a[array::dim(0,1)] = 3;
	a[array::dim(1,1)] = 4;
	a[array::dim(0,2)] = 5;
	a[array::dim(1,2)] = 6;

	insula::display_2d_grid(
		s,
		a);

	BOOST_CHECK(
		s.str() == FCPPT_TEXT("1, 2, \n3, 4, \n5, 6, \n"));
}

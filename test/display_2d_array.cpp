#include "../display_2d_array.hpp"
#include <boost/test/unit_test.hpp>
#include <fcppt/io/ostringstream.hpp>
#include <fcppt/text.hpp>

BOOST_AUTO_TEST_CASE(display_2d_array)
{
	fcppt::io::ostringstream s;

	typedef
	boost::multi_array<int,2>
	array;

	array a(boost::extents[3][2]);

	/*
	 * 1 2
	 * 3 4
	 * 5 6
	 */

	a[0][0] = 1;
	a[0][1] = 2;
	a[1][0] = 3;
	a[1][1] = 4;
	a[2][0] = 5;
	a[2][1] = 6;

	insula::display_2d_array(
		s,
		a);

	BOOST_CHECK(
		s.str() == FCPPT_TEXT("1, 2, \n3, 4, \n5, 6, \n"));
}

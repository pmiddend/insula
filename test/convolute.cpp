#include "../height_map/convolute.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/compare.hpp>
#include <fcppt/io/cout.hpp>
#include <boost/test/unit_test.hpp>

using namespace insula::height_map;

BOOST_AUTO_TEST_CASE(convolute_matrix)
{
	/*
		1 2 3 4
		5 6 7 8
		9 1 2 3
		4 5 6 7
	 */
	array a(boost::extents[4][4]);

	a[0][0] = 1;
	a[0][1] = 5;
	a[0][2] = 9;
	a[0][3] = 4;
	a[1][0] = 2;
	a[1][1] = 6;
	a[1][2] = 1;
	a[1][3] = 5;
	a[2][0] = 3;
	a[2][1] = 7;
	a[2][2] = 2;
	a[2][3] = 6;
	a[3][0] = 4;
	a[3][1] = 8;
	a[3][2] = 3;
	a[3][3] = 7;

	scalar const s = static_cast<scalar>(1)/static_cast<scalar>(9);

	convolution_matrix conv_matrix(
		s,s,s,
		s,s,s,
		s,s,s);

	array const c = 
		convolute(
			a,
			conv_matrix);

	for (int i = 0; i < 16; ++i)
		fcppt::io::cout << *(c.data() + i) << ",";
	fcppt::io::cout << std::endl;

	// The border wasn't touched
	BOOST_CHECK(
		fcppt::math::compare(a[0][0], c[0][0]));
	BOOST_CHECK( 
		fcppt::math::compare(a[0][1], c[0][1]));
	BOOST_CHECK( 
		fcppt::math::compare(a[0][2], c[0][2])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[0][3], c[0][3])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[1][0], c[1][0])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[2][0], c[2][0])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[1][3], c[1][3])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[2][3], c[2][3])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[3][0], c[3][0])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[3][1], c[3][1])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[3][2], c[3][2])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[3][3], c[3][3]));

	BOOST_CHECK(
		fcppt::math::compare(c[1][1],static_cast<scalar>(4)));
	BOOST_CHECK(
		fcppt::math::compare(c[2][1],static_cast<scalar>(4)));
	BOOST_CHECK(
		fcppt::math::compare(c[1][2],static_cast<scalar>(5)));
	BOOST_CHECK(
		fcppt::math::compare(c[2][2],static_cast<scalar>(5)));
}

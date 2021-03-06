#include "../height_map/array.hpp"
#include "../stdlib/grid/convolve_3x3_no_borders.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/container/grid/object_impl.hpp>
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
	array a(array::dim(4,4));

	a[array::dim(0,0)] = 1;
	a[array::dim(0,1)] = 5;
	a[array::dim(0,2)] = 9;
	a[array::dim(0,3)] = 4;
	a[array::dim(1,0)] = 2;
	a[array::dim(1,1)] = 6;
	a[array::dim(1,2)] = 1;
	a[array::dim(1,3)] = 5;
	a[array::dim(2,0)] = 3;
	a[array::dim(2,1)] = 7;
	a[array::dim(2,2)] = 2;
	a[array::dim(2,3)] = 6;
	a[array::dim(3,0)] = 4;
	a[array::dim(3,1)] = 8;
	a[array::dim(3,2)] = 3;
	a[array::dim(3,3)] = 7;

	scalar const s = static_cast<scalar>(1)/static_cast<scalar>(9);

	fcppt::math::matrix::static_<scalar,3,3>::type conv_matrix(
		s,s,s,
		s,s,s,
		s,s,s);

	array const c = 
		insula::stdlib::grid::convolve_3x3_no_borders(
			a,
			conv_matrix);

	for (int i = 0; i < 16; ++i)
		fcppt::io::cout << (*(c.data() + i)) << ",";
	fcppt::io::cout << std::endl;

	// The border wasn't touched
	BOOST_CHECK(
		fcppt::math::compare(a[array::dim(0,0)], c[array::dim(0,0)]));
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(0,1)], c[array::dim(0,1)]));
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(0,2)], c[array::dim(0,2)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(0,3)], c[array::dim(0,3)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(1,0)], c[array::dim(1,0)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(2,0)], c[array::dim(2,0)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(1,3)], c[array::dim(1,3)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(2,3)], c[array::dim(2,3)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(3,0)], c[array::dim(3,0)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(3,1)], c[array::dim(3,1)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(3,2)], c[array::dim(3,2)])); 
	BOOST_CHECK( 
		fcppt::math::compare(a[array::dim(3,3)], c[array::dim(3,3)]));

	BOOST_CHECK(
		fcppt::math::compare(c[array::dim(1,1)],static_cast<scalar>(4)));
	BOOST_CHECK(
		fcppt::math::compare(c[array::dim(2,1)],static_cast<scalar>(4)));
	BOOST_CHECK(
		fcppt::math::compare(c[array::dim(1,2)],static_cast<scalar>(5)));
	BOOST_CHECK(
		fcppt::math::compare(c[array::dim(2,2)],static_cast<scalar>(5)));
}

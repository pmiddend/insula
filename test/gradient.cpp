#include "../height_map/generate_gradient_simple.hpp"
#include <boost/test/unit_test.hpp>
#include <fcppt/container/grid/object_impl.hpp>
#include <fcppt/math/compare.hpp>
#include <algorithm>

BOOST_AUTO_TEST_CASE(gradient)
{
	insula::height_map::array a(
		insula::height_map::array::dim(3,3));

	std::iota(
		a.begin(),
		a.end(),
		static_cast<insula::height_map::scalar>(
			1));
	
	insula::height_map::array const grad =
		insula::height_map::generate_gradient_simple(
			a);
	
	BOOST_CHECK(
		fcppt::math::compare(
			grad[insula::height_map::array::dim(1,1)],
			static_cast<insula::height_map::scalar>(
				4)));
}

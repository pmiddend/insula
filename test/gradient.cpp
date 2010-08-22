#include "../display_2d_array.hpp"
#include "../height_map/generate_gradient_simple.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/multi_array.hpp>
#include <fcppt/math/compare.hpp>
#include <algorithm>

BOOST_AUTO_TEST_CASE(gradient)
{
	insula::height_map::array a(
		boost::extents[3][3]);

	std::iota(
		a.data(),
		a.data() + a.num_elements(),
		static_cast<insula::height_map::scalar>(
			1));
	
	insula::height_map::array const grad =
		insula::height_map::generate_gradient_simple(
			a);
	
	BOOST_CHECK(
		fcppt::math::compare(
			grad[1][1],
			static_cast<insula::height_map::scalar>(
				4)));
}

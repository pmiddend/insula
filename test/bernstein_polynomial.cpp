#include "../textures/interpolators/bernstein_polynomial.hpp"
#include "../textures/weight.hpp"
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <boost/test/unit_test.hpp>
#include <numeric>
#include <cmath>

BOOST_AUTO_TEST_CASE(bernstein_polynomial)
{
	insula::textures::weight const bound = 0.001;

	insula::textures::interpolators::bernstein_polynomial bp(
		4);

	insula::textures::weight const 
		calculated[] = 
		{
			bp.at(0,0.0),
			bp.at(0,0.25),
			bp.at(0,0.5),
			bp.at(0,0.75),
			bp.at(0,1.0),
			bp.at(1,0.0),
			bp.at(1,0.25),
			bp.at(1,0.5),
			bp.at(1,0.75),
			bp.at(1,1.0),
			bp.at(2,0.0),
			bp.at(2,0.25),
			bp.at(2,0.5),
			bp.at(2,0.75),
			bp.at(2,1.0),
			bp.at(3,0.0),
			bp.at(3,0.25),
			bp.at(3,0.5),
			bp.at(3,0.75),
			bp.at(3,1.0)
		},
		comparison[] = 
		{
			1,0.421875,0.125,0.015625,0,
			0,0.421875,0.375,0.140625,0,
			0,0.140625,0.375,0.421875,0,
			0,0.015625,0.125,0.421875,1

		};

	for (std::size_t i = 0; i < 5*4; ++i)
	{
		fcppt::io::cout << "i=" << i << "\n";
		BOOST_CHECK(
			std::abs(
				calculated[i] - comparison[i]) < bound);
	}

	for (insula::textures::weight f = 0.0; f <= 1.0; f += 0.001)
	{
		auto testweights = 
			bp.calculate_weights(
				f,
				0.0).sequence();

		fcppt::io::cout 
			<< FCPPT_TEXT("f=") 
			<< f 
			<< FCPPT_TEXT(": ") 
			<< 
				std::accumulate(
					testweights.begin(),
					testweights.end(),
					0.0f) 
			<< FCPPT_TEXT("\n");

		BOOST_CHECK(
			std::abs(
				1.0 - 
				std::accumulate(
					testweights.begin(),
					testweights.end(),
					0.0)) < bound);
	}
}

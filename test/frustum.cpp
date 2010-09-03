#include "../graphics/scalar.hpp"
#include "../graphics/vec3.hpp"
#include "../math/basic_frustum.hpp"
#include "../math/extract_frustum.hpp"
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/io/cout.hpp>
#include <boost/test/unit_test.hpp>
#include <cmath>

namespace
{
insula::graphics::scalar
fovy_to_focal_length(
	insula::graphics::scalar const fovy,
	insula::graphics::scalar const aspect)
{
	return aspect/std::atan(static_cast<insula::graphics::scalar>(2)/fovy);
}
}

BOOST_AUTO_TEST_CASE(frustum)
{
	using namespace insula;

	using graphics::scalar;

	scalar const
		fovy = fcppt::math::deg_to_rad(static_cast<scalar>(90)),
		aspect = static_cast<scalar>(480.0/640.0),
		focal_length = fovy_to_focal_length(fovy,aspect),
		near = 1,
		far = 100;

	fcppt::io::cout << "Aspect is " << aspect << ", focal length is " << focal_length << "\n";

	math::basic_frustum<scalar> fr = 
		math::extract_frustum<scalar>(
			fcppt::math::matrix::perspective(
				//1.0f/aspect,
				1.0f,
				fovy,
				near,
				far) * 
			fcppt::math::matrix::translation(
				graphics::vec3(-1,0,0)));

	fcppt::io::cout 
		<< "Frustum planes are: \n"
		<< "Left: " << fr.left() << "\n"
		<< "Right: " << fr.right() << "\n"
		<< "Top: " << fr.top() << "\n"
		<< "Bottom: " << fr.bottom() << "\n"
		<< "Near: " << fr.near() << "\n"
		<< "Far: " << fr.far() << "\n";
}

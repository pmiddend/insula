#include "cli_factory.hpp"
#include "object.hpp"
#include "../../get_option.hpp"
#include <fcppt/math/deg_to_rad.hpp>

insula::graphics::camera::object_ptr const
insula::graphics::camera::cli_factory(
	boost::program_options::variables_map const &vm,
	input_delegator &input_delegator_,
	scalar const aspect,
	vec3 const &position)
{
	return 
		std::make_shared<object>(
			input_delegator_,
			aspect,
			fcppt::math::deg_to_rad(
				get_option<graphics::scalar>(vm,"camera-fov")),
			get_option<graphics::scalar>(vm,"camera-near"),
			get_option<graphics::scalar>(vm,"camera-far"),
			get_option<graphics::scalar>(vm,"camera-speed"),
			get_option<graphics::scalar>(vm,"camera-roll-speed"),
			position);
}

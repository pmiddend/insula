#include "cli_factory.hpp"
#include "object.hpp"
#include "input.hpp"
#include "../create_path.hpp"
#include "../get_option.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/text.hpp>

insula::vehicle::object_ptr const
insula::vehicle::cli_factory(
	boost::program_options::variables_map const &vm,
	physics::world &w,
	physics::vec3 const &position,
	sge::renderer::device_ptr const rend,
	sge::image::multi_loader &il,
	sge::model::loader_ptr const model_loader,
	graphics::shader &shader,
	graphics::camera::object &cam,
	input_delegator &_input_delegator,
	console::object &console,
	sge::audio::multi_loader &audio_loader,
	sge::audio::player_ptr const audio_player)
{
	return 
		std::make_shared<object>(
			create_path(
				get_option<fcppt::string>(vm,"vehicle-file"),
				FCPPT_TEXT("vehicles")),
			std::ref(
				w),
			position,
			rend,
			std::ref(
				il),
			model_loader,
			std::ref(
				shader),
			std::ref(
				cam),
			std::ref(
				_input_delegator),
			get_option<graphics::scalar>(vm,"vehicle-camera-distance"),
			fcppt::math::deg_to_rad( 
				get_option<graphics::scalar>(vm,"vehicle-camera-angle")),
			std::ref(
				console),
			std::ref(
				audio_loader),
			audio_player);
}

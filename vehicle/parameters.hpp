#ifndef INSULA_VEHICLE_PARAMETERS_HPP_INCLUDED
#define INSULA_VEHICLE_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/vehicle/wheel_info_sequence.hpp"
#include "../physics/scalar.hpp"
#include "../physics/vec3.hpp"
#include "../height_map/object_fwd.hpp"
#include "../input_delegator_fwd.hpp"
#include "../console/object_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include <sge/model/object_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace vehicle
{
class parameters
{
public:
	scene::manager &scene_manager;
	sge::model::object_ptr chassis_model,wheel_model;
	sge::renderer::device_ptr renderer;
	sge::image::multi_loader &image_loader;
	graphics::camera::object &camera;
	graphics::shader::object &model_shader;
	// Here, we store just a path instead of a
	// sge::renderer::texture_ptr because the vehicle should decide
	// which filter to apply to the texture
	fcppt::filesystem::path chassis_texture,wheel_texture;
	physics::world &physics_world;
	physics::scalar 
		mass,
		chassis_position,
		steering_clamp,
		max_engine_force,
		max_breaking_force,
		max_speed,
		track_connection;
	physics::vec3 position;
	physics::vehicle::wheel_info_sequence wheel_infos;
	input_delegator &input_delegator_;
	console::object &console;
	graphics::scalar camera_distance,camera_angle;
	sge::audio::player_ptr audio_player;
	sge::audio::buffer_ptr engine_buffer;
	sge::audio::buffer_ptr skid_buffer;
	height_map::object &height_map;

	explicit 
	parameters(
		scene::manager &scene_manager,
		sge::model::object_ptr chassis_model,
		sge::model::object_ptr wheel_model,
		sge::renderer::device_ptr renderer,
		sge::image::multi_loader &image_loader,
		graphics::camera::object &camera,
		graphics::shader::object &model_shader,
		fcppt::filesystem::path chassis_texture,
		fcppt::filesystem::path wheel_texture,
		physics::world &physics_world,
		physics::scalar mass,
		physics::scalar chassis_position,
		physics::scalar steering_clamp,
		physics::scalar max_engine_force,
		physics::scalar max_breaking_force,
		physics::scalar max_speed,
		physics::scalar track_connection,
		physics::vec3 position,
		physics::vehicle::wheel_info_sequence wheel_infos,
		input_delegator &input_delegator_,
		console::object &console,
		graphics::scalar camera_distance,
		graphics::scalar camera_angle,
		sge::audio::player_ptr audio_player,
		sge::audio::buffer_ptr engine_buffer,
		sge::audio::buffer_ptr skid_buffer,
		height_map::object &height_map)
	:
		scene_manager(scene_manager),
		chassis_model(chassis_model),
		wheel_model(wheel_model),
		renderer(renderer),
		image_loader(image_loader),
		camera(camera),
		model_shader(model_shader),
		chassis_texture(chassis_texture),
		wheel_texture(wheel_texture),
		physics_world(physics_world),
		mass(mass),
		chassis_position(chassis_position),
		steering_clamp(steering_clamp),
		max_engine_force(max_engine_force),
		max_breaking_force(max_breaking_force),
		max_speed(max_speed),
		track_connection(track_connection),
		position(position),
		wheel_infos(wheel_infos),
		input_delegator_(input_delegator_),
		console(console),
		camera_distance(camera_distance),
		camera_angle(camera_angle),
		audio_player(audio_player),
		engine_buffer(engine_buffer),
		skid_buffer(skid_buffer),
		height_map(height_map)
	{
	}
};
}
}

#endif

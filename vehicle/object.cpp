#include "object.hpp"
#include "input.hpp"
#include "speed_to_pitch.hpp"
#include "../create_path.hpp"
#include "../console/object.hpp"
#include "../gizmo/structure_cast.hpp"
#include "../gizmo/lock_to.hpp"
#include "../physics/json/parse_vehicle.hpp"
#include "../physics/gizmo.hpp"
#include "../physics/vehicle/object.hpp"
#include "../graphics/camera/object.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/console/object.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/listener.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/exception.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <cmath>

insula::vehicle::object::object(
	fcppt::filesystem::path const &json_file,
	physics::world &physics_world,
	physics::vec3 const &position,
	sge::renderer::device_ptr const rend,
	sge::image::multi_loader &il,
	sge::model::loader_ptr const model_loader,
	graphics::shader &shader,
	graphics::camera::object &_cam,
	input_delegator &_input_delegator,
	graphics::scalar const _camera_distance,
	graphics::scalar const _camera_angle,
	console::object &_console,
	sge::audio::multi_loader &_audio_loader,
	sge::audio::player_ptr const _audio_player)
:
	cam_(
		_cam),
	lock_camera_(
		true),
	toggle_camera_lock_(
		_console.model().insert(
			FCPPT_TEXT("lock_camera"),
			[&lock_camera_](
				sge::console::arg_list const &,
				sge::console::object &)
			{
				lock_camera_ = !lock_camera_;
			},
			FCPPT_TEXT("Toggle camera lock to vehicle on/off"))),
	camera_distance_(
		_camera_distance),
	camera_angle_(
		_camera_angle),
	audio_player_(
		_audio_player)
{
	sge::parse::json::object json_object;
	if (!sge::parse::json::parse_file(json_file,json_object))
		throw sge::exception(FCPPT_TEXT("Error parsing file: ")+json_file.string());

	physics_ = 
		physics::json::parse_vehicle(
			json_object,
			physics_world,
			position,
			rend,
			il,
			model_loader,
			shader,
			_cam);

	input_.reset(
		new input(
			_input_delegator,
			*physics_));

	engine_buffer_ = 
		audio_player_->create_buffer(
			_audio_loader.load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						json_object.members,
						FCPPT_TEXT("engine_sound")),
				FCPPT_TEXT("sounds"))));

	engine_source_ = 
		engine_buffer_->create_positional(
			sge::audio::sound::positional_parameters()
				.position(
					fcppt::math::vector::structure_cast<sge::audio::vector>(
						position)));

	engine_source_->play(
		sge::audio::sound::repeat::loop);
}

void
insula::vehicle::object::update_camera()
{
	if (!lock_camera_)
		return;

	cam_.gizmo() = 
		gizmo::lock_to(
			gizmo::structure_cast<physics::gizmo>(
				physics_->gizmo()),
			camera_distance_,
			camera_angle_);
}

void
insula::vehicle::object::update()
{
	physics_->update();

	engine_source_->position(
		fcppt::math::vector::structure_cast<sge::audio::vector>(
			physics_->gizmo().position()));

	audio_player_->listener().position(
		fcppt::math::vector::structure_cast<sge::audio::vector>(
			//cam_.gizmo().position()));
			physics_->gizmo().position()));

	engine_source_->pitch(
		static_cast<sge::audio::scalar>(1) +
		speed_to_pitch(
			std::abs(
				physics_->speed_kmh())));
}

void
insula::vehicle::object::render()
{
	physics_->render();
}

insula::physics::scalar
insula::vehicle::object::speed_kmh() const
{
	return 
		std::abs(
			physics_->speed_kmh());
}

#include "parameters.hpp"
#include "object.hpp"
#include "input.hpp"
#include "speed_to_pitch.hpp"
#include "../scene/manager.hpp"
#include "../model/object.hpp"
#include "../console/object.hpp"
#include "../graphics/mat4.hpp"
#include "../gizmo/structure_cast.hpp"
#include "../gizmo/lock_to.hpp"
#include "../physics/shape_from_model.hpp"
#include "../physics/gizmo.hpp"
#include "../physics/vehicle/object.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/shader/scoped.hpp"
#include <sge/console/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/image/file.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/listener.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/exception.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <cmath>

insula::vehicle::object::object(
	parameters const &params)
:
	chassis_backend_(
		// no transparency
		false,
		params.renderer,
		params.camera,
		params.model_shader,
		fcppt::assign::make_container<model::backend::texture_map>
			(model::backend::texture_map::value_type(
				"texture",
				params.renderer->create_texture(
					params.image_loader.load(params.chassis_texture)->view(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none))),
		model::shared_object_ptr(
			new model::object(
				params.chassis_model,
				params.renderer))),
	chassis_instance_(
		fcppt::math::matrix::translation(
			fcppt::math::vector::structure_cast<graphics::vec3>(
				params.position))),
	wheel_backend_(
		// no transparency
		false,
		params.renderer,
		params.camera,
		params.model_shader,
		fcppt::assign::make_container<model::backend::texture_map>
			(model::backend::texture_map::value_type(
				"texture",
				params.renderer->create_texture(
					params.image_loader.load(params.wheel_texture)->view(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none))),
		model::shared_object_ptr(
			new model::object(
				params.wheel_model,
				params.renderer))),
	physics_(
		params.physics_world,
		physics::shape_from_model(
			chassis_backend_.model(),
			physics::approximation::numeric_value::box),
		params.mass,
		params.chassis_position,
		params.steering_clamp,
		params.position,
		params.max_engine_force,
		params.max_breaking_force,
		params.max_speed,
		params.track_connection,
		fcppt::math::box::structure_cast<physics::box>(
			wheel_backend_.model().bounding_box()),
		params.wheel_infos),
	input_(
		params.input_delegator_,
		physics_),
	camera_(
		params.camera),
	lock_camera_(
		false),
	toggle_camera_lock_(
		params.console.model().insert(
			FCPPT_TEXT("lock_camera"),
			[&lock_camera_](
				sge::console::arg_list const &,
				sge::console::object &)
			{
				lock_camera_ = !lock_camera_;
			},
			FCPPT_TEXT("Toggle camera lock to vehicle on/off"))),
	camera_distance_(
		params.camera_distance),
	camera_angle_(
		params.camera_angle),
	audio_player_(
		params.audio_player),
	engine_buffer_(
		params.engine_buffer),
	engine_source_(
		engine_buffer_->create_positional(
			sge::audio::sound::positional_parameters()
				.position(
					fcppt::math::vector::structure_cast<sge::audio::vector>(
						params.position)))),
	skid_buffer_(
		params.skid_buffer),
	skid_source_(
		skid_buffer_->create_positional(
			sge::audio::sound::positional_parameters()
				.position(
					fcppt::math::vector::structure_cast<sge::audio::vector>(
						params.position)))),
	active_(false)
{
	params.scene_manager.insert(
		chassis_backend_,
		chassis_instance_);

	BOOST_FOREACH(
			physics::mat4_sequence::const_reference r,
			physics_.wheel_transforms())
	{
		wheel_instances_.push_back(
			new model::instance(
				fcppt::math::matrix::structure_cast<graphics::mat4>(
					r)));
		params.scene_manager.insert(
			wheel_backend_,
			wheel_instances_.back());
	}
}

insula::graphics::gizmo const
insula::vehicle::object::lock_to_gizmo() const
{
	return 
		gizmo::lock_to(
			gizmo::structure_cast<physics::gizmo>(
				physics_.gizmo()),
			camera_distance_,
			camera_angle_);
}

void
insula::vehicle::object::update_camera()
{
	if (!lock_camera_)
		return;

	camera_.gizmo() = 
		lock_to_gizmo();
}

void
insula::vehicle::object::update()
{
	physics_.update();
	
	chassis_instance_.transformation(
		physics_.chassis_transform());

	// I don't want to run through the containers with two iterators and
	// since wheel_instance_array is already an array, I'll iterate by
	// index.
	wheel_instance_array::size_type i = 0;
	BOOST_FOREACH(
			physics::mat4_sequence::const_reference r,
			physics_.wheel_transforms())
		wheel_instances_[i++].transformation(
			fcppt::math::matrix::structure_cast<graphics::mat4>(
				r));
	

	// Sound stuff
	if (!active_)
		return;

	engine_source_->position(
		fcppt::math::vector::structure_cast<sge::audio::vector>(
			physics_.gizmo().position()));

	skid_source_->position(
		fcppt::math::vector::structure_cast<sge::audio::vector>(
			physics_.gizmo().position()));

	if (physics_.is_skidding())
	{
		if (skid_source_->status() != sge::audio::sound::play_status::playing)
			skid_source_->play(
				sge::audio::sound::repeat::loop);
	}
	else
		skid_source_->stop();

	audio_player_->listener().position(
		fcppt::math::vector::structure_cast<sge::audio::vector>(
			physics_.gizmo().position()));

	engine_source_->pitch(
		static_cast<sge::audio::scalar>(1) +
		speed_to_pitch(
			std::abs(
				physics_.speed_kmh())));
}

insula::physics::scalar
insula::vehicle::object::speed_kmh() const
{
	return 
		std::abs(
			physics_.speed_kmh());
}

void
insula::vehicle::object::activate()
{
	active_ = true;
	engine_source_->play(
		sge::audio::sound::repeat::loop);
	input_.is_active(true);
	lock_camera_ = true;
}

void
insula::vehicle::object::deactivate()
{
	active_ = false;
	engine_source_->stop();
	input_.is_active(false);
	lock_camera_ = false;
}

insula::vehicle::object::~object() {}

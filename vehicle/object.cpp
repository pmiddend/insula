#include "parameters.hpp"
#include "object.hpp"
#include "input.hpp"
#include "speed_to_pitch.hpp"
#include "../model/scoped.hpp"
#include "../console/object.hpp"
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
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <cmath>

insula::vehicle::object::object(
	parameters const &params)
:
	renderer_(
		params.renderer),
	chassis_model_(
		params.chassis_model,
		params.renderer),
	wheel_model_(
		params.wheel_model,
		params.renderer),
	model_shader_(
		params.model_shader),
	chassis_texture_(
		params.renderer->create_texture(
			params.image_loader.load(params.chassis_texture)->view(),
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none)),
	wheel_texture_(
		params.renderer->create_texture(
			params.image_loader.load(params.wheel_texture)->view(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	physics_(
		params.physics_world,
		physics::shape_from_model(
			chassis_model_,
			physics::model_approximation(
				physics::model_approximation::box,
				static_cast<physics::scalar>(1))),
		params.mass,
		params.chassis_position,
		params.steering_clamp,
		params.position,
		params.max_engine_force,
		params.max_breaking_force,
		params.max_speed,
		fcppt::math::box::structure_cast<physics::box>(
			wheel_model_.bounding_box()),
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
						params.position))))
{
	// Update once, so the wheel transformations are correct. That's
	// needed in pregame when the physics loop isn't active yet.
	update();
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
			//camera_.gizmo().position()));
			physics_.gizmo().position()));

	engine_source_->pitch(
		static_cast<sge::audio::scalar>(1) +
		speed_to_pitch(
			std::abs(
				physics_.speed_kmh())));
}

void
insula::vehicle::object::render()
{
	graphics::mat4 const mvp = 
		camera_.perspective() * 
		camera_.world();

	{
		model::scoped scoped_model(
			renderer_,
			chassis_model_);

		// FIRST update the texture, THEN scope the shader!
		model_shader_.update_texture(
			"texture",
			chassis_texture_);

		graphics::shader::scoped scoped_shader(
			model_shader_);

		model_shader_.set_uniform(
			"mvp",
			mvp * 
			physics_.chassis_transform());

		chassis_model_.render();
	}

	{
		model::scoped scoped_model(
			renderer_,
			wheel_model_);

		// FIRST update the texture, THEN scope the shader!
		model_shader_.update_texture(
			"texture",
			wheel_texture_);

		graphics::shader::scoped scoped_shader(
			model_shader_);

		BOOST_FOREACH(
			physics::mat4_sequence::const_reference r,
			physics_.wheel_transforms())
		{
			model_shader_.set_uniform(
				"mvp",
				mvp * 
				r); 
			wheel_model_.render();
		}
	}
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
	engine_source_->play(
		sge::audio::sound::repeat::loop);
	input_.is_active(true);
	lock_camera_ = true;
}

void
insula::vehicle::object::deactivate()
{
	engine_source_->stop();
	input_.is_active(false);
	lock_camera_ = false;
}

insula::vehicle::object::~object() {}

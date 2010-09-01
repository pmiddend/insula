#ifndef INSULA_VEHICLE_OBJECT_HPP_INCLUDED
#define INSULA_VEHICLE_OBJECT_HPP_INCLUDED

#include "input.hpp"
#include "parameters_fwd.hpp"
#include "../model/object.hpp"
#include "../physics/vehicle/object.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/vec3.hpp"
#include "../console/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/gizmo.hpp"
#include "../input_delegator.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/sound/positional_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <memory>

namespace insula
{
namespace vehicle
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit 
	object(
		parameters const &);

	// The camera_move state needs this to determine the "target" gizmo,
	// and also the update_camera function
	graphics::gizmo const 
	lock_to_gizmo() const;

	void
	update_camera();

	void
	update();

	void
	render();

	physics::scalar 
	speed_kmh() const;

	void
	activate();

	void
	deactivate();

	~object();
private:
	sge::renderer::device_ptr renderer_;
	model::object chassis_model_,wheel_model_;
	graphics::shader::object &model_shader_;
	sge::renderer::texture_ptr chassis_texture_,wheel_texture_;
	physics::vehicle::object physics_;
	input input_;
	graphics::camera::object &camera_;
	bool lock_camera_;
	fcppt::signal::scoped_connection toggle_camera_lock_;
	graphics::scalar camera_distance_,camera_angle_;
	sge::audio::player_ptr audio_player_;
	sge::audio::buffer_ptr engine_buffer_;
	sge::audio::sound::positional_ptr engine_source_;
	sge::audio::buffer_ptr skid_buffer_;
	sge::audio::sound::positional_ptr skid_source_;
};
}
}

#endif

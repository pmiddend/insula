#ifndef INSULA_VEHICLE_OBJECT_HPP_INCLUDED
#define INSULA_VEHICLE_OBJECT_HPP_INCLUDED

#include "input.hpp"
#include "parameters_fwd.hpp"
#include "../model/backend.hpp"
#include "../model/instance.hpp"
#include "../physics/vehicle/object.hpp"
#include "../physics/world_fwd.hpp"
#include "../console/object_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/gizmo.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/sound/positional_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <memory>

namespace insula
{
namespace vehicle
{
class object
{
/**
	This class unifies the components needed for a single vehicle:
	- A vehicle::input object which receives input events (from
    keyboard/mouse/gamepad) and translates those to steering, braking
    etc. stuff (functions from physics::vehicle::object
	- Model backends and instances for the wheels and the chassis
	- Sounds (positional engine sounds, pitch changes)
	- Camera stuff

	A vehicle can be active or inactive. An active vehicle will play the
	engine sound (as well as re-pitch it) and lock the camera (the
	update_camera() call will reset the global camera's gizmo)

	TODO: Can't the update_call conditionally reset the camera gizmo?
	*/
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

	physics::scalar 
	speed_kmh() const;

	void
	activate();

	void
	deactivate();

	~object();
private:
	typedef 
	boost::ptr_vector<model::instance> 
	wheel_instance_array;

	model::backend chassis_backend_;
	model::instance chassis_instance_;
	model::backend wheel_backend_;
	wheel_instance_array wheel_instances_;
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
	bool active_;
};
}
}

#endif

#ifndef INSULA_VEHICLE_OBJECT_HPP_INCLUDED
#define INSULA_VEHICLE_OBJECT_HPP_INCLUDED

#include "../physics/vehicle/object_ptr.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/vec3.hpp"
#include "../console/object_fwd.hpp"
#include "../graphics/shader_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/gizmo.hpp"
#include "input_fwd.hpp"
#include "../input_delegator.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/audio/multi_loader_fwd.hpp>
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
		fcppt::filesystem::path const &json_file,
		physics::world &w,
		physics::vec3 const &position,
		sge::renderer::device_ptr const rend,
		sge::image::multi_loader &il,
		sge::model::loader_ptr const model_loader,
		graphics::shader &shader,
		graphics::camera::object &cam,
		input_delegator &,
		graphics::scalar camera_distance,
		graphics::scalar camera_angle,
		console::object &,
		sge::audio::multi_loader &,
		sge::audio::player_ptr);

	// The camera_move state needs this
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
private:
	physics::vehicle::object_ptr physics_;
	std::unique_ptr<input> input_;
	graphics::camera::object &cam_;
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

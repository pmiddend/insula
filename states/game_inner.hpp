#ifndef INSULA_STATES_GAME_INNER_HPP_INCLUDED
#define INSULA_STATES_GAME_INNER_HPP_INCLUDED

#include "game_outer.hpp"
#include "freelook_fwd.hpp"
#include "../player.hpp"
#include "../turn_timer.hpp"
#include "../physics/world.hpp"
#include "../physics/height_map.hpp"
#include "../physics/debug_drawer.hpp"
#include "../physics/static_model.hpp"
#include "../graphics/shader.hpp"
#include "../model/object.hpp"
#include "../events/tick_fwd.hpp"
#include "../events/render_fwd.hpp"
#include "../vehicle/object_ptr.hpp"
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace insula
{
namespace states
{
class game_inner
:
	public boost::statechart::state<game_inner,game_outer,freelook>
{
public:
	explicit
	game_inner(
		my_context);

	void
	react(
		events::tick const &);

	void
	react(
		events::render const &);

	insula::vehicle::object &
	vehicle();
	
	player const &
	current_player() const;

	insula::turn_timer &
	turn_timer();

	insula::turn_timer const &
	turn_timer() const;

	physics::world &
	physics_world();

	void
	erase_nugget(
		physics::static_model &);

	// FIXME: is size_t ok here?
	std::size_t 	
	nugget_count() const;

	~game_inner();
private:
	// These are the actual physics models (which will be deleted
	// once the player touches them). They are kept here because they
	// are needed from freelook on. That's why some of the game control
	// logic like deleting this models is handled in game_inner, too
	// (though it might be better placed in running)
	typedef
	boost::ptr_vector<physics::static_model>
	nugget_model_sequence;

	typedef
	std::set<physics::static_model*>
	deletion_set;

	player current_player_;
	insula::turn_timer turn_timer_;
	physics::world physics_world_;
	physics::height_map physics_height_map_;
	physics::debug_drawer physics_debug_drawer_;
	bool physics_debug_;
	fcppt::signal::scoped_connection toggle_physics_debug_; 
	graphics::shader_old nugget_shader_;
	model::object nugget_model_;
	nugget_model_sequence nugget_models_;
	insula::vehicle::object_ptr vehicle_;
	fcppt::signal::scoped_connection vehicle_static_connection_;
	deletion_set to_delete_;

	void
	vehicle_static_callback(
		physics::vehicle::object &,
		physics::static_model &);
};
}
}

#endif

#ifndef INSULA_STATES_GAME_INNER_HPP_INCLUDED
#define INSULA_STATES_GAME_INNER_HPP_INCLUDED

#include "game_outer.hpp"
#include "freelook_fwd.hpp"
#include "../physics/world.hpp"
#include "../physics/height_map.hpp"
#include "../physics/debug_drawer.hpp"
#include "../physics/static_model.hpp"
#include "../graphics/shader.hpp"
#include "../model/object.hpp"
#include "../events/tick_fwd.hpp"
#include "../events/render_fwd.hpp"
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
	
	~game_inner();
private:
	// These are the actual physics models (which will be deleted
	// once the player touches them)
	typedef
	boost::ptr_vector<physics::static_model>
	nugget_model_sequence;

	physics::world physics_world_;
	physics::height_map physics_height_map_;
	physics::debug_drawer physics_debug_drawer_;
	bool physics_debug_;
	fcppt::signal::scoped_connection toggle_physics_debug_; 
	graphics::shader nugget_shader_;
	model::object nugget_model_;
	nugget_model_sequence nugget_models_;
};
}
}

#endif
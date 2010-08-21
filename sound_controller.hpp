#ifndef INSULA_SOUND_CONTROLLER_HPP_INCLUDED
#define INSULA_SOUND_CONTROLLER_HPP_INCLUDED

#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/pool.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <fcppt/string.hpp>
#include <map>

namespace insula
{
/// The sound_controller is very similar to the
/// music_controller. Sounds shouldn't be hard-coded, so we store a map 
///
/// sound_name -> filename 
/// 
/// In a json file and convert it to a map
///
/// sound_name -> buffer_ptr 
/// 
/// In this controller. We use soudn buffers, so the sounds are
/// non-streaming. This class also includes the sound pool, which
/// solves the problem that a sound stops playing when the destructor is
/// called (we want the sound to be destroyed when the destructor is
/// called _and_ it's finished playing. Use case: a character dies).
///
/// Note that the sound controller wasn't designed for engine sounds
/// since those are defined per vehicle; also they're looping and
/// should end when the vehicle dies, so putting them in the pool
/// isn't really appropriate
class sound_controller
{
public:
	sound_controller(sound_controller const &) = delete;
	sound_controller &operator=(sound_controller const &) = delete;

	explicit 
	sound_controller(
		// This is just the "sounds" section of some global config file,
		// or it might a separate file
		sge::parse::json::object const &,
		sge::audio::multi_loader &,
		sge::audio::player_ptr);

	void
	play(
		fcppt::string const &);

	void
	play_positional(
		fcppt::string const &,
		sge::audio::sound::positional_parameters const &);

	// The pool has update which needs to be called so the sounds are
	// deleted properly
	void
	update();

	~sound_controller();
private:
	typedef
	std::map
	<
		fcppt::string,
		sge::audio::buffer_ptr
	>
	audio_map;

	sge::audio::player_ptr player_;
	audio_map sounds_;
	sge::audio::pool pool_;
};
}

#endif

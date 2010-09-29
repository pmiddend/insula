#ifndef INSULA_MUSIC_CONTROLLER_HPP_INCLUDED
#define INSULA_MUSIC_CONTROLLER_HPP_INCLUDED

#include "random_engine.hpp"
#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/file_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/sound/base_ptr.hpp>
#include <sge/parse/json/object_fwd.hpp>
// Why do we need this?
#include <sge/parse/json/array.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/string.hpp>
#include <random>
#include <vector>
#include <map>

namespace insula
{
class music_controller
{
public:
	music_controller(music_controller const &) = delete;
	music_controller &operator=(music_controller const &) = delete;

	explicit
	music_controller(
		sge::parse::json::object const &,
		sge::audio::multi_loader &,
		sge::audio::player_ptr);

	void
	update();

	void
	play_event(
		fcppt::string const &);

	void
	play_random();

	void
	stop();

	~music_controller();
private:
	typedef
	std::map
	<
		fcppt::string,
		sge::audio::file_ptr
	>
	file_map;

	typedef
	std::vector<sge::audio::file_ptr>
	file_set;

	sge::audio::scalar volume_;
	file_map event_sounds_;
	file_set random_sounds_;
	sge::time::timer crossfade_;

	sge::audio::player_ptr player_;
	sge::audio::buffer_ptr silence_buffer_;
	sge::audio::sound::base_ptr silence_source_;

	std::uniform_int_distribution<file_set::size_type> rng_;
	random_engine rng_engine_;

	sge::audio::sound::base_ptr current_source_,new_source_;

	void
	do_play(
		sge::audio::sound::base_ptr);
};
}

#endif

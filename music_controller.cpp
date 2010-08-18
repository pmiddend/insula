#include "music_controller.hpp"
#include "create_path.hpp"
#include "media_path.hpp"
#include "stdlib/map.hpp"
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/time/second.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/text.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <iterator>

insula::music_controller::music_controller(
	sge::parse::json::object const &o,
	sge::audio::multi_loader &ml,
	sge::audio::player_ptr const _player)
:
	event_sounds_(
		stdlib::map<file_map>(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				o.members,
				FCPPT_TEXT("events")).members,
			[&ml](sge::parse::json::member const &v)
			{
				return 
					file_map::value_type(
						v.name,
						ml.load(
							create_path(
								sge::parse::json::get<sge::parse::json::string>(
									v.value_),
								FCPPT_TEXT("sounds/music"))));
			})),
	random_sounds_(
		stdlib::map<file_set>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				o.members,
				FCPPT_TEXT("random")).elements,
			[&ml](sge::parse::json::value const &v)
			{
				return 
					ml.load(
						create_path(
							sge::parse::json::get<sge::parse::json::string>(
								v),
							FCPPT_TEXT("sounds/music")));
			})),
	crossfade_(
		sge::time::second(
			static_cast<sge::time::unit>(
				sge::parse::json::find_member_exn<sge::parse::json::int_type>(
						o.members,
						FCPPT_TEXT("crossfade_seconds"))))),
	player_(
		_player),
	silence_buffer_(
		player_->create_buffer(
			ml.load(
				media_path()/FCPPT_TEXT("sounds")/FCPPT_TEXT("silence.wav")))),
	silence_source_(
		silence_buffer_->create_nonpositional()),
	rng_(
		static_cast<file_set::size_type>(0),
		static_cast<file_set::size_type>(random_sounds_.size()-1)),
	rng_engine_(),
	current_source_(
		silence_source_)
{
}

void
insula::music_controller::update()
{
	if (new_source_)
	{
		if (crossfade_.expired())
		{
			FCPPT_ASSERT(
				current_source_);
			current_source_->stop();
			current_source_ = new_source_;
			new_source_.reset();
		}
		else
		{
			current_source_->gain(
				static_cast<sge::audio::scalar>(1) - 
				static_cast<sge::audio::scalar>(
					crossfade_.elapsed_frames()));
			new_source_->gain(
				static_cast<sge::audio::scalar>(
					crossfade_.elapsed_frames()));

			new_source_->update();
		}
	}

	if (current_source_)
		current_source_->update();
}

void
insula::music_controller::play_event(
	fcppt::string const &e)
{
	if (event_sounds_.find(e) == event_sounds_.end())
		throw fcppt::exception(FCPPT_TEXT("Event \"")+e+FCPPT_TEXT("\" not found"));

	do_play(
		player_->create_nonpositional_stream(
			event_sounds_[e]));
}

void
insula::music_controller::play_random()
{
	if (random_sounds_.empty())
		return;

	file_set::size_type const n = 
		rng_(
			rng_engine_);
		
	fcppt::io::cout << "Choosing the " << n << "th sound\n";
	
	do_play(
		player_->create_nonpositional_stream(
			*std::next(
				random_sounds_.begin(),
				n)));
}

void
insula::music_controller::stop()
{
	do_play(
		silence_source_);
}

void
insula::music_controller::do_play(
	sge::audio::sound::base_ptr b)
{
	b->gain(
		static_cast<sge::audio::scalar>(0));
	b->play(
		sge::audio::sound::repeat::once);
	crossfade_.reset();

	new_source_ = b;
}

#include "sound_controller.hpp"
#include "exception.hpp"
#include "create_path.hpp"
#include "stdlib/map.hpp"
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/member.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/stop_mode.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/sound/positional.hpp>
#include <fcppt/text.hpp>

insula::sound_controller::sound_controller(
	sge::parse::json::object const &o,
	sge::audio::multi_loader &ml,
	sge::audio::player_ptr const _player)
:
	player_(
		_player),
	sounds_(
		stdlib::map<audio_map>(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				o.members,
				FCPPT_TEXT("events")).members,
			[&ml,&player_](sge::parse::json::member const &v)
			{
				return 
					audio_map::value_type(
						v.name,
						player_->create_buffer(
							ml.load(
								create_path(
									sge::parse::json::get<sge::parse::json::string>(
										v.value_),
									FCPPT_TEXT("sounds")))));
			})),
	pool_()
{
	
}

void
insula::sound_controller::play(
	fcppt::string const &p)
{
	if (sounds_.find(p) == sounds_.end())
		throw exception(FCPPT_TEXT("Couldn't find sound effect \"")+p+FCPPT_TEXT("\""));

	sge::audio::sound::base_ptr const b = 
		sounds_[p]->create_nonpositional();

	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::stop_mode::play_once);
}

void
insula::sound_controller::play_positional(
	fcppt::string const &p,
	sge::audio::sound::positional_parameters const &pp)
{
	if (sounds_.find(p) == sounds_.end())
		throw exception(FCPPT_TEXT("Couldn't find sound effect \"")+p+FCPPT_TEXT("\""));

	sge::audio::sound::base_ptr const b = 
		sounds_[p]->create_positional(
			pp);

	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::stop_mode::play_once);
}

void
insula::sound_controller::update()
{
	pool_.update();
}

insula::sound_controller::~sound_controller() {}

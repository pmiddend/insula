#include "music_controller.hpp"
#include "get_option.hpp"
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/multi_loader.hpp>

insula::music_controller::music_controller(
	boost::program_options::variables_map const &vm,
	sge::audio::multi_loader &ml,
	sge::audio::player_ptr const player)
:
	source_()
{
	if (vm.count("music-file"))
	{
		source_ = 
			player->create_nonpositional_stream(
				ml.load(
					get_option<fcppt::string>(vm,"music-file")));
		source_->play(
			sge::audio::sound::repeat::once);
	}
}

void
insula::music_controller::update()
{
	if (source_)
		source_->update();
}

boost::program_options::options_description 
insula::music_controller::cli_options()
{
	boost::program_options::options_description opts("Music");
	opts.add_options()
		("music-file",boost::program_options::value<fcppt::string>(),"Absolute location of the music file to be looped in the background.");
	return opts;
}

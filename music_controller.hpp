#ifndef INSULA_MUSIC_CONTROLLER_HPP_INCLUDED
#define INSULA_MUSIC_CONTROLLER_HPP_INCLUDED

#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/sound/base_ptr.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
class music_controller
{
public:
	music_controller(music_controller const &) = delete;
	music_controller &operator=(music_controller const &) = delete;

	explicit
	music_controller(
		boost::program_options::variables_map const &,
		sge::audio::multi_loader &,
		sge::audio::player_ptr);

	void
	update();

	static boost::program_options::options_description 
	cli_options();
private:
	sge::audio::sound::base_ptr source_;
};
}

#endif

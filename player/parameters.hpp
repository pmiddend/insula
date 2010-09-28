#ifndef INSULA_PLAYER_PARAMETERS_HPP_INCLUDED
#define INSULA_PLAYER_PARAMETERS_HPP_INCLUDED

#include "../random_engine.hpp"
#include "../height_map/object_fwd.hpp"
#include "../height_map/scalar.hpp"
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace player
{
class parameters
{
public:
	sge::parse::json::object const &config_file;
	height_map::object &height_map;
	height_map::scalar &water_level;
	random_engine &rng_engine;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		height_map::object &height_map,
		height_map::scalar water_level,
		random_engine &rng_engine)
	:
		config_file(config_file),
		height_map(height_map),
		water_level(water_level),
		rng_engine(rng_engine)
	{
	}
};
}
}

#endif

#ifndef INSULA_CREATE_VARIABLES_MAP_HPP_INCLUDED
#define INSULA_CREATE_VARIABLES_MAP_HPP_INCLUDED

#include <sge/parse/json/object_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
boost::program_options::variables_map const
create_variables_map(
	int argc,
	char **argv,
	sge::parse::json::object const &);
}

#endif

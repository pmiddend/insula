#ifndef INSULA_PARSE_JSON_CONFIG_HPP_INCLUDED
#define INSULA_PARSE_JSON_CONFIG_HPP_INCLUDED

#include <boost/program_options/parsers.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/char_type.hpp>

namespace insula
{
boost::program_options::basic_parsed_options<fcppt::char_type> const
parse_json_config(
	sge::parse::json::object const &,
	boost::program_options::options_description const &);
}

#endif

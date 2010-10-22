#ifndef INSULA_JSON_CONFIG_WRAPPER_HPP_INCLUDED
#define INSULA_JSON_CONFIG_WRAPPER_HPP_INCLUDED

#include <sge/parse/json/object.hpp>
#include <fcppt/string.hpp>
#include <vector>

namespace insula
{
namespace json
{
// This function wraps the ever-repeating "read config file(s), add
// options from the console" part. Note that this function "includes"
// the config.json by default, so it makes no sense to include it in
// the additional_files vector
sge::parse::json::object const
config_wrapper(
	std::vector<fcppt::string> const &additional_files,
	int argc,
	char *argv[]);
}
}

#endif

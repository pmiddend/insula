#ifndef INSULA_JSON_PARSE_FONT_HPP_INCLUDED
#define INSULA_JSON_PARSE_FONT_HPP_INCLUDED

#include <sge/font/metrics_ptr.hpp>
#include <sge/font/system_ptr.hpp>
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace json
{
sge::font::metrics_ptr const
parse_font(
	sge::parse::json::object const &,
	sge::font::system_ptr);
}
}

#endif

#include "parse_font.hpp"
#include "../create_path.hpp"
#include <sge/font/system.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/int_type.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/text.hpp>

sge::font::metrics_ptr const
insula::json::parse_font(
	sge::parse::json::object const &o,
	sge::font::system_ptr const sys)
{
	return 
		sys->create_font(
			create_path(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					o.members,
					FCPPT_TEXT("metrics")),
				FCPPT_TEXT("fonts")),
			static_cast<sge::font::size_type>(
				sge::parse::json::find_member_exn<sge::parse::json::int_type>(
					o.members,
					FCPPT_TEXT("size"))));
}

#ifndef INSULA_JSON_MEMBER_HAS_TYPE_HPP_INCLUDED
#define INSULA_JSON_MEMBER_HAS_TYPE_HPP_INCLUDED

#include <sge/parse/json/object.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/invalid_get.hpp>
#include <fcppt/string.hpp>

namespace insula
{
namespace json
{
template<typename T>
bool
member_has_type(
	sge::parse::json::object const &o,
	fcppt::string const &s)
{
	try
	{
		sge::parse::json::find_member_exn<T>(
			o.members,
			s);
	}
	catch (sge::parse::json::invalid_get const &)
	{
		return false;
	}
	return true;
}
}
}

#endif

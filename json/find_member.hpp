#ifndef INSULA_JSON_FIND_MEMBER_HPP_INCLUDED
#define INSULA_JSON_FIND_MEMBER_HPP_INCLUDED

#include "convert.hpp"
#include "../stdlib/accumulate.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/member_name_equal.hpp>
#include <fcppt/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include <algorithm>

namespace insula
{
namespace json
{
template<typename T>
T const &
find_member(
	sge::parse::json::object const &o,
	fcppt::string const &path)
{
	typedef
	std::vector<fcppt::string>
	string_vector;

	string_vector parts;
	
	sge::parse::json::object *target = 
		stdlib::accumulate(
			boost::algorithm::split(
				path,
				parts,
				boost::algorithm::is_any_of(FCPPT_TEXT("/"))),
			&o,
			[](
				sge::parse::json::object *o,
				fcppt::string const &s)
			{
				return 
					&sge::parse::json::find_member_exn<sge::parse::json::object>(
						o->members,
						s);
			});

	sge::parse::json::member_vector::iterator it = 
		std::find_if(
			target->begin(),
			target->end(),
			sge::parse::json::member_name_equal(
				parts.back()));

	if (it == target->end())
		throw exception(
			FCPPT_TEXT("Couldn't find member \"")+parts.back()+FCPPT_TEXT("\""));

	return 
		convert<T>(
			*it);
}
}
}

#endif

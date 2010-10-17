#ifndef INSULA_JSON_FIND_MEMBER_HPP_INCLUDED
#define INSULA_JSON_FIND_MEMBER_HPP_INCLUDED

#include "convert.hpp"
#include "../stdlib/accumulate.hpp"
#include "../exception.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/member_name_equal.hpp>
#include <fcppt/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include <algorithm>

#include <boost/foreach.hpp>
#include <fcppt/io/cout.hpp>
namespace insula
{
namespace json
{
template<typename T>
T const 
find_member(
	sge::parse::json::object const &o,
	fcppt::string const &path)
{
	typedef
	std::vector<fcppt::string>
	string_vector;

	string_vector parts;

	boost::algorithm::split(
		parts,
		path,
		boost::algorithm::is_any_of(FCPPT_TEXT("/")));

	fcppt::string const last_element = parts.back();

	parts.pop_back();

	sge::parse::json::object const *target = 
		stdlib::accumulate(
			parts,
			&o,
			[](
				sge::parse::json::object const *o,
				fcppt::string const &s)
			{
				return 
					&sge::parse::json::find_member_exn<sge::parse::json::object>(
						o->members,
						s);
			});

	sge::parse::json::member_vector::const_iterator it = 
		std::find_if(
			target->members.begin(),
			target->members.end(),
			sge::parse::json::member_name_equal(
				last_element));

	if (it == target->members.end())
		throw exception(
			FCPPT_TEXT("Couldn't find member \"")+parts.back()+FCPPT_TEXT("\""));

	return 
		insula::json::convert<T>(
			it->value_);
}
}
}

#endif

#ifndef INSULA_STDLIB_SHORTEST_LEVENSHTEIN_HPP_INCLUDED
#define INSULA_STDLIB_SHORTEST_LEVENSHTEIN_HPP_INCLUDED

#include "levenshtein.hpp"
#include <boost/foreach.hpp>

namespace insula
{
namespace stdlib
{
/// Returns the array element with the shortest levenshtein
/// distance. 
/// Undefined if c.empty()
template<typename Container>
typename 
Container::value_type
shortest_levenshtein(
	Container const &c,
	typename Container::value_type const &ref)
{
	typedef typename 
	Container::value_type
	string_type;

	typedef typename 
	string_type::size_type
	size_type;

	size_type shortest_dist = 
		levenshtein(
			*c.begin(),
			ref);

	string_type shortest = 
		*c.begin();

	BOOST_FOREACH(
		typename Container::const_reference r,
		c)
	{
		size_type const new_shortest_dist =
			levenshtein(
				r,
				ref);

		if (new_shortest_dist < shortest_dist)
		{
			shortest = r;
			shortest_dist = new_shortest_dist;
		}
	}

	return shortest;
}
}
}

#endif

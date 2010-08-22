#ifndef INSULA_STDLIB_NORMALIZE_HPP_INCLUDED
#define INSULA_STDLIB_NORMALIZE_HPP_INCLUDED

#include <algorithm>
#include <utility>

namespace insula
{
namespace stdlib
{
// This normalizes the elements of a container
template<typename Container>
Container const
normalize(
	Container c)
{
	typedef typename
	Container::value_type
	value_type;

	std::pair
	<
		value_type const *,
		value_type const *
	> minmax_ptr = 
		std::minmax_element(
			c.begin(),
			c.end());
	
	std::pair<value_type,value_type>
		minmax(
			*minmax_ptr.first,
			*minmax_ptr.second);
	
	std::transform(
		c.begin(),
		c.end(),
		c.begin(),
		[&minmax](value_type const &e) 
		{ 
			return (e - minmax.first)/(minmax.second - minmax.first);
		});

	return c;
}
}
}

#endif

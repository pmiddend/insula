#ifndef INSULA_STDLIB_SORT_HPP_INCLUDED
#define INSULA_STDLIB_SORT_HPP_INCLUDED

#include <algorithm>

namespace insula
{
namespace stdlib
{
template<typename Sequence,typename Comp>
Sequence const 
sort(
	Sequence s,
	Comp c)
{
	std::sort(
		s.begin(),
		s.end(),
		c);
	return s;
}
}
}

#endif

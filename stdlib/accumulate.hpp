#ifndef INSULA_STDLIB_ACCUMULATE_HPP_INCLUDED
#define INSULA_STDLIB_ACCUMULATE_HPP_INCLUDED

#include <algorithm>

namespace insula
{
namespace stdlib
{
template<typename Container, typename T,typename BinaryOp>
T
accumulate(
	Container const &c,
	T const &init,
	BinaryOp const &operation)
{
	return 
		std::accumulate(
			c.begin(),
			c.end(),
			init,
			operation);
}
}
}

#endif

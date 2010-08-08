#ifndef INSULA_STDLIB_COPY_HPP_INCLUDED
#define INSULA_STDLIB_COPY_HPP_INCLUDED

#include <algorithm>

namespace insula
{
namespace stdlib
{
template<typename Container,typename Iterator>
void
copy(
	Container const &c,
	Iterator const i)
{
	std::copy(
		c.begin(),
		c.end(),
		i);
}
}
}

#endif

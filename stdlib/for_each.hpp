#ifndef INSULA_STDLIB_FOR_EACH_HPP_INCLUDED
#define INSULA_STDLIB_FOR_EACH_HPP_INCLUDED

#include <algorithm>

namespace insula
{
namespace stdlib
{
template<typename T,typename F>
void
for_each(
	T const &t,
	F const &f)
{
	std::for_each(
		t.begin(),
		t.end(),
		f);
}
}
}

#endif

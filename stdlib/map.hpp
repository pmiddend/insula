#ifndef INSULA_STDLIB_TRANSFORM_COPY_HPP_INCLUDED
#define INSULA_STDLIB_TRANSFORM_COPY_HPP_INCLUDED

#include <algorithm>
#include <iterator>

namespace insula
{
namespace stdlib
{
template<typename TargetContainer,typename SourceContainer,typename Functor>
TargetContainer const
map(
	SourceContainer const &s,
	Functor const &f)
{
	TargetContainer t;
	std::transform(
		s.begin(),
		s.end(),
		std::inserter<TargetContainer>(
			t,
			t.end()),
		f);
	return t;
}
}
}

#endif

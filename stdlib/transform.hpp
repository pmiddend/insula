#ifndef INSULA_STDLIB_TRANSFORM_HPP_INCLUDED
#define INSULA_STDLIB_TRANSFORM_HPP_INCLUDED

#include <algorithm>

namespace insula
{
namespace stdlib
{
// The result container is a hack for containers which don't have a
// canonical constructor (one which takes a size
// argument). grid::object is an example of this.
template
<
	typename ResultContainer,
	typename Container1,
	typename Container2,
	typename Functor
>
ResultContainer const
transform(
	Container1 const &c1,
	Container2 const &c2,
	ResultContainer r,
	Functor const &f)
{
	std::transform(
		c1.begin(),
		c1.end(),
		c2.begin(),
		r.begin(),
		f);
	return r;
}
}
}

#endif

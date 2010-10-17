#ifndef INSULA_STDLIB_RANGE_CONSTRUCT_HPP_INCLUDED
#define INSULA_STDLIB_RANGE_CONSTRUCT_HPP_INCLUDED

namespace insula
{
namespace stdlib
{
template<typename Result,typename Argument>
Result const
range_construct(
	Argument const &a)
{
	return Result(a.begin(),a.end());
}
}
}

#endif

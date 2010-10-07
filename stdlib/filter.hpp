#ifndef INSULA_STDLIB_FILTER_HPP_INCLUDED
#define INSULA_STDLIB_FILTER_HPP_INCLUDED

#include <boost/foreach.hpp>

namespace insula
{
namespace stdlib
{
template<typename Container,typename Functor>
Container const
filter(
	Container const &c,
	Functor const &f)
{
	Container result;
	BOOST_FOREACH(
		typename Container::const_reference r,
		c)
		if (f(r))
			result.push_back(
				r);
	return result;
}
}
}

#endif

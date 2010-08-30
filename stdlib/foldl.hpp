#ifndef INSULA_STDLIB_FOLDL_HPP_INCLUDED
#define INSULA_STDLIB_FOLDL_HPP_INCLUDED

namespace insula
{
namespace stdlib
{
template<typename Target,typename Container,typename Functor>
Target const
foldl(
	Functor const f,
	Target const start,
	Container const &c,
	typename Container::size_type const n = 
		static_cast<typename Container::size_type>(0))
{
	if (n == static_cast<typename Container::size_type>(c.size()))
		return start;

	return 
		foldl(
			f,
			f(
				start,
				*std::next(
					c.begin(),
					n)),
			c,
			static_cast<typename Container::size_type>(
				n+1));
}
}
}

#endif

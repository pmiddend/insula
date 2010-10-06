#ifndef INSULA_STDLIB_KEYS_TO_SEQUENCE_HPP_INCLUDED
#define INSULA_STDLIB_KEYS_TO_SEQUENCE_HPP_INCLUDED

namespace insula
{
namespace stdlib
{
template<typename Sequence,typename Map>
Sequence const 
keys_to_sequence(
	Map const &m)
{
	Sequence s;
	for (typename Map::const_iterator i = m.begin(); i != m.end(); ++i)
		s.push_back(
			i->first);
	return s;
}
}
}

#endif

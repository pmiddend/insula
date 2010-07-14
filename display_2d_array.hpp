#ifndef INSULA_DISPLAY_2D_ARRAY_HPP_INCLUDED
#define INSULA_DISPLAY_2D_ARRAY_HPP_INCLUDED

#include <fcppt/io/ostream.hpp>
#include <fcppt/text.hpp>
#include <boost/foreach.hpp>
#include <boost/multi_array.hpp>
#include <ostream>

namespace insula
{
template<typename T>
void
display_2d_array(
	fcppt::io::ostream &s,
	boost::multi_array<T,2> const &a)
{
	typedef 
	boost::multi_array<T,2>
	array_type;

	typedef typename 
	array_type::const_reference
	row;

	typedef typename
	row::reference
	column;

	BOOST_FOREACH(row r,a)
	{
		for (typename row::const_iterator i = r.begin(); i != r.end(); ++i)
			s << *i << FCPPT_TEXT(", ");
		// NOTE: this doesn't compile for some reason
		//BOOST_FOREACH(column c,r)
		//	s << c << FCPPT_TEXT(", ");
		s << FCPPT_TEXT('\n');
	}
}
}

#endif

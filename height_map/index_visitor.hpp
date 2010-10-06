#ifndef INSULA_HEIGHT_MAP_INDEX_VISITOR_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_INDEX_VISITOR_HPP_INCLUDED

#include "array.hpp"
#include "calculate_index_cell.hpp"
#include <fcppt/math/dim/basic_impl.hpp>

namespace insula
{
namespace height_map
{
struct index_visitor
{
public:
	typedef 
	void 
	result_type;

	typedef 
	array::size_type
	size_type;

	explicit
	index_visitor(
		array::dim const &_dim)
	:
		dim_(
			_dim)
	{
	}

	template<typename T>
	result_type
	operator()(
		T const &t) const
	{
		typedef typename 
		T::value_type 
		value_type;

		typename T::iterator it = t.begin();

		for (size_type y = 0; y < static_cast<size_type>(dim_.h()-1); ++y)
			for (size_type x = 0; x < static_cast<size_type>(dim_.w()-1); ++x)
				BOOST_FOREACH(
					value_type const i,
					calculate_index_cell<value_type>(
						static_cast<value_type>(
							x),
						static_cast<value_type>(
							y),
						static_cast<value_type>(
							dim_.w()),
						static_cast<value_type>(
							dim_.h())))
					*it++ = i;
	}
private:
	array::dim dim_;
};
}
}

#endif

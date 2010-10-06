#ifndef INSULA_SKYDOME_INDEX_VISITOR_HPP_INCLUDED
#define INSULA_SKYDOME_INDEX_VISITOR_HPP_INCLUDED

#include "size_type.hpp"
#include <fcppt/assert.hpp>

namespace insula
{
namespace skydome
{
// NOTE: We should replace this by a non-generic class since there are
// only two index types
struct index_visitor
{
public:
	// apply_unary needs this
	typedef 
	void 
	result_type;

	size_type ib_size;
	size_type it_lat,it_lon;

	explicit
	index_visitor(
		size_type const ib_size,
		size_type const it_lat,
		size_type const it_lon)
	:
		ib_size(
			ib_size),
		it_lat(
			it_lat),
		it_lon(
			it_lon)
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

		size_type indices = 0;

		// First, the triangles at the top
		for (size_type lons = 1; lons <= it_lon; ++lons)
		{
			/* DEBUG
			fcppt::io::cout
				<< "Adding triangle: "
				<< 0
				<< ","
				<< ((lons == it_lon) ? 1 : (lons+1))
				<< ","
				<< lons
				<< "\n";*/
			*it++ = 0;
			*it++ = 
				static_cast<value_type>(
					lons == it_lon ? 1 : (lons+1));
			*it++ = 
				static_cast<value_type>(
					lons);
			indices += 3;
		}

		// Start at the first vertex after the top
		value_type current = 1;
		// Again, we have it_lat-1 ring pairs (see below)
		for (size_type lats = 0; lats < static_cast<size_type>(it_lat-1); ++lats)
		{
			// Remember the beginning index of the current circle so we can wrap around later in the inner loop
			value_type const beginning = 
				current;
			// The last index would wrap around the first one, so it_lon-1
			for (size_type lon = 0; lon < static_cast<size_type>(it_lon); ++lon)
			{
				value_type const 
					next = 
						lon == static_cast<size_type>(it_lon-1)
						?
							static_cast<value_type>(beginning)
						:
							static_cast<value_type>(
								current+1),
					below = 
						static_cast<value_type>(
							current + it_lon),
					next_below = 
						lon == static_cast<size_type>(it_lon-1)
						?
							static_cast<value_type>(
								beginning + it_lon)
						:
							static_cast<value_type>(
								below+1);
				
				*it++ = current;
				*it++ = next_below;
				*it++ = below;

				*it++ = current;
				*it++ = next_below;
				*it++ = next;
				
				indices += 6;
			
				++current;
			}
		}
		
		FCPPT_ASSERT(indices <= ib_size);
	}
};
}
}

#endif

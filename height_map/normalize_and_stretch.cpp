#include "normalize_and_stretch.hpp"
#include <fcppt/container/grid/object_impl.hpp>
#include <algorithm>
#include <utility>

void
insula::height_map::normalize_and_stretch(
	array &a)
{
	std::pair
	<
		array::value_type const *,
		array::value_type const *
	> minmax_ptr = 
		std::minmax_element(
			a.begin(),
			a.end());
	
	std::pair<array::value_type,array::value_type>
		minmax(
			*minmax_ptr.first,
			*minmax_ptr.second);
	
	std::transform(
		a.begin(),
		a.end(),
		a.begin(),
		[&minmax](array::value_type const &e) 
		{ 
			return (e - minmax.first)/(minmax.second - minmax.first);
		});
}

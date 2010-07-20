#include "normalize_and_stretch.hpp"
#include <algorithm>
#include <utility>

void
insula::height_map::normalize_and_stretch(
	array &a)
{
	std::pair
	<
		array::element const *,
		array::element const *
	> minmax_ptr = 
		std::minmax_element(
			a.data(),
			a.data() + a.num_elements());
	
	std::pair<array::element,array::element>
		minmax(
			*minmax_ptr.first,
			*minmax_ptr.second);
	
	std::transform(
		a.data(),
		a.data() + a.num_elements(),
		a.data(),
		[&minmax](array::element const &e) 
		{ 
			return (e - minmax.first)/(minmax.second - minmax.first);
		});
}

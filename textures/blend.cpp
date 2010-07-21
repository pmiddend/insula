#include "blend.hpp"
#include "rgb_view.hpp"
#include "rgb_store.hpp"
#include "../height_map/array.hpp"
#include <mizuiro/image/const_view.hpp>
#include <mizuiro/image/store.hpp>
#include <mizuiro/image/types/pointer.hpp>
#include <mizuiro/image/types/normal.hpp>
#include <mizuiro/image/types/raw.hpp>
#include <mizuiro/image/make_const_view.hpp>
#include <mizuiro/color/operators/scalar_multiply.hpp>
#include <mizuiro/color/operators/add.hpp>
#include <mizuiro/nonconst_tag.hpp>
#include <sge/image/file.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <functional>
#include <iterator>

insula::textures::rgb_store const
insula::textures::blend(
	rgb_view const &gradient_image,
	rgb_view_sequence const &images,
	height_map::array const &heights,
	height_map::array const &grad,
	interpolators::base &lerper)
{
	FCPPT_ASSERT(
		!images.empty());
	
	FCPPT_ASSERT(
		std::all_of(
			images.begin(),
			images.end(),
			[&images](rgb_view const &f) { return f.dim() == images.front().dim(); }) &&
		gradient_image.dim() == images.front().dim());

	rgb_store m(
		images.front().dim());
	
	rgb_store::view_type target = 
		m.view();
	
	for (rgb_store::dim_type::value_type y = 0; y < target.dim()[0]; ++y)
	{
		for (rgb_store::dim_type::value_type x = 0; x < target.dim()[1]; ++x)
		{
			weights const w = 
				lerper.calculate_weights(
					static_cast<weight>(
						heights[y][x]),
					static_cast<weight>(
						grad[y][x]));

			FCPPT_ASSERT(
				static_cast<weights::weight_sequence::size_type>(w.sequence().size()) == 
				static_cast<weights::weight_sequence::size_type>(images.size()));

			//fcppt::io::cout << "weight sequence: ";
			//BOOST_FOREACH(weights::weight_sequence::const_reference r,w)
			//	fcppt::io::cout << r << ",";
			//fcppt::io::cout << "\n";

			// Current position
			rgb_store::dim_type const cp(
				x,
				y);

			target.at(cp) = 
				w.gradient() * gradient_image.at(cp);

			rgb_view_sequence::const_iterator view_it = 
				images.begin();
			for (weights::weight_sequence::const_iterator i = w.sequence().begin(); i != w.sequence().end(); ++i)
				target.at(cp) = 
					target.at(cp) + 
					(*i) * (view_it++)->at(cp);
		}
	}
	
	return m;
}

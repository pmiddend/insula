#include "blend.hpp"
#include "../height_map/array.hpp"
#include <sge/image/view/detail/view_types.hpp>
#include <sge/image/color/rgb8_format.hpp>
#include <sge/image/file.hpp>
#include <mizuiro/image/const_view.hpp>
#include <mizuiro/image/store.hpp>
#include <mizuiro/image/types/pointer.hpp>
#include <mizuiro/image/types/normal.hpp>
#include <mizuiro/image/types/raw.hpp>
#include <mizuiro/image/make_const_view.hpp>
#include <mizuiro/color/operators/scalar_multiply.hpp>
#include <mizuiro/color/operators/add.hpp>
#include <mizuiro/nonconst_tag.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/variant/invalid_get.hpp>
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

sge::image::file_ptr const
insula::textures::blend(
	image_sequence &images,
	height_map::array const &heights,
	interpolators::base &lerper)
try
{
	FCPPT_ASSERT(
		!images.empty());
	
	FCPPT_ASSERT(
		std::all_of(
			images.begin(),
			images.end(),
			[&images](sge::image::file_ptr const &f) { return f->dim() == images.front()->dim(); }));
	
	FCPPT_ASSERT(
		std::inner_product(
			images.front()->dim().begin(),
			images.front()->dim().end(),
			heights.shape(),
			true,
			std::logical_and<bool>(),
			std::equal_to<std::size_t>())
		);

	typedef
	mizuiro::image::const_view
	<
		sge::image::view::detail::view_types
		<
			sge::image::color::rgb8_format
		>::type
	>::type
	target_view;

	typedef
	std::vector<target_view>
	view_sequence;

	view_sequence views;
	
	std::transform(
		images.begin(),
		images.end(),
		std::back_inserter<view_sequence>(
			views),
		[](sge::image::file_ptr const f) 
		{ 
			return f->view().get<target_view>(); 
		});
	
	typedef
	mizuiro::image::store
	<
		target_view::format,
		target_view::access
	>
	store_type;
	
	store_type m(
		views.front().dim());
	
	store_type::view_type target = 
		m.view();
	
	for (store_type::dim_type::value_type x = 0; x < target.dim()[0]; ++x)
	{
		for (store_type::dim_type::value_type y = 0; y < target.dim()[1]; ++y)
		{
			weights::weight_sequence const w = 
				lerper.calculate_weights(
					static_cast<weight>(
						heights[x][y]),
					// the gradient
					static_cast<weight>(
						0)).sequence();

			FCPPT_ASSERT(
				w.size() == views.size());

			//fcppt::io::cout << "weight sequence: ";
			//BOOST_FOREACH(weights::weight_sequence::const_reference r,w)
			//	fcppt::io::cout << r << ",";
			//fcppt::io::cout << "\n";

			// Current position
			store_type::dim_type const cp(
				x,
				y);

			target.at(cp) = 
				w.front() * views.front().at(cp);

			view_sequence::const_iterator view_it = 
				std::next(
					views.begin());
			for (weights::weight_sequence::const_iterator i = std::next(w.begin()); i != w.end(); ++i)
				target.at(cp) = target.at(cp) + (*i) * view_it->at(cp);
		}
	}
	
	images.front()->data(
		mizuiro::image::make_const_view(
			target));
	
	return images.front();
}
catch (fcppt::variant::invalid_get const &)
{
	throw fcppt::exception(FCPPT_TEXT("Tried to use an image as height texture which is not rgb8"));
}

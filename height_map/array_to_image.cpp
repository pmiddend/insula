#include "array_to_image.hpp"
#include "gray_view.hpp"
#include "gray_store.hpp"
#include <mizuiro/color/object.hpp>
#include <mizuiro/color/homogenous.hpp>
#include <mizuiro/color/layout/gray.hpp>
#include <mizuiro/color/init/gray.hpp>
#include <mizuiro/image/make_const_view.hpp>
#include <fcppt/container/grid/object_impl.hpp>
#include <sge/image/loader.hpp>
#include <sge/image/file.hpp>

void
insula::height_map::array_to_image(
	array const &a,
	sge::image::multi_loader &loader,
	fcppt::filesystem::path const &file)
{
	gray_store m(
		gray_store::dim_type(
			// FIXME: static_cast
			a.dimension()[0],
			a.dimension()[1]));
	
	gray_store::view_type target = 
		m.view();

	typedef boost::uint8_t channel_type;

	typedef 
	mizuiro::color::object<
		mizuiro::color::homogenous
		<
			channel_type,
			mizuiro::color::layout::gray
		>
	> gray_color;
	
	for (gray_store::dim_type::value_type y = 0; y < target.dim()[0]; ++y)
	{
		for (gray_store::dim_type::value_type x = 0; x < target.dim()[1]; ++x)
		{
			gray_store::dim_type const cp(
				x,
				y);

			target.at(cp) = 
				gray_color(
					(mizuiro::color::init::gray %= a[array::dim(x,y)]));
		}
	}

	loader.loaders().at(0)->create(
		mizuiro::image::make_const_view(
			target))->save(file);
}

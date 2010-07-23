#include "image_to_array.hpp"
#include "gray_view.hpp"
#include <sge/image/file.hpp>
#include <sge/image/color/gray8_format.hpp>
#include <mizuiro/image/const_view.hpp>
#include <mizuiro/image/view_impl.hpp>
#include <mizuiro/color/normalize.hpp>
#include <mizuiro/color/channel/gray.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/variant/invalid_get.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>

#include <functional>

insula::height_map::array const
insula::height_map::image_to_array(
	sge::image::file_ptr const im)
try
{
	gray_view const v(
		im->view().get<gray_view>());

	array h(
		v.dim());
	
	std::transform(
		v.begin(),
		v.end(),
		h.data(),
		[](gray_view::iterator::reference const &f) { return mizuiro::color::normalize<mizuiro::color::channel::gray,float>(f); });

	return h;
}
catch (fcppt::variant::invalid_get const &)
{
	throw fcppt::exception(FCPPT_TEXT("Tried to convert an image to a height map which is not greyscale!"));
}

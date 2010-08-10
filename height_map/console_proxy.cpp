#include "console_proxy.hpp"
#include "object.hpp"
#include "image_to_array.hpp"
#include <sge/console/object.hpp>
#include <sge/image/multi_loader.hpp>
#include <fcppt/filesystem/is_regular.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/bad_lexical_cast.hpp>
#include <functional>

insula::height_map::console_proxy::console_proxy(
	object &_obj,
	sge::console::object &console,
	sge::image::multi_loader &_image_loader)
:
	shader_(
		FCPPT_TEXT("terrain"),
		obj_.shader(),
		console),
	/*
	regenerate_connection_(
		console.insert(
			FCPPT_TEXT("terrain_regenerate"),
			std::bind(
				&console_proxy::regenerate,
				this,
				std::placeholders::_1,
				std::placeholders::_2),
			FCPPT_TEXT("Regenerate terrain from a file"),
			FCPPT_TEXT("Usage: /terrain_regenerate <filename> <cell-size> <height-scaling>\nThe height-scaling is arbitrary (can even be negative). The file name is below media/"))),*/
	image_loader_(
		_image_loader),
	obj_(
		_obj)
{
}

void
insula::height_map::console_proxy::regenerate(
	sge::console::arg_list const &,
	sge::console::object &)
{
/*
	if (args.size() <= 3)
	{
		con.emit_error(
			FCPPT_TEXT("Invalid number of arguments"));
		return;
	}

	if (!fcppt::filesystem::is_regular(args[1]))
	{
		con.emit_error(
			FCPPT_TEXT("File \"")+
			args[1]+
			FCPPT_TEXT("\" isn't regular"));
		return;

	}

	try
	{
		obj_.regenerate(
			fcppt::lexical_cast<graphics::scalar>(
				args[2]),
			fcppt::lexical_cast<graphics::scalar>(
				args[3]),
			image_to_array(
				image_loader_.load(
					args[1])));
	}
	catch (fcppt::bad_lexical_cast const &)
	{
		con.emit_error(
			FCPPT_TEXT("Cell size/height scaling invalid"));
		return;
	}
*/
}

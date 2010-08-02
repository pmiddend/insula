#include "console_proxy.hpp"
#include "object.hpp"
#include <sge/console/object.hpp>
#include <sge/image/multi_loader.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/bad_lexical_cast.hpp>
#include <functional>

insula::skydome::console_proxy::console_proxy(
	object &_obj,
	sge::console::object &console)
:
	shader_(
		FCPPT_TEXT("terrain"),
		obj_.shader(),
		console),
	regenerate_connection_(
		console.insert(
			FCPPT_TEXT("skydome_regenerate"),
			std::bind(
				&console_proxy::regenerate,
				this,
				std::placeholders::_1,
				std::placeholders::_2),
			FCPPT_TEXT("regenerate skydome, parameters are self-explanatory"))),
	obj_(
		_obj)
{
}

void
insula::skydome::console_proxy::regenerate(
	sge::console::arg_list const &args,
	sge::console::object &con)
{
	if (args.size() <= 3)
	{
		con.emit_error(
			FCPPT_TEXT("usage: ")+args[0]+FCPPT_TEXT(" <angle> <latitudes> <longitudes>"));
		return;
	}

	try
	{
		obj_.regenerate_buffer(
			fcppt::lexical_cast<graphics::scalar>(
				args[1]),
			fcppt::lexical_cast<skydome::size_type>(
				args[2]),
			fcppt::lexical_cast<skydome::size_type>(
				args[3]));
	}
	catch (fcppt::bad_lexical_cast const &)
	{
		con.emit_error(
			FCPPT_TEXT("Parameter invalid"));
	}
}

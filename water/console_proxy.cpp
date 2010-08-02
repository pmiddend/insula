#include "console_proxy.hpp"
#include "object.hpp"
#include <sge/console/object.hpp>
#include <fcppt/text.hpp>

insula::water::console_proxy::console_proxy(
	object &_obj,
	sge::console::object &console)
:
	shader_(
		FCPPT_TEXT("water"),
		_obj.shader(),
		console)
{
}

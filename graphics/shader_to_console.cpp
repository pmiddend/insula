#include "shader_to_console.hpp"
#include "shader.hpp"
#include "../console/object.hpp"
#include "../stdlib/for_each.hpp"
#include "scalar.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include <sge/renderer/glsl/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/bad_lexical_cast.hpp>
#include <fcppt/io/cout.hpp>
#include <functional>

insula::graphics::shader_to_console::shader_to_console(
	fcppt::string const &name,
	shader_old &_shader,
	sge::console::object &console)
#if 0
:
	shader_(
		_shader),
	list_con_(
		console.insert(
			name+FCPPT_TEXT("_list"),
			std::bind(
				&shader_to_console::list,
				this,
				std::placeholders::_1,
				std::placeholders::_2),
			FCPPT_TEXT("Lists all uniform variables for \"")+name+FCPPT_TEXT("\""))),
	set_con_(
		console.insert(
			name+FCPPT_TEXT("_set"),
			std::bind(
				&shader_to_console::set,
				this,
				std::placeholders::_1,
				std::placeholders::_2),
			FCPPT_TEXT("Set a uniform variable for \"")+name+FCPPT_TEXT("\""),
			FCPPT_TEXT("This function takes at least 1 and up to 4 arguments. One argument is treated as a scalar value, the other are vec2, vec3, vec4, respectively. You will get an error if:\n-you try to assign a non-numeric value\n-you try to assign a value to a non-defined variable\n-you try to assign a value of the wrong type.")))
#endif
{
}

void
insula::graphics::shader_to_console::list(
	sge::console::arg_list const &args,
	sge::console::object &c)
{
#if 0
	if (args.size() == 1)
		c.emit_error(
			args[0]+FCPPT_TEXT(" takes no arguments"));
	
	stdlib::for_each(
		shader_.uniform_names(),
		[&c](fcppt::string const &s) { c.emit_message(s); });
#endif
}

void
insula::graphics::shader_to_console::set(
	sge::console::arg_list const &a,
	sge::console::object &c)
#if 0
try
#endif
{
#if 0
	shader_.activate();

	if (a.size() <= 2)
		c.emit_error(
			FCPPT_TEXT("usage: ")+a[0]+FCPPT_TEXT(" <name> <value>"));
	
	if (a.size() == 3)
	{
		shader_.set_uniform(
			a[1],
			fcppt::lexical_cast<scalar>(
				a[2]));
	}
	else if (a.size() == 4)
	{
		shader_.set_uniform(
			a[1],
			vec2(
				fcppt::lexical_cast<scalar>(
					a[2]),
				fcppt::lexical_cast<scalar>(
					a[3])));
	}
	else if (a.size() == 5)
	{
		shader_.set_uniform(
			a[1],
			vec3(
				fcppt::lexical_cast<scalar>(
					a[2]),
				fcppt::lexical_cast<scalar>(
					a[3]),
				fcppt::lexical_cast<scalar>(
					a[4])));
	}
	else if (a.size() == 6)
	{
		shader_.set_uniform(
			a[1],
			vec4(
				fcppt::lexical_cast<scalar>(
					a[2]),
				fcppt::lexical_cast<scalar>(
					a[3]),
				fcppt::lexical_cast<scalar>(
					a[4]),
				fcppt::lexical_cast<scalar>(
					a[5])));
	}
	else
	{
		c.emit_error(
			FCPPT_TEXT("invalid number of arguments"));
	}
#endif
}
#if 0
catch (fcppt::bad_lexical_cast const &)
{
	c.emit_error(
		FCPPT_TEXT("invalid argument type, only scalars are allowed"));
}
catch (sge::renderer::glsl::exception const &e)
{
	c.emit_error(
		FCPPT_TEXT("glsl error while setting variable, probably wrong type: ")+e.string());
}
#endif

#include "exception.hpp"

insula::exception::exception(
	fcppt::string const &string_)
:
	fcppt::exception(
		string_)
{}

insula::exception::~exception() throw()
{
}

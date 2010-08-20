#include "help_needed.hpp"

insula::help_needed::help_needed(
	fcppt::string const &s)
:
	exception(s)
{
}

insula::help_needed::~help_needed() throw()
{
}

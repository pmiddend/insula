#ifndef INSULA_HELP_NEEDED_HPP_INCLUDED
#define INSULA_HELP_NEEDED_HPP_INCLUDED

#include "exception.hpp"
#include <fcppt/string.hpp>

namespace insula
{
class help_needed
:
	public exception
{
public:
	explicit 
	help_needed(
		fcppt::string const &);

	virtual ~help_needed() throw();
};
}

#endif

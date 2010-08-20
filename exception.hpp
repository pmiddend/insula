#ifndef INSULA_EXCEPTION_HPP_INCLUDED
#define INSULA_EXCEPTION_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

namespace insula
{
class exception
:
	public fcppt::exception
{
public:
	explicit 
	exception(
		fcppt::string const &);

	virtual ~exception() throw();
};
}

#endif

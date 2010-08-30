#ifndef INSULA_MILLISECONDS_TO_STRING_HPP_INCLUDED
#define INSULA_MILLISECONDS_TO_STRING_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <chrono>

namespace insula
{
fcppt::string const
milliseconds_to_string(
	std::chrono::milliseconds const &);
}

#endif

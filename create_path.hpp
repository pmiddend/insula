#ifndef INSULA_CREATE_PATH_HPP_INCLUDED
#define INSULA_CREATE_PATH_HPP_INCLUDED

#include <fcppt/filesystem/path.hpp>
#include <fcppt/string.hpp>

namespace insula
{
fcppt::filesystem::path const
create_path(
	fcppt::string const &,
	fcppt::string const &dir_below_media);
}

#endif

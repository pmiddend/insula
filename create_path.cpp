#include "create_path.hpp"
#include "media_path.hpp"
#include <fcppt/assert.hpp>

fcppt::filesystem::path const
insula::create_path(
	fcppt::string const &g,
	fcppt::string const &dir_below_media)
{
	FCPPT_ASSERT(!g.empty());

	if (g[0] == FCPPT_TEXT('/'))
		return g;

	return media_path()/dir_below_media/g;
}

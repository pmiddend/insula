#ifndef INSULA_SCENE_AUTO_UNLINK_HOOK_HPP_INCLUDED
#define INSULA_SCENE_AUTO_UNLINK_HOOK_HPP_INCLUDED

#include <boost/intrusive/list.hpp>

namespace insula
{
namespace scene
{
typedef 
boost::intrusive::list_base_hook
<
	boost::intrusive::link_mode
	<
		boost::intrusive::auto_unlink
	> 
> 
auto_unlink_hook;
}
}

#endif

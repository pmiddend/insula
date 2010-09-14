#ifndef INSULA_PROP_INSTANCE_SEQUENCE_HPP_INCLUDED
#define INSULA_PROP_INSTANCE_SEQUENCE_HPP_INCLUDED

#include "../scene/transparent_instance.hpp"
#include <boost/ptr_container/ptr_vector.hpp>

namespace insula
{
namespace prop
{
typedef
boost::ptr_vector<scene::transparent_instance>
instance_sequence;
}
}

#endif

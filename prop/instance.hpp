#ifndef INSULA_PROP_INSTANCE_HPP_INCLUDED
#define INSULA_PROP_INSTANCE_HPP_INCLUDED

#include "instance_parameters_fwd.hpp"
#include "../scene/transparent_instance.hpp"
#include <boost/ptr_container/ptr_vector.hpp>

namespace insula
{
namespace prop
{
class instance
{
public:
	explicit
	instance(
		instance_parameters const &);
private:
	typedef
	boost::ptr_vector<scene::transparent_instance>
	instance_sequence;

	instance_sequence instances_;
};
}
}

#endif

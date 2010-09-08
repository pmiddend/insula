#ifndef INSULA_PROP_BACKEND_HPP_INCLUDED
#define INSULA_PROP_BACKEND_HPP_INCLUDED

#include "../model/object.hpp"
#include "../model_backend.hpp"

namespace insula
{
namespace prop
{
class backend
{
public:
	model::object model_;
	model_backend scene_backend_;
};
}
}

#endif

#ifndef INSULA_SCENE_SCOPED_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_SCOPED_BACKEND_HPP_INCLUDED

#include "backend_ptr.hpp"

namespace insula
{
namespace scene
{
class scoped_backend
{
public:
	explicit
	scoped_backend(
		backend_ptr);

	~scoped_backend();
private:
	backend_ptr backend_;
};
}
}

#endif

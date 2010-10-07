#ifndef INSULA_SCENE_SCOPED_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_SCOPED_BACKEND_HPP_INCLUDED

#include "backend_ptr.hpp"
#include "render_pass/object_fwd.hpp"

namespace insula
{
namespace scene
{
class scoped_backend
{
public:
	explicit
	scoped_backend(
		backend_ptr,
		render_pass::object const &);

	~scoped_backend();
private:
	backend_ptr backend_;
	render_pass::object const &render_pass_;
};
}
}

#endif

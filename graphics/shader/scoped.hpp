#ifndef INSULA_GRAPHICS_SHADER_SCOPED_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"

namespace insula
{
namespace graphics
{
namespace shader
{
class scoped
{
public:
	scoped(scoped const &) = delete;
	scoped &operator=(scoped const &) = delete;
	
	explicit
	scoped(
		object &);

	~scoped();
private:
	object &object_;
};
}
}
}

#endif

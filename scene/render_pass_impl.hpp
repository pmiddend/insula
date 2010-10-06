#ifndef INSULA_SCENE_RENDER_PASS_IMPL_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_IMPL_HPP_INCLUDED

#include "render_pass.hpp"
#include "render_pass_callback.hpp"
#include "manager_fwd.hpp"

namespace insula
{
namespace scene
{
class render_pass_impl
{
public:
	explicit	
	render_pass_impl(
		manager &,
		render_pass::type,
		render_pass_callback const &);
	
	~render_pass_impl();
private:
	manager &manager_;
	render_pass::type rp_;
};
}
}

#endif

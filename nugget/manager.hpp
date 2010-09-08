#ifndef INSULA_NUGGET_MANAGER_HPP_INCLUDED
#define INSULA_NUGGET_MANAGER_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "../graphics/vec2.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>

namespace insula
{
namespace nugget
{
class manager
{
public:
	explicit
	manager(
		parameters const &);
private:
	// Those are the positions from which the model list is
	// reconstructed each turn. We only need the x and z coordinates,
	// the y coordinates are determined from that
	typedef
	std::vector<graphics::vec2>
	position_sequence;

	position_sequence nugget_positions_;
	model::object model_;
	model_backend backend_;
};
}
}

#endif

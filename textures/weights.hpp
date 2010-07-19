#ifndef INSULA_TEXTURES_WEIGHTS_HPP_INCLUDED
#define INSULA_TEXTURES_WEIGHTS_HPP_INCLUDED

#include "weight.hpp"
#include <vector>

namespace insula
{
namespace textures
{
class weights
{
public:
	typedef
	std::vector
	<
		weight
	>
	weight_sequence;

	typedef
	weight_sequence::size_type
	size_type;

	explicit
	weights(
		weight_sequence const &,
		weight gradient);
	
	weight_sequence const &
	sequence() const;

	weight
	gradient() const;
private:
	weight_sequence sequence_;
	weight gradient_;
};
}
}

#endif

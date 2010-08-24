#include "random_seed.hpp"
#include <ctime>

insula::random_engine::result_type
insula::random_seed()
{
	return 
		static_cast<random_engine::result_type>(
			std::time(
				0));
}

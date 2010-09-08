#include "random_seed.hpp"
#include <chrono>

insula::random_engine::result_type
insula::random_seed()
{
	return 
		static_cast<insula::random_engine::result_type>(
			std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

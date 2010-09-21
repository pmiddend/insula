#include "random_point.hpp"
#include "object.hpp"
#include <fcppt/container/grid/object.hpp>

#include <fcppt/io/cerr.hpp>
#include <fcppt/math/vector/output.hpp>

namespace
{
template<typename T>
bool
inside_range(
	T const &t,
	T const &a,
	T const &b)
{
	return t >= a && t <= b;
}

unsigned long const iteration_threshold = 1000000;
}

insula::height_map::vec2 const
insula::height_map::random_point(
	object const &o,
	scalar const water_level,
	random_engine &engine,
	flatness_range const &f)
{
	// For now, generate just some random point
	std::uniform_real_distribution<scalar> 
		xdist(
			o.extents().left(),
			o.extents().right()),
		ydist(
			o.extents().front(),
			o.extents().back());

	vec2 result;

	unsigned long iterations = 0;

	do
	{
		result = 
			vec2(
				xdist(
					engine),
				ydist(
					engine));

		if (iterations++ > iteration_threshold)
		{
			fcppt::io::cerr 
				<< FCPPT_TEXT("I've tried my best to generate a point in the gradient range ") 
				<< f
				<< FCPPT_TEXT(" but it didn't work. Now I'll just take the next best point.\n");
			break;
		}
	} 
	while(
		o.project(result) < water_level ||
		!inside_range(
			o.gradient()[
				array::dim(
					static_cast<array::size_type>(
						result.x()/o.cell_size()),
					static_cast<array::size_type>(
						result.y()/o.cell_size()))],
			f.x(),
			f.y()));

	return 
		result;
}

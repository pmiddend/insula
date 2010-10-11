#include "height_for_point.hpp"
#include "scalar.hpp"
#include "vec3.hpp"
#include "dim2.hpp"
#include "rect.hpp"
#include <fcppt/math/box/contains_point.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/container/grid/object_impl.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>

#include <fcppt/io/cout.hpp>

insula::height_map::scalar
insula::height_map::height_for_point(
	array const &heights,
	scalar cell_size,
	vec2 const &p,
	vec2 const &)
{
	// This is too strict due to numerical inaccuracies
	/*
	FCPPT_ASSERT_MESSAGE(
		fcppt::math::box::contains_point(
			rect(
				vec2::null(),
				fcppt::math::dim::structure_cast<dim2>(
					static_cast<array::size_type>(cell_size) * heights.dimension())),
			p),
		FCPPT_TEXT("The point is not inside the height field"));
	*/

	if(
		!fcppt::math::box::contains_point(
			rect(
				vec2::null(),
				fcppt::math::dim::structure_cast<dim2>(
					static_cast<array::size_type>(cell_size) * heights.dimension())),
			p))
		return static_cast<scalar>(0);


	// Determine which grid cell we're talking about
	array::dim const cell = 
		fcppt::math::vector::structure_cast<array::dim>(
			p / cell_size);

//	fcppt::io::cout << "cell size is " << cell_size << ", point is " << p << "\n";
//	fcppt::io::cout << "calculated the cell: " << cell << "\n";

	// Then, define a line through the top left and the bottom right
	// corner of the quad (the diagonal line) in slope form: 
	// f(x)=m*x+b
	dim2 const 
		topleft = 
			fcppt::math::dim::structure_cast<dim2>(cell) * 
			static_cast<scalar>(cell_size),
		bottomright = 
			topleft + 
			dim2(
				cell_size,
				cell_size);

//	fcppt::io::cout << "cell's top left: " << topleft << ", cell's bottom right: " << bottomright << "\n";

	// This should only happen if cell_size is zero.
	FCPPT_ASSERT(
		!fcppt::math::almost_zero(
			topleft.w() - bottomright.w()));

	scalar const 
		denom = bottomright.w() - topleft.w(),
		m = (bottomright.h() - topleft.h())/denom,
		//b = cross(topleft,bottomright)/denom;
		b = (bottomright.w() * topleft.h() - bottomright.h()*topleft.w())/denom;

//	fcppt::io::cout << "m: " << m << ", b: " << b << "\n";

	// Define the triangle points
	vec3 vertices[3];

	if (p.y() > (m * p.x() + b))
	{
//		fcppt::io::cout << "choosing the upper triangle\n";
		vertices[0] = 
			vec3(
				static_cast<scalar>(cell.w()) * static_cast<scalar>(cell_size),
				heights[array::dim(cell.w(),cell.h())],
				static_cast<scalar>(cell.h()) * static_cast<scalar>(cell_size));
		vertices[1] = 
			vec3(
				static_cast<scalar>(cell.w()+1) * static_cast<scalar>(cell_size),
				heights[array::dim(cell.w()+1,cell.h()+1)],
				static_cast<scalar>(cell.h()+1) * static_cast<scalar>(cell_size));
		vertices[2] = 
			vec3(
				static_cast<scalar>(cell.w()+1) * static_cast<scalar>(cell_size),
				heights[array::dim(cell.w(),cell.h())],
				static_cast<scalar>(cell.h()) * static_cast<scalar>(cell_size));
	}
	else
	{
//		fcppt::io::cout << "choosing the lower triangle\n";
		vertices[0] = 
			vec3(
				static_cast<scalar>(cell.w()) * static_cast<scalar>(cell_size),
				heights[array::dim(cell.w(),cell.h())],
				static_cast<scalar>(cell.h()) * static_cast<scalar>(cell_size));
		vertices[1] = 
			vec3(
				static_cast<scalar>(cell.w()) * static_cast<scalar>(cell_size),
				heights[array::dim(cell.w(),cell.h()+1)],
				static_cast<scalar>(cell.h()+1) * static_cast<scalar>(cell_size));
		vertices[2] = 
			vec3(
				static_cast<scalar>(cell.w()+1) * static_cast<scalar>(cell_size),
				heights[array::dim(cell.w()+1,cell.h()+1)],
				static_cast<scalar>(cell.h()+1) * static_cast<scalar>(cell_size));
	}

	// Define plane for triangle
	vec3 const 
		normal = 
			normalize(
				cross(
					vertices[1]-vertices[0],
					vertices[2]-vertices[0]));

	scalar const lambda = 
		dot(
			normal,
			vertices[0]);

	/*
	fcppt::io::cout 
		<< "height at nearest neighbor is " 
		<< heights[array::dim(cell.w(),cell.h())] 
		<< ", calculated height is "
		<< ((lambda - normal[0] * p[0] - normal[2] * p[2])/normal[1])
		<< "\nnormal is " 
		<< normal
		<< "\n";
*/


	return 
		(lambda - normal[0] * p[0] - normal[2] * p[1])/*/normal[1]*/;
}

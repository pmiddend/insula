#ifndef INSULA_MATH_TRIANGLE_BASIC_HPP_INCLUDED
#define INSULA_MATH_TRIANGLE_BASIC_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/assert.hpp>
#include <initializer_list>
#include <array>
#include <algorithm>

namespace insula
{
namespace math
{
namespace triangle
{
template<typename T,fcppt::math::size_type N>
class basic
{
public:
	typedef typename
	fcppt::math::vector::static_<T,N>::type
	vector;

	typedef 
	std::array<vector,3>
	array_type;

	array_type points;
	array_type normals;

	explicit
	basic(
		std::initializer_list<vector> const &_points)
	{
		FCPPT_ASSERT(
			_points.size() == 3);
		std::copy(
			_points.begin(),
			_points.end(),
			points.begin());
	}

	explicit
	basic(
		std::initializer_list<vector> const &_points,
		std::initializer_list<vector> const &_normals)
	{
		FCPPT_ASSERT(
			_points.size() == 3 && _normals.size() == 3);
		std::copy(
			_points.begin(),
			_points.end(),
			points.begin());
		std::copy(
			_normals.begin(),
			_normals.end(),
			normals.begin());
	}
};
}
}
}

#endif

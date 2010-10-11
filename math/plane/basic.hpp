#ifndef INSULA_MATH_PLANE_BASIC_HPP_INCLUDED
#define INSULA_MATH_PLANE_BASIC_HPP_INCLUDED

#include <fcppt/math/vector/vector.hpp>
#include <fcppt/io/ostream.hpp>

namespace insula
{
namespace math
{
namespace plane
{
template<typename T>
class basic
{
public:
	typedef typename
	fcppt::math::vector::static_<T,3>::type 
	vector;

	typedef
	T
	scalar;

	explicit
	basic()
	{
	}

	explicit
	basic(
		vector const &_normal,
		scalar const _lambda)
	:
		normal_(
			_normal),
		lambda_(
			_lambda)
	{
	}

	vector const
	normal() const
	{
		return normal_;
	}

	scalar const
	lambda() const
	{
		return lambda_;
	}
private:
	vector normal_;
	scalar lambda_;
};
}
}
}

template<typename T>
fcppt::io::ostream &
operator<<(
	fcppt::io::ostream &s,
	insula::math::plane::basic<T> const &p)
{
	return 
		s << FCPPT_TEXT('(') << p.normal() << FCPPT_TEXT(',') 
			<< p.lambda() << FCPPT_TEXT(')');
}

#endif

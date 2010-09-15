#ifndef INSULA_JSON_PARSE_VECTOR_HPP_INCLUDED
#define INSULA_JSON_PARSE_VECTOR_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/get.hpp>
#include <boost/foreach.hpp>

namespace insula
{
namespace json
{
template<typename T,fcppt::math::size_type N,typename Source>
typename
fcppt::math::vector::static_<T,N>::type const
parse_vector(
	sge::parse::json::array const &a)
{
	typedef
	typename
	fcppt::math::vector::static_<T,N>::type
	vector;

	vector
	result;

	typename vector::iterator i = 
		result.begin();
	

	BOOST_FOREACH(sge::parse::json::value const &v,a.elements)
		*i++ = 
			static_cast<T>(
				sge::parse::json::get<Source>(
					v));

	return result;
}
}
}

#endif

#ifndef INSULA_JSON_CONVERT_HPP_INCLUDED
#define INSULA_JSON_CONVERT_HPP_INCLUDED

#include "../stdlib/range_construct.hpp"
#include "../stdlib/map.hpp"
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/type_traits/is_iterable.hpp>
#include <fcppt/string.hpp>
#include <boost/mpl/and.hpp>
#include <type_traits>

namespace insula
{
namespace json
{

template<typename T>
T const
apply_convert(
	sge::parse::json::value const &_value
);

// Assume float_type
template<typename T>
typename
std::enable_if
<
	std::is_floating_point<T>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		static_cast<T>(
			sge::parse::json::get<sge::parse::json::float_type>(
				v));
}

// Assume int_type
template<typename T>
typename
std::enable_if
<
	std::is_integral<T>::value && !std::is_same<T,bool>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		static_cast<T>(
			sge::parse::json::get<sge::parse::json::int_type>(
				v));
}

// Assume bool
template<typename T>
typename
std::enable_if
<
	std::is_same<T,bool>::value,
	bool
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<bool>(v);
}

// Assume array_type (exception string)
template<typename T>
typename
std::enable_if
<
	fcppt::type_traits::is_iterable<T>::value 
		&& !std::is_same<T,sge::parse::json::string>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		stdlib::range_construct<T>(
			stdlib::map<std::vector<typename T::value_type>>(
				sge::parse::json::get<sge::parse::json::array>(
					v).elements,
				&apply_convert<typename T::value_type>));
}

// This special case handles fcppt::string which would otherwise be
// seen as an array
template<typename T>
typename
std::enable_if
<
	std::is_same<T,sge::parse::json::string>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<sge::parse::json::string>(
			v);
}

// Assume...nothing. You cannot json::get for a json::value, hence
// this special case
template<typename T>
typename
std::enable_if
<
	std::is_same<T,sge::parse::json::value>::value,
	sge::parse::json::value const
>::type
convert(
	sge::parse::json::value const &v)
{
	return v;
}

// Everything else -> has to be a json type!
template<typename T>
typename
std::enable_if
<
	!std::is_floating_point<T>::value 
		&& !std::is_integral<T>::value
		&& !fcppt::type_traits::is_iterable<T>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<T>(
			v);
}

template<typename T>
T const
apply_convert(
	sge::parse::json::value const &_value
)
{
	return convert<T>(_value);
}

}
}

#endif

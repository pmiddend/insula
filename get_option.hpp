#ifndef INSULA_GET_OPTION_HPP_INCLUDED
#define INSULA_GET_OPTION_HPP_INCLUDED

#include <fcppt/text.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/from_std_string.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/any.hpp>
#include <string>

namespace insula
{
template<typename T>
T const
get_option(
	boost::program_options::variables_map const &vm,
	std::string const &s)
{
	if (!vm.count(s))
		throw fcppt::exception(
			FCPPT_TEXT("Option \"")+fcppt::from_std_string(s)+FCPPT_TEXT("\" not found! Restart with \"--")+fcppt::from_std_string(s)+FCPPT_TEXT(" value\""));

	try
	{
		return vm[s].as<T>();
	}
	catch (boost::bad_any_cast const &)
	{
		throw 
			fcppt::exception(
				FCPPT_TEXT("Option \"")+fcppt::from_std_string(s)+FCPPT_TEXT("\" doesn't have type ")+
				fcppt::type_name(typeid(T)));
	}
}
}

#endif

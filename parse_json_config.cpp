#include "parse_json_config.hpp"
#include "stdlib/map.hpp"
#include "exception.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/invalid_get.hpp>
#include <sge/parse/json/member.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <boost/program_options/option.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <unordered_set>

boost::program_options::basic_parsed_options<fcppt::char_type> const
insula::parse_json_config(
	sge::parse::json::object const &o,
	boost::program_options::options_description const &desc)
{
	boost::program_options::basic_parsed_options<fcppt::char_type> result(
		&desc);

	typedef
	std::unordered_set<fcppt::string>
	allowed_options_set;

	// options_description has a function which returns the options as a
	// vector<shared_ptr<option_description>>. Note that
	// option_description != options_description ;)
	allowed_options_set const allowed_options = 
		stdlib::map<allowed_options_set>(
			desc.options(),
			[](boost::shared_ptr<boost::program_options::option_description> const &opt) 
			{
				return opt->long_name();
			});

	BOOST_FOREACH(
		sge::parse::json::member const &v,
		o.members)
	{
		try
		{
			sge::parse::json::string const 
				name = 
					v.name,
				value = 
					sge::parse::json::get<sge::parse::json::string>(
						v.value_);

			if (allowed_options.find(name) == allowed_options.end())
				throw exception(
					FCPPT_TEXT("The option \"")+
					name+
					FCPPT_TEXT("\" is unknown"));

			// option has no constructor which inits all values
			boost::program_options::option newopt;
			// string_key is std::string, I don't know how what string type
			// to use here
			newopt.string_key = name;
			// value is a vector (for multitoken maybe)
			newopt.value.push_back(
				value);
			// In the config file parser, this is set to
			// !allowed_option(name), so I guess it's always false in our
			// case
			newopt.unregistered = false;
			// I don't know what purpose the original_tokens have
			newopt.original_tokens.push_back(
				name);
			newopt.original_tokens.push_back(
				value);

			result.options.push_back(
				newopt);
		}
		catch(sge::parse::json::invalid_get const &)
		{
			throw exception(FCPPT_TEXT("Command line entries in the json file must be of type string!"));
		}
	}

	return result;
}

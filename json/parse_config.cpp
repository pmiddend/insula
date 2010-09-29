#include "parse_config.hpp"
#include "member_has_type.hpp"
#include "../stdlib/map.hpp"
#include "../stdlib/shortest_levenshtein.hpp"
#include "../exception.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/invalid_get.hpp>
#include <sge/parse/json/member.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/io/cout.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/program_options/option.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <unordered_set>
#include <iterator>

namespace
{
typedef
std::unordered_set<fcppt::string>
allowed_options_set;

class json_to_string
{
public:
	typedef fcppt::string result_type;

	template<typename T>
	result_type const
	operator()(
		T const &t) const
	{
		return fcppt::lexical_cast<result_type>(t);
	}

	result_type const
	operator()(
		sge::parse::json::object const &) const
	{
		throw insula::exception(FCPPT_TEXT("How did this happen?!"));
	}

	result_type const
	operator()(
		sge::parse::json::null const &) const
	{
		return result_type(FCPPT_TEXT("null"));
	}

	result_type const
	operator()(
		sge::parse::json::array const &a) const
	{
		result_type s = FCPPT_TEXT("(");
		for (sge::parse::json::element_vector::const_iterator i = a.elements.begin(); i != a.elements.end(); ++i)
		{
			s += boost::apply_visitor(*this,*i);
			if (i != std::prev(a.elements.end()))
				s += FCPPT_TEXT(",");
		}
		s += FCPPT_TEXT(")");
		return s;
	}
};

void
parse_config(
	sge::parse::json::object const &o,
	boost::program_options::options_description const &desc,
	boost::program_options::basic_parsed_options<fcppt::char_type> &opts,
	allowed_options_set const &allowed_options,
	fcppt::string const &prefix)
{
	BOOST_FOREACH(
		sge::parse::json::member const &v,
		o.members)
	{
		try
		{
			sge::parse::json::string const 
				name = 
					v.name,
				real_name = 
					prefix+name;

			if (insula::json::member_has_type<sge::parse::json::object>(o,name))
			{
				parse_config(
					sge::parse::json::get<sge::parse::json::object>(
						v.value_),
					desc,
					opts,
					allowed_options,
					prefix+name+FCPPT_TEXT("-"));
				continue;
			}

			fcppt::string const value = 
				boost::apply_visitor(
					json_to_string(),
					v.value_);
			/*
			sge::parse::json::string const value = 
				sge::parse::json::get<sge::parse::json::string>(
					v.value_);
			*/

			if (allowed_options.find(real_name) == allowed_options.end())
				throw insula::exception(
					FCPPT_TEXT("The option \"")+
					real_name+
					FCPPT_TEXT("\" is unknown. Did you mean: ")+
					insula::stdlib::shortest_levenshtein(
						allowed_options,
						real_name)+
					FCPPT_TEXT("?"));

			// option has no constructor which inits all values
			boost::program_options::option newopt;
			// string_key is std::string, I don't know how what string type
			// to use here
			newopt.string_key = real_name;
			// value is a vector (for multitoken maybe)
			newopt.value.push_back(
				value);
			// In the config file parser, this is set to
			// !allowed_option(name), so I guess it's always false in our
			// case
			newopt.unregistered = false;
			// I don't know what purpose the original_tokens have
			newopt.original_tokens.push_back(
				real_name);
			newopt.original_tokens.push_back(
				value);

			opts.options.push_back(
				newopt);
		}
		catch(sge::parse::json::invalid_get const &)
		{
			throw insula::exception(FCPPT_TEXT("Command line entries in the json file must be of type string!"));
		}
	}
}
}

boost::program_options::basic_parsed_options<fcppt::char_type> const
insula::json::parse_config(
	sge::parse::json::object const &o,
	boost::program_options::options_description const &desc)
{
	boost::program_options::basic_parsed_options<fcppt::char_type> result(
		&desc);

	// options_description has a function which returns the options as a
	// vector<shared_ptr<option_description>>. Note that
	// option_description != options_description ;)
	allowed_options_set const allowed_options = 
		insula::stdlib::map<allowed_options_set>(
			desc.options(),
			[](boost::shared_ptr<boost::program_options::option_description> const &opt) 
			{
				return opt->long_name();
			});

	::parse_config(
		o,
		desc,
		result,
		allowed_options,
		FCPPT_TEXT(""));

	return result;
}

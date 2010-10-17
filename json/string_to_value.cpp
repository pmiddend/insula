#include "string_to_value.hpp"
#include "../exception.hpp"
#include <sge/parse/json/member_name_equal.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/parse_stream.hpp>
#include <fcppt/io/istringstream.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <fcppt/format.hpp>
#include <fcppt/string.hpp>

sge::parse::json::value const
insula::json::string_to_value(
	fcppt::string const &s)
{
	using namespace sge::parse::json;

	fcppt::string const edited = 
		(fcppt::format(FCPPT_TEXT("{ \"value\" : %s }")) % s).str();

	fcppt::io::cout << "edited: " << edited << "\n";
	
	fcppt::io::istringstream stream(edited);
	object result;
	if (!parse_stream(stream,result))
		throw exception(FCPPT_TEXT("Couldn't convert string \"")+s+FCPPT_TEXT("\" to json value"));
	return 
		std::find_if(
			result.members.begin(),
			result.members.end(),
			member_name_equal(
				FCPPT_TEXT("value")))->value_;
}

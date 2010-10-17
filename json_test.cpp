#include "json/process_option.hpp"
#include "json/merge_trees.hpp"
#include <sge/parse/json/output/to_stream.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/parse_stream.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/istringstream.hpp>
#include <fcppt/text.hpp>

fcppt::string const 
	global_json1 = 
		FCPPT_TEXT("{ \"renderer\" : { \"screen\" : { \"size\" : [1024,768] }, \"foobar\" : 1.0  }  }"),
	global_json2 = 
		FCPPT_TEXT("{ \"renderer\" : { \"screen\" : { \"size\" : [640,480],\"is_leet\" : true }  }  }");

int main(int argc,char *argv[])
try
{
	fcppt::io::istringstream 
		stream1(
			global_json1),
		stream2(
			global_json2);

	sge::parse::json::object 
		o1,
		o2;

	if(
		!sge::parse::json::parse_stream(stream1,o1) || 
		!sge::parse::json::parse_stream(stream2,o2))
		fcppt::io::cerr << "FFFFUUUUU1!\n";

	fcppt::io::cout << "First tree: \n";
	sge::parse::json::output::to_stream(
		fcppt::io::cout,
		o1);
	fcppt::io::cout << "\n";

	fcppt::io::cout << "Second tree: \n";
	sge::parse::json::output::to_stream(
		fcppt::io::cout,
		o2);
	fcppt::io::cout << "\n";

	insula::json::process_option(
		o1,
		FCPPT_TEXT("renderer/screen/size=[640,480]"));

	fcppt::io::cout << "First tree after processing: \n";
	sge::parse::json::output::to_stream(
		fcppt::io::cout,
		o1);
	fcppt::io::cout << "\n";

	fcppt::io::cout << "Trees after merging:\n";
	sge::parse::json::output::to_stream(
		fcppt::io::cout,
		insula::json::merge_trees(
			o1,
			o2));
	fcppt::io::cout << "\n";
}
catch (fcppt::exception const &e)
{
	fcppt::io::cout << "Fehler: " << e.string() << "\n";
}

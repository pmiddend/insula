#include "json/process_option.hpp"
#include "json/merge_trees.hpp"
#include "json/convert.hpp"
#include <sge/parse/json/output/to_stream.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/parse_stream.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/math/vector/vector.hpp>
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
#if 0
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
#endif
	sge::parse::json::array a;
	sge::parse::json::array suba;
	suba.elements.push_back(1);
	suba.elements.push_back(2);
	suba.elements.push_back(3);
	sge::parse::json::array subb;
	subb.elements.push_back(4);
	subb.elements.push_back(5);
	subb.elements.push_back(6);
	a.elements.push_back(suba);
	a.elements.push_back(subb);
	/*
	fcppt::io::cout << 
		fcppt::type_traits::is_iterable<fcppt::math::vector::static_<int,3>::type>::value << "\n";
	*/
	/*
	std::vector<std::vector<int>> vs = 
		insula::json::convert<std::vector<std::vector<int>>>(
			a);

	for (int i = 0; i < vs.size(); ++i)
	{
		for (int j = 0; j < vs[i].size(); ++j)
			fcppt::io::cout << vs[i][j] << " ";
		fcppt::io::cout << "\n";
	}
	*/
}
catch (fcppt::exception const &e)
{
	fcppt::io::cout << "Fehler: " << e.string() << "\n";
}

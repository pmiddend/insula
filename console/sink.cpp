#include "sink.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sge/console/gfx.hpp>
#include <fcppt/string.hpp>
#include <vector>
#include <iterator>

insula::console::sink::sink(
	sge::console::gfx &_gfx)
:
	gfx_(
		_gfx)
{
}

std::streamsize
insula::console::sink::write(
	char_type const * const s,
	std::streamsize const n)
{
	if (!n)
		return n;

	buffer_ += 
		fcppt::string(
			s,
			s+n);

	typedef
	std::vector<fcppt::string> 
	string_vector; 

	string_vector lines;

	boost::algorithm::split(
		lines,
		buffer_,
		boost::algorithm::is_any_of("\n"));

	buffer_ = lines.back();

	for (
		string_vector::const_iterator i = lines.begin(); 
		i != std::prev(lines.end()); 
		++i)
		gfx_.print(
			*i);

	return n;
}

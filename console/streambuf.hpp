#ifndef INSULA_CONSOLE_STREAMBUF_HPP_INCLUDED
#define INSULA_CONSOLE_STREAMBUF_HPP_INCLUDED

#include <sge/console/object_fwd.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/string.hpp>
#include <fcppt/io/ostream.hpp>
#include <streambuf>

namespace insula
{
namespace console
{
class streambuf
:
	public std::basic_streambuf<fcppt::char_type>
{
public:
	explicit
	streambuf(
		fcppt::io::ostream &,
		sge::console::object &);

	~streambuf();
private:
	bool in_progress_;
	sge::console::object &object_;
	fcppt::io::ostream &stream_;
	std::basic_streambuf<fcppt::char_type> *old_streambuf_;
	fcppt::string buffer_;

	int_type
	overflow(
		int_type);
};
}
}

#endif

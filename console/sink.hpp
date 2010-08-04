#ifndef INSULA_CONSOLE_SINK_HPP_INCLUDED
#define INSULA_CONSOLE_SINK_HPP_INCLUDED

#include <boost/iostreams/categories.hpp>
#include <sge/console/gfx_fwd.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/string.hpp>
#include <iosfwd>

namespace insula
{
namespace console
{
class sink
{
public:
	typedef fcppt::char_type char_type;
	typedef boost::iostreams::sink_tag category;

	explicit sink(
		sge::console::gfx &);

	std::streamsize
	write(
		char_type const *,
		std::streamsize);
private:
	sge::console::gfx &gfx_;
	fcppt::string buffer_;
};
}
}

#endif

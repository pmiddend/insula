#ifndef INSULA_CONSOLE_SCOPED_RDBUF_HPP_INCLUDED
#define INSULA_CONSOLE_SCOPED_RDBUF_HPP_INCLUDED

#include "sink.hpp"
#include <sge/console/gfx_fwd.hpp>
#include <fcppt/io/ostream.hpp>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <streambuf>

namespace insula
{
namespace console
{
class scoped_rdbuf
{
public:
	scoped_rdbuf(scoped_rdbuf const &) = delete;
	scoped_rdbuf &operator=(scoped_rdbuf const &) = delete;
	
	explicit
	scoped_rdbuf(
		fcppt::io::ostream &,
		sge::console::gfx &);

	~scoped_rdbuf();
private:
	typedef
	boost::iostreams::tee_device
	<
		sink,
		std::basic_streambuf<fcppt::char_type>
	>
	tee_device;

	typedef
	boost::iostreams::stream_buffer<tee_device>
	stream_buffer;

	fcppt::io::ostream &stream_;
	sink sink_;
	tee_device tee_;
	stream_buffer stream_buffer_;
	std::basic_streambuf<fcppt::char_type> *old_buffer_;
};
}
}

#endif

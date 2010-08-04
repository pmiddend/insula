#include "scoped_rdbuf.hpp"

insula::console::scoped_rdbuf::scoped_rdbuf(
	fcppt::io::ostream &_stream,
	sge::console::gfx &con)
:
	stream_(
		_stream),
	sink_(
		con),
	tee_(
		stream_,
		sink_),
	stream_buffer_(
		tee_),
	old_buffer_(
		stream_.rdbuf(
			&stream_buffer_))
{
}

insula::console::scoped_rdbuf::~scoped_rdbuf()
{
	stream_.rdbuf(
		old_buffer_);
}

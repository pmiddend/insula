#include "streambuf.hpp"
#include "object.hpp"
#include <fcppt/text.hpp>

insula::console::streambuf::streambuf(
	fcppt::io::ostream &_stream,
	sge::console::object &_object)
:
	in_progress_(
		false),
	object_(
		_object),
	stream_(
		_stream),
	old_streambuf_(
		stream_.rdbuf(this))
{
}

insula::console::streambuf::~streambuf()
{
	stream_.rdbuf(
		old_streambuf_);
}

insula::console::streambuf::int_type
insula::console::streambuf::overflow(
	int_type const c)
{
	if (in_progress_)
		return traits_type::not_eof(c);

	// FIXME: What happens if something throws here?
	in_progress_ = true;

	old_streambuf_->sputc(
		traits_type::to_char_type(
			c));

	if (c != traits_type::eof())
	{
		if (c == FCPPT_TEXT('\n'))
		{
			object_.emit_message(buffer_);
			buffer_.clear();
		}
		else
		{
			buffer_ += traits_type::to_char_type(c);
		}
	}

	in_progress_ = false;

	return traits_type::not_eof(c);
}

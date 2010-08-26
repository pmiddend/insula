#include "timed_output.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/funit.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/io/cout.hpp>
#include <streambuf>
#include <ostream>

template<typename CharType>
struct basic_null_buffer 
: 
	public std::basic_streambuf<CharType>
{
	typedef typename 
	std::basic_streambuf<CharType>::traits_type 
	traits_type;

	int overflow(
		int c) 
	{ 
		return traits_type::not_eof(c); 
	}
};

template<typename CharType>
class basic_null_stream 
: 
	private basic_null_buffer<CharType>, 
	public std::basic_ostream<CharType>
{
public:
	explicit
	basic_null_stream() 
	: 
		std::basic_ostream<CharType>(
			this) 
	{}
};

fcppt::io::ostream &
insula::timed_output()
{
	static sge::time::timer t(
		sge::time::millisecond(
			100));
	static basic_null_stream<fcppt::char_type> null;

	if (t.update_b())
		return fcppt::io::cout;
	return null;
}

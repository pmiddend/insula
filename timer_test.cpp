#include "turn_timer.hpp"
#include "timed_output.hpp"
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>

int main()
{
	insula::turn_timer t;

	fcppt::io::cout 
		<< FCPPT_TEXT("Turn timer before: ") 
		<< t.string() 
		<< FCPPT_TEXT("\n");

	sge::time::timer end_timer(sge::time::second(5));

	fcppt::io::cout 
		<< FCPPT_TEXT("Starting timer...\n");

	t.start();

	while (!end_timer.expired())
	//	insula::timed_output() << t.string() << FCPPT_TEXT("\n");
		fcppt::io::cout << FCPPT_TEXT('\r') << t.string();
	fcppt::io::cout << FCPPT_TEXT("\n");
	
	t.stop();

	fcppt::io::cout 
		<< FCPPT_TEXT("After stopping: ") << t.string() << FCPPT_TEXT('\n');
}

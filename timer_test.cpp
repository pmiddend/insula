#include "turn_timer.hpp"
#include "timed_output.hpp"
#include "milliseconds_to_string.hpp"
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>

int main()
{
	fcppt::io::cout 
		<< FCPPT_TEXT("Starting timer...\n");

	insula::turn_timer t(
		std::chrono::milliseconds(
			20000));

	sge::time::timer add_timer(
		sge::time::second(5));

	while (!t.expired())
	{
		if (add_timer.update_b())
		{
			fcppt::io::cout << FCPPT_TEXT("\nAdding some time...\n");
			t.add_to_remaining(
				std::chrono::milliseconds(1000));
		}
		fcppt::io::cout 
			<< FCPPT_TEXT('\r') 
			<< 
				insula::milliseconds_to_string(
					t.milliseconds_remaining())
			<< FCPPT_TEXT('|')
			<<
				insula::milliseconds_to_string(
					t.milliseconds_total());
	}
	fcppt::io::cout << FCPPT_TEXT("\n");
}

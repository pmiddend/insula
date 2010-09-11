#include "create_variables_map.hpp"
#include "scoped_machine.hpp"
#include "machine.hpp"
// The first state to be used has to be included here
#include "states/freelook.hpp"
#include "help_needed.hpp"
#include "media_path.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/text.hpp>
#include <iostream>
#include <cstddef>

#include <string>
#include <vector>
namespace
{
template<typename T>
typename 
std::basic_string<T>::difference_type 
distance(
	std::basic_string<T> const &source, 
	std::basic_string<T> const &target) 
{
	typedef typename
	std::basic_string<T>::difference_type 
	result_type;

	result_type const 
		n = static_cast<result_type>(source.length()),
		m = static_cast<result_type>(target.length());

	if (source.empty())
		return m;
	if (target.empty())
		return n;

	typedef
	fcppt::container::grid::object
	<
		result_type,
		2
	>
	grid;

	typedef typename
	grid::vector
	vector;

	grid matrix(
		vector(
			static_cast<result_type>(
				n+1),
			static_cast<result_type>(
				m+1)));

  // Step 2

  for (result_type i = 0; i <= n; i++)
    matrix[vector(i,0)] = i;

  for (result_type j = 0; j <= m; j++)
    matrix[vector(0,j)] = j;

  for (result_type i = 1; i <= n; i++) 
	{
    T const s_i = 
			source[i-1];

    for (result_type j = 1; j <= m; j++) 
		{
      T const t_j = target[j-1];

      result_type cost;
      if (s_i == t_j) 
			{
        cost = 0;
      }
      else 
			{
        cost = 1;
      }

      // Step 6

      result_type const 
				above = matrix[vector(i-1,j)],
				left = matrix[vector(i,j-1)],
				diag = matrix[vector(i-1,j-1)],
				cell = std::min(above + 1,std::min(left + 1, diag + cost));

      // Step 6A: Cover transposition, in addition to deletion,
      // insertion and substitution. This step is taken from:
      // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's 
      // Enhanced Dynamic Programming ASM Algorithm"
      // (http://www.acm.org/~hlb/publications/asm/asm.html)
      if (i>static_cast<result_type>(2) && j>static_cast<result_type>(2)) 
			{
        result_type trans = matrix[vector(i-2,j-2)]+1;
        if (source[i-2]!=t_j) trans++;
        if (s_i!=target[j-2]) trans++;
        if (cell>trans) cell = trans;
      }

      matrix[vector(i,j)] = cell;
    }
  }

  return matrix[n][m];
}

}

int main(int argc,char *argv[])
try
{
	sge::parse::json::object const config_file =
		sge::parse::json::parse_file_exn(
			insula::media_path()/FCPPT_TEXT("config.json"));

	insula::machine m(
		config_file,
		insula::create_variables_map(
			argc,
			argv,
			config_file));

	insula::scoped_machine scope(
		m);

	sge::time::timer frame_timer(
		sge::time::second(1));
	while (m.running())
		m.tick(
			frame_timer.reset());
}
catch (insula::help_needed const &h)
{
	fcppt::io::cout << h.string() << FCPPT_TEXT("\n");
	return EXIT_SUCCESS;
}
catch (fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	std::cerr << e.what() << '\n';
	return EXIT_FAILURE;
}

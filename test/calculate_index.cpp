#include "../height_map/calculate_index_cell.hpp"
#include <boost/test/unit_test.hpp>

namespace
{
template<typename T>
std::tr1::array<T,6> const
calculate_index_cell(
	T const x,
	T const y,
	T const w,
	T const)
{
	std::tr1::array<T,6> a;
	T const 
		base = 
			static_cast<T>(
				y * w + x),
		nextbase = 
			static_cast<T>(
				(y+1) * w + x);

	a[0] = base;
	a[1] = static_cast<T>(nextbase+1);
	a[2] = static_cast<T>(base+1);
	a[3] = base;
	a[4] = nextbase;
	a[5] = static_cast<T>(nextbase+1);
	return a;
}
}

BOOST_AUTO_TEST_CASE(calculate_index)
{
	typedef
	std::size_t
	size_type;

	typedef 
	std::tr1::array<size_type,6>
	array_type;

	{
	array_type const a = 
		insula::height_map::calculate_index_cell<size_type>(
			0,
			0,
			5,
			3);

	BOOST_CHECK(
		a[0] == 0 && 
		a[1] == 6 && 
		a[2] == 1 && 
		a[3] == 0 && 
		a[4] == 5 && 
		a[5] == 6);
	}

	{
	array_type const a = 
		insula::height_map::calculate_index_cell<size_type>(
			1,
			1,
			5,
			3);

	BOOST_CHECK(
		a[0] == 6 && 
		a[1] == 12 && 
		a[2] == 7 && 
		a[3] == 6 && 
		a[4] == 11 && 
		a[5] == 12);
	}

	{
	array_type const a = 
		insula::height_map::calculate_index_cell<size_type>(
			2,
			0,
			5,
			3);

	BOOST_CHECK(
		a[0] == 2 && 
		a[1] == 8 && 
		a[2] == 3 && 
		a[3] == 2 && 
		a[4] == 7 && 
		a[5] == 8);
	}

	{
	array_type const a = 
		insula::height_map::calculate_index_cell<size_type>(
			0,
			0,
			3,
			3);

	BOOST_CHECK(
		a[0] == 0 && 
		a[1] == 4 && 
		a[2] == 1 && 
		a[3] == 0 && 
		a[4] == 3 && 
		a[5] == 4);
	}
}

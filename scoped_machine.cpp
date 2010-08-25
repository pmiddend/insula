#include "scoped_machine.hpp"
#include "machine.hpp"
#include "states/freelook.hpp"

insula::scoped_machine::scoped_machine(
	machine &m)
:
	m(m)
{
	m.initiate();
}

insula::scoped_machine::~scoped_machine()
{
	m.terminate();
}

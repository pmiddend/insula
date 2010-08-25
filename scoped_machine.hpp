#ifndef INSULA_SCOPED_MACHINE_HPP_INCLUDED
#define INSULA_SCOPED_MACHINE_HPP_INCLUDED

#include "machine_fwd.hpp"

namespace insula
{
/**
	Motivation: When insula::machine goes out of scope, its destructor
	is called. This destructor destroys all of the data fields like
	sge::systems::instance. _THEN_ it destroys the outermost state. In
	the destructor of the outermost state, however, we might need some
	of the data fields from the machine. So to properly dispose a
	machine, we have to call terminate, which done in this class.
 */
class scoped_machine
{
public:
	scoped_machine(scoped_machine const &) = delete;
	scoped_machine &operator=(scoped_machine &) = delete;

	explicit
	scoped_machine(
		machine &);

	~scoped_machine(
		);
private:
	machine &m;
};
}

#endif

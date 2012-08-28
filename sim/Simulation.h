/*----------------------------------------------------------------------------*
 * class Simulation
 *   Generic simulation type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Constants.h"
#include "sim/Environment.h"

namespace sim
{

class Simulation
{

public:

	// constructors and destructors
	Simulation();
	~Simulation();

	void run();

protected:

	Environment *env;

};

} // namespace sim

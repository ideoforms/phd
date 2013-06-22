/*----------------------------------------------------------------------------*
 * class Environment:
 *   Generic environment type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Constants.h"
#include "sim/Util.h"
#include "sim/Agent.h"

#include <vector>
#include <bitset>

using namespace std;

namespace sim
{

extern struct settings_t settings;

class Environment
{

public:

	// constructors and destructors
	Environment(int population);
	~Environment();

	void 				update();
	void 				dump();
	void				perturb(double amount = 0.5);
	stats_t				stats();

	int 				get_popsize();

	double 				payoff(Task genotype);
	vector <Agent *> 	get_neighbours(Agent &agent);

	/*-----------------------------------------------------------------------*
	 * define as a friend so that agents can directly access protected
	 * properties of the environment.
	 *-----------------------------------------------------------------------*/
	friend class Agent;

	friend ostream & operator<< (ostream &stream, const Environment &object)
	{
		stream << "[environment: t = " <<
			object.mAge << ", pop = " <<
			object.mAgents.size() << ", task = " <<
			object.mTask << "]";
		return stream;
	}

protected:

	std::vector <Agent> 	mAgents;

	Task					mTask;
	int						mSwitchIndex;
	int						mAge;
};

} // namespace sim

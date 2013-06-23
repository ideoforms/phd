/*----------------------------------------------------------------------------*
 * class Environment:
 *   Generic environment type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Constants.h"
#include "sim/Util.h"
#include "sim/Agent.h"
#include "sim/Geometry.h"

#include <vector>
#include <bitset>

using namespace std;

namespace sim
{

extern struct settings_t settings;

// template <class A>
class Environment
{

public:

	// constructors and destructors
	Environment(int population);
	~Environment() {};

	void 					update();
	void 					reproduce();
	void 					fluctuate();
	void 					dump();
	void					perturb(double amount = 0.5);
	stats_t					stats();

	int 					get_popsize();

	double 					payoff(Task genotype);
	vector <Agent *> 		get_neighbours(Agent &agent);

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

	std::vector <Agent> 	mAgents;

	Task					mTask;
	int						mSwitchIndex;
	int						mAge;
};

class EnvironmentCA : public Environment
{

public:

	EnvironmentCA(unsigned width, unsigned height);
	~EnvironmentCA();

	Agent *				agent_at(unsigned x, unsigned y);

	stats_t				stats();
	vector <Agent *> 	get_neighbours(Agent &agent);

	friend class Agent;

	friend ostream & operator<< (ostream &stream, const EnvironmentCA &object)
	{
		stream << "[environment-ca: " <<
			"t = " << object.mAge << ", " <<
			"dim = [" << object.mWidth << ", " << object.mHeight << "], " <<
			"task = " << object.mTask << "]";
		return stream;
	}

protected:

	/*-----------------------------------------------------------------------*
	 * A CA environment has a fixed integer width and height (which are
	 * usually the same)
	 *-----------------------------------------------------------------------*/
	unsigned					mWidth;
	unsigned					mHeight;

	/*-----------------------------------------------------------------------*
	 * Methods to return the next/previous X/Y values, wrapping around grid
	 *-----------------------------------------------------------------------*/
	unsigned					px(unsigned x) { return (x == 0) ? this->mWidth - 1 : x - 1; }
	unsigned					nx(unsigned x) { return (x == mWidth - 1) ? 0 : x + 1; }
	unsigned					py(unsigned y) { return (y == 0) ? this->mHeight - 1 : y - 1; }
	unsigned					ny(unsigned y) { return (y == mHeight - 1) ? 0 : y + 1; }

	/*-----------------------------------------------------------------------*
	 * Store a 2D grid of cells, each of which holds precisely one agent.
	 *-----------------------------------------------------------------------*/
	vector <vector <Agent *> > 	mGrid;
	vector <Point2D <int> >		mPositions;

};

} // namespace sim


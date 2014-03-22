/*----------------------------------------------------------------------------*
 * class Environment:
 *   Generic environment type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Environment.h"

using namespace std;

namespace sim
{

class EnvironmentCA2DMulti : public Environment
{

public:

	EnvironmentCA2DMulti();
	~EnvironmentCA2DMulti();

    virtual double              payoff(Agent *agent, Task genotype);
	virtual void                reproduce();
    virtual void                update();
    virtual void                move(Agent *agent);
	virtual Task                goal_for(Agent *agent);
	virtual vector <Agent *> 	get_neighbours(const Agent *agent);
	virtual stats_t				stats();

	friend class Agent;

	virtual const char * classname() { return "EnvironmentCA2DMulti"; }
    

	friend ostream & operator<< (ostream &stream, const EnvironmentCA2DMulti &object)
	{
		stream << "[environment-ca: " <<
			"t = " << object.mAge << ", " <<
			"dim = [" << object.mWidth << ", " << object.mHeight << "]";
		return stream;
	}

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
	 * Store a 2D grid of cells, each of which holds a vector of 0 or more
	 * agents.
	 *-----------------------------------------------------------------------*/
	vector <vector <vector <Agent *> > > 		mGrid;
	vector <Point2Di>							mPositions;
};

} // namespace sim


/*----------------------------------------------------------------------------*
 * class Environment:
 *   Generic environment type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Constants.h"
#include "sim/Util.h"
#include "sim/Agent.h"
#include "sim/Geometry.h"
#include "sim/Landscape.h"

using namespace std;

namespace sim
{

extern struct settings_t settings;

class Environment
{

public:

	Environment();
	~Environment() {};

	/*-----------------------------------------------------------------------*
	 * all of these declared as virtual so that they can be overriden in
	 * subclasses and then looked up successfully
	 *-----------------------------------------------------------------------*/
	virtual void                update();
	virtual int                 select_parent();
	virtual void                reproduce();
	virtual void                fluctuate();
	virtual void                dump() {};
	virtual void                perturb(double amount = 0.5);
	virtual void 				invade(unsigned mode, float mode_value);

	virtual stats_t             stats();

	int                         get_popsize();

    virtual void                move(Agent *agent);
	virtual double              payoff(Agent *agent, Task genotype);
	virtual Task				goal_for(Agent *agent);
	virtual vector <Agent *> 	get_neighbours(const Agent *agent);

	/*-----------------------------------------------------------------------*
	 * define as a friend so that agents can directly access protected
	 * properties of the environment.
	 *-----------------------------------------------------------------------*/
	friend class Agent;

	virtual const char * classname() { return "Environment"; }

	friend ostream & operator<< (ostream &stream, const Environment &object)
	{
		stream << "[environment: t = " <<
			object.mAge << ", pop = " <<
			object.mAgents.size() << ", task = " <<
			object.mLandscape->taskAt(0, 0) << "]";
		return stream;
	}

	std::vector <Agent *> 	mAgents;

	int						mAge;

	/*-----------------------------------------------------------------------*
	 * Task landscape, usually containing a grid of the same dimensions as
	 * mGrid.
	 *-----------------------------------------------------------------------*/
	Landscape					*mLandscape;

};
        

class EnvironmentABM : public Environment
{

public:

	EnvironmentABM();
	~EnvironmentABM();

    virtual void 		update();
	virtual void 		reproduce();

	virtual vector <Agent *> 	get_neighbours(const Agent *agent);

	friend class Agent;

	virtual const char * classname() { return "EnvironmentABM"; }

	friend ostream & operator<< (ostream &stream, const EnvironmentABM &object)
	{
		stream << "[environment-ca: " <<
			"t = " << object.mAge << ", " <<
			"dim = [" << object.mWidth << ", " << object.mHeight << "]";
		return stream;
	}

	/*-----------------------------------------------------------------------*
	 * An ABM environment has a fixed integer width and height (which are
	 * usually the same)
	 *-----------------------------------------------------------------------*/
	unsigned						mWidth;
	unsigned						mHeight;

	vector <Point2Df>		mPositions;
    vector <Point2Df>		mVelocity;

};


} // namespace sim


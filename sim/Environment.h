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
	Environment();
	~Environment() {};

	/*-----------------------------------------------------------------------*
	 * all of these declared as virtual so that they can be overriden in
	 * subclasses and then looked up successfully
	 *-----------------------------------------------------------------------*/
	virtual void                update();
	virtual void                reproduce();
	virtual void                fluctuate();
	virtual void                dump() {};
	virtual void                perturb(double amount = 0.5);
	stats_t                     stats();

	int                         get_popsize();

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
        
class EnvironmentCA1D : public Environment
{
    
public:
    
    EnvironmentCA1D();
    ~EnvironmentCA1D();
    
    virtual void                reproduce();
    virtual Task                goal_for(Agent *agent);
    virtual vector <Agent *> 	get_neighbours(const Agent *agent);
    
    friend class Agent;

	virtual const char * classname() { return "Environment1D"; }
    
    friend ostream & operator<< (ostream &stream, const EnvironmentCA1D &object)
    {
        stream << "[environment-ca1D: " << "t = " << object.mAge << ", " << "dim = [" << object.mWidth << "]";
        return stream;
    }
    
    unsigned                    mWidth;
    
    /*-----------------------------------------------------------------------*
     * Methods to return the next/previous X/Y values, wrapping around grid
     *-----------------------------------------------------------------------*/
    unsigned					px(unsigned x) { return (x == 0) ? this->mWidth - 1 : x - 1; }
    unsigned					nx(unsigned x) { return (x == mWidth - 1) ? 0 : x + 1; }
    
};

class EnvironmentCA : public Environment
{

public:

	EnvironmentCA();
	~EnvironmentCA();

	virtual void                reproduce();
	virtual Task                goal_for(Agent *agent);
	virtual vector <Agent *> 	get_neighbours(const Agent *agent);

	friend class Agent;

	virtual const char * classname() { return "EnvironmentCA"; }

	friend ostream & operator<< (ostream &stream, const EnvironmentCA &object)
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
	 * Store a 2D grid of cells, each of which holds precisely one agent.
	 *-----------------------------------------------------------------------*/
	vector <vector <Agent *> > 	mGrid;
	vector <Point2Di>			mPositions;
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


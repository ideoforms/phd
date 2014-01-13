/*----------------------------------------------------------------------------*
 * class Landscape
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Util.h"
#include "sim/Constants.h"

#include <vector>
#include <bitset>

using namespace std;

namespace sim
{

class Landscape
{

public:

	// constructors and destructors
	Landscape(unsigned bits, unsigned width, unsigned height);
	~Landscape() {};

	/*-----------------------------------------------------------------------*
	 * all of these declared as virtual so that they can be overriden in
	 *-----------------------------------------------------------------------*/
	virtual Task				taskAt(unsigned x, unsigned y);
    virtual float               payoffAt(unsigned x, unsigned y);
	virtual void                fluctuate();
    virtual void                deplete_at(unsigned x, unsigned y);
    virtual void                regenerate();
	virtual void                perturb(float perturbance);
    virtual void                distribute_payoffs();
	virtual void                distribute(unsigned heterogeneity);
	virtual void                dump();

	friend ostream & operator<< (ostream &stream, const Landscape &object)
	{
		stream << "[landscape: " << object.mWidth << ", " << object.mHeight << "]";
		return stream;
	}

	vector <vector < Task > > mObjective;
    vector <vector < float > > mPayoff;

	unsigned mWidth, mHeight;
	unsigned mBits;
};

} /* namespace sim */

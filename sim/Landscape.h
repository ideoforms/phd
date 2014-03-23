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
	 * all of these declared as virtual so that they can be overridden
	 *-----------------------------------------------------------------------*/
	virtual TaskVector			tasks_at(unsigned x, unsigned y);
    virtual PayoffVector		payoffs_at(unsigned x, unsigned y);
	virtual void                fluctuate();
    virtual void                deplete_at(unsigned x, unsigned y);
    virtual void                regenerate();
	virtual void                perturb(float perturbance);
    virtual void                distribute_payoffs();
	virtual void                distribute(unsigned heterogeneity);
	virtual void                dump();

    /*-----------------------------------------------------------------------*
	 * Methods to return the next/previous X/Y values, wrapping around grid
	 *-----------------------------------------------------------------------*/
    unsigned                    px(unsigned x) { return (x == 0) ? this->mWidth - 1 : x - 1; }
    unsigned                    nx(unsigned x) { return (x == mWidth - 1) ? 0 : x + 1; }
    unsigned                    py(unsigned y) { return (y == 0) ? this->mHeight - 1 : y - 1; }
    unsigned                    ny(unsigned y) { return (y == mHeight - 1) ? 0 : y + 1; }

	friend ostream & operator<< (ostream &stream, const Landscape &object)
	{
		stream << "[landscape: " << object.mWidth << ", " << object.mHeight << "]";
		return stream;
	}

	vector <vector < TaskVector > > mObjective;
    vector <vector < PayoffVector > > mPayoff;

	unsigned mWidth, mHeight;
	unsigned mBits;
};

} /* namespace sim */

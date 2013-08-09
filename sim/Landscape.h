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

class LandscapeTiled
{

public:

	// constructors and destructors
	LandscapeTiled(unsigned width, unsigned height);
	~LandscapeTiled() {};

	/*-----------------------------------------------------------------------*
	 * all of these declared as virtual so that they can be overriden in
	 *-----------------------------------------------------------------------*/
	virtual Task				taskAt(unsigned x, unsigned y);
	virtual void                fluctuate();
	virtual void                distribute(unsigned correlation);
	virtual void                dump();

	friend ostream & operator<< (ostream &stream, const LandscapeTiled &object)
	{
		stream << "[landscape: " << object.mWidth << ", " << object.mHeight << "]";
		return stream;
	}

	vector <vector < Task > > mGrid;

	unsigned mWidth, mHeight;
};

} /* namespace sim */

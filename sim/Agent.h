/*----------------------------------------------------------------------------*
 * class Agent:
 *   Generic agent type.
 *----------------------------------------------------------------------------*/
 
#pragma once

// using namespace std;

#include "sim/Constants.h"
#include <bitset>

namespace sim
{

using namespace std;

extern struct settings_t settings;

class Environment;

class Agent
{

public:

	// constructors and destructors
	Agent(Environment *env);
	Agent(Agent *parent);
	~Agent() {}

	void 				reset();
	void 				update();
	int					index();

	double				fitness();
	Agent *				replicate();
	void				mutate();

	friend ostream& operator<< (ostream &stream, Agent &object);

protected:

	int					mAge;
	int					mLastAction;

	double				mBEvo,
						mBInd,
						mBSoc;

	double				mOmega;
	double				mDelta;

	Task			 	mGenotype;
	Task			 	mPhenotype;
	Environment 		*mEnv;


	void 				normalize();


	friend class		Environment;

};

} // namespace sim

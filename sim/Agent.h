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
	virtual ~Agent() {}

	virtual void 		reset();
	virtual void 		update();
	virtual void		mutate();
	virtual Agent *		replicate();
    
    Task                learn_ind();
    Task                learn_soc();

	int					get_index() const;
	double				get_fitness() const;


	friend ostream& operator<< (ostream &stream, Agent &object);

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

	double				mMRate; /* [0..1] */
	double				mMCoh;	/* [-1..1] */


	void 				normalize();

};

} // namespace sim

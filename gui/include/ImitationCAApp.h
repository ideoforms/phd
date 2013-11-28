#pragma once

// #define MODEL_ABM

#include "cinder/app/AppCocoaView.h"

#include "sim/Constants.h"
#include "sim/Environment.h"

#include "osc-client.h"

using namespace sim;


class ImitationCAApp : public cinder::app::AppCocoaView
{
  public:
    
	void				setup();
	void                update();
    void				draw();
    
    int                 getAge() { return mEnvironment->mAge; }

	unsigned            mRate;
    
    #ifdef MODEL_ABM
        EnvironmentABM      *mEnvironment;
        OSCClient           *mOSC;
    #else
        EnvironmentCA2D     *mEnvironment;
    #endif
};



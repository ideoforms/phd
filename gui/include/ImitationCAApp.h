#pragma once

#include "cinder/app/AppCocoaView.h"

#include "sim/Constants.h"
#include "sim/Simulation.h"

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
    
    Environment         *mEnvironment;
};



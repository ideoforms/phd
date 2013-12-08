/*----------------------------------------------------------------------------*
 * class Environment:
 *   Generic environment type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Environment.h"

using namespace std;

namespace sim
{

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

} // namespace sim


/*----------------------------------------------------------------------------*
 * class Environment:
 *   Generic environment type.
 *----------------------------------------------------------------------------*/
 
#pragma once

#include "sim/Environment.h"
#include "sim/Graph.h"

using namespace std;
using namespace sim;

namespace sim
{

class EnvironmentGraph : public Environment
{
    
public:
    
    EnvironmentGraph();
    ~EnvironmentGraph();
    
    virtual void                reproduce();
    virtual vector <Agent *> 	get_neighbours(const Agent *agent);
    
    friend class Agent;

	virtual const char * classname() { return "EnvironmentGraph"; }
    
    friend ostream & operator<< (ostream &stream, const EnvironmentGraph &object)
    {
        stream << "[environment-graph: " << "t = " << object.mAge << "]";
        return stream;
    }
    
	Graph						*mGraph;
    
};

} // namespace sim


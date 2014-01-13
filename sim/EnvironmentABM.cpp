/*--------------------------------------------------------------------*
 * class EnvironmentABM
 *--------------------------------------------------------------------*/

#include "sim/Environment.h"

#include <iostream>
#include <math.h>

namespace sim
{

EnvironmentABM::EnvironmentABM() : Environment()
{
	this->mWidth = settings.abm_width_arg;
	this->mHeight = settings.abm_width_arg;

	this->mPositions.resize(settings.popsize_arg);
    this->mVelocity.resize(settings.popsize_arg);

	int n = 0;
	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		this->mPositions[n].set(rng_uniform(this->mWidth), rng_uniform(this->mHeight));
        this->mVelocity[n].set(rng_uniform(-2, 2), rng_uniform(-2, 2));
        
        n++;
	}
}


vector <Agent *> EnvironmentABM::get_neighbours(const Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * return a vector containing the 4 neighbouring agents in the 
	 * von Neumann neighbourhood.
	 *--------------------------------------------------------------------*/

	int index = agent->get_index();
	vector <Agent *> neighbours;

    int neighbourhood_type = (int) settings.abm_neighbourhood_type_arg;
    
    if (neighbourhood_type == NEIGHBOURHOOD_TYPE_METRIC)
    {
        Point2Df thisPosition = mPositions[index];
        
        int popsize = this->get_popsize();
        for (int i = 0; i < popsize; i++)
        {
            Agent *neighbour = mAgents[i];
            Point2Df position = mPositions[i];
            
            if (neighbour == agent) continue;
            if (position.distance(thisPosition) < settings.abm_neighbourhood_size_arg)
            {
                neighbours.push_back(neighbour);
            }
        }
    }
    else if (neighbourhood_type == NEIGHBOURHOOD_TYPE_TOPOLOGICAL)
    {
        // N nearest neighbours
        Point2Df thisPosition = mPositions[index];
        
        vector <float> distances;
        
        int popsize = this->get_popsize();
        for (int i = 0; i < popsize; i++)
        {
            Agent *neighbour = mAgents[i];
            Point2Df position = mPositions[i];
            
            if (neighbour == agent) continue;
            float distance = position.distance(thisPosition);
            
            for (int j = 0; j <= neighbours.size(); j++)
            {
                if (j == neighbours.size() || distance < distances[j])
                {
                    distances.insert(distances.begin() + j, distance);
                    neighbours.insert(neighbours.begin() + j, neighbour);
                    break;
                }
            }
        }
        
        while (neighbours.size() > settings.abm_neighbourhood_size_arg)
            neighbours.pop_back();
        
//        printf("found %ld neighbours\n", neighbours.size());
//        printf("distances:\n");
//        for (int i = 0; i < settings.abm_neighbourhood_size_arg; i++)
//            printf(" - %.3f\n", distances[i]);
    }

	return neighbours;
}

    
    
    
void EnvironmentABM::update()
{
    this->mAge++;
    // printf("age now %d\n", this->mAge);
    if (settings.debug_flag)
        cout << *this << std::endl;
    
    int n = 0;
    for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
    {
        Agent *agent = *it;
        agent->update();
        Point2Df position = mPositions[n];
        Point2Df velocity = mVelocity[n];
        position.x += velocity.x;
        if (position.x <= 0)       position.x += mWidth;
        if (position.x >= mWidth)  position.x -= mWidth;
        if (position.y <= 0)       position.y += mHeight;
        if (position.y >= mHeight) position.y -= mHeight;
        position.y += velocity.y;
        mPositions[n] = position;
        
        // cout << "fitness: " << it->fitness() << std::endl;
        n++;
    }
    
    this->reproduce();
    this->fluctuate();
}


void EnvironmentABM::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	int popsize = this->get_popsize();
	double fitnesses[popsize];
	for (int i = 0; i < popsize; i++)
		fitnesses[i] = mAgents[i]->get_fitness();

	int parent_index = roulette(fitnesses, popsize);
	if (parent_index < 0)
	{
		printf("couldn't find parent with non-zero fitness, selecting random...\n");
		parent_index = rng_randint(popsize);
	}

	Agent *parent = mAgents[parent_index];
	Agent *child = parent->replicate();

	/*
	 * Will this sometimes copy the ptr from the parent object?
	 */
	Point2Df parent_loc = mPositions[parent_index];
	Point2Df child_loc = Point2Df();
	child_loc = parent_loc;

	child_loc.x += rng_uniform(-1, 1);
	child_loc.y += rng_uniform(-1, 1);

	int child_index = rng_randint(popsize);

	// printf("parent: [%.1f, %.1f], index = %d\n", parent_loc.x, parent_loc.y, parent_index);
	// cout << "       - " << *parent << std::endl;
	// printf(" child: [%.1f, %.1f], index = %d\n", child_loc.x, child_loc.y, child_index);
	// cout << "       - " << *child<< std::endl;

	delete mAgents[child_index];

	mAgents[child_index] = child;
}



} /* namespace sim */

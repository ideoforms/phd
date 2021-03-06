/*--------------------------------------------------------------------*
 * class EnvironmentCA1D
 *--------------------------------------------------------------------*/

#include "sim/EnvironmentCA1D.h"

#include <iostream>
#include <math.h>

namespace sim
{

EnvironmentCA1D::EnvironmentCA1D() : Environment()
{
	this->mWidth = settings.popsize_arg;

	this->mLandscape = new Landscape(settings.bits_arg,
                                     this->mWidth / settings.spatial_patch_size_arg, 1);
	this->mLandscape->distribute(settings.spatial_variance_arg);
}


vector <Agent *> EnvironmentCA1D::get_neighbours(const Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * Return a vector containing the 2 neighbouring agents in the 
	 * von Neumann neighbourhood.
	 *--------------------------------------------------------------------*/

	int index = agent->get_index();
	vector <Agent *> neighbours;
	// printf("index %d, position %d, %d\n", index, position.x, position.y);
	// index = rng_randint(mAgents.size());
	neighbours.push_back(this->mAgents[this->px(index)]);
	// index = rng_randint(mAgents.size());
	neighbours.push_back(this->mAgents[this->nx(index)]);

	return neighbours;
}

void EnvironmentCA1D::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	int parent_index = this->select_parent();
	Agent *parent = mAgents[parent_index];
    // printf("selected parent %d, fitness %.3f\n", parent_index, parent->get_fitness());
	Agent *child = parent->replicate();

	int child_index;

	if (settings.ca_non_adjacent_birth_arg)
	{
		child_index = rng_randint(0, this->mWidth);
	}
	else
	{
		int location = rng_randint(2);
		child_index = (location == 0) ?
			this->px(parent_index) :
			this->nx(parent_index);
	}

	// cout << "parent " << parent_index << " " << *parent << " [" << parent << "]" << endl;
	// cout << " -> " << child_index << " " << *child << " [" << child << "]" << endl;

	delete mAgents[child_index];

	mAgents[child_index] = child;
}

TaskVector EnvironmentCA1D::tasks_for(Agent *agent)
{
    /*-----------------------------------------------------------------------*
     * In a well-mixed environment, the goal is the same for every agent.
     * Subclasses should override this with a topography-specific method.
     *-----------------------------------------------------------------------*/
	int index = agent->get_index();
    TaskVector task = this->mLandscape->tasks_at(index, 0);

	// cout << "(agent " << index << ") task at " << position << " is " << task << endl;

	return task;
}


} /* namespace sim */

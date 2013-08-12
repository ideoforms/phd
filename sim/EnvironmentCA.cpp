/*--------------------------------------------------------------------*
 * class EnvironmentCA
 *--------------------------------------------------------------------*/

#include "sim/Environment.h"

#include <iostream>
#include <math.h>

namespace sim
{

EnvironmentCA::EnvironmentCA(unsigned width, unsigned height) : Environment(width * height)
{
	this->mWidth = width;
	this->mHeight = height;

	this->mGrid.resize(width);
	for (int i = 0; i < width; i++)
		this->mGrid[i].resize(height);

	this->mPositions.resize(width * height);

	this->mLandscape = new Landscape(settings.bits_arg,
                                     width / settings.spatial_patch_size_arg,
                                     height / settings.spatial_patch_size_arg);
	this->mLandscape->distribute(settings.spatial_variance_arg);

	int n = 0;
	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = *it;
		int x = n % width;
		int y = (int) n / width;

		this->mGrid[x][y] = agent;
		this->mPositions[n].set(x, y);

		n += 1;
	}
}


vector <Agent *> EnvironmentCA::get_neighbours(const Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * Return a vector containing the 4 neighbouring agents in the 
	 * von Neumann neighbourhood.
	 *--------------------------------------------------------------------*/

	int index = agent->get_index();
	Point2Di position = this->mPositions[index];
	vector <Agent *> neighbours;
	// printf("index %d, position %d, %d\n", index, position.x, position.y);
	neighbours.push_back(this->mGrid[this->px(position.x)][position.y]);
	neighbours.push_back(this->mGrid[this->nx(position.x)][position.y]);
	neighbours.push_back(this->mGrid[position.x][this->py(position.y)]);
	neighbours.push_back(this->mGrid[position.x][this->ny(position.y)]);

	return neighbours;
}

void EnvironmentCA::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	int popsize = this->get_popsize();
	double fitnesses[popsize];
	for (int i = 0; i < popsize; i++)
		fitnesses[i] = mAgents[i]->get_fitness();

	int parent_index = roulette(fitnesses, popsize);

	Agent *parent = mAgents[parent_index];
	Agent *child = parent->replicate();

	Point2Di parent_loc = mPositions[parent_index];
	Point2Di child_loc = parent_loc;
	int location = rng_randint(4);
	switch (location)
	{
		case 0: child_loc.x = this->px(child_loc.x); break;
		case 1: child_loc.x = this->nx(child_loc.x); break;
		case 2: child_loc.y = this->py(child_loc.y); break;
		case 3: child_loc.y = this->ny(child_loc.y); break;
	}
	int child_index = this->mWidth * child_loc.y + child_loc.x;

	// cout << "parent " << parent_index << " (" << parent_loc.x << ", " << parent_loc.y << "): " << *parent << " [" << parent << "]" << endl;
	// cout << " -> " << child_index << " (" << child_loc.x << ", " << child_loc.y << "): " << *child << " [" << child << "]" << endl;

	delete mAgents[child_index];

	mAgents[child_index] = child;
	mGrid[child_loc.x][child_loc.y] = child;
}

Task EnvironmentCA::goal_for(Agent *agent)
{
    /*-----------------------------------------------------------------------*
     * In a well-mixed environment, the goal is the same for every agent.
     * Subclasses should override this with a topography-specific method.
     *-----------------------------------------------------------------------*/
	int index = agent->get_index();
	Point2Di position = this->mPositions[index];
    Task task = this->mLandscape->taskAt(position.x, position.y);

	// cout << "(agent " << index << ") task at " << position << " is " << task << endl;

	return task;
}


} /* namespace sim */

/*--------------------------------------------------------------------*
 * class EnvironmentCA2D
 *--------------------------------------------------------------------*/

#include "sim/EnvironmentCA2D.h"

#include <iostream>
#include <math.h>

namespace sim
{

EnvironmentCA2D::EnvironmentCA2D() : Environment()
{
	this->mWidth = settings.ca_width_arg;
	this->mHeight = settings.ca_width_arg;

	// Slight hack to update popsize based on the CA width and height (as it is fixed)
	assert(settings.popsize_arg == settings.ca_width_arg * settings.ca_width_arg);

	this->mGrid.resize(this->mWidth);
	for (int i = 0; i < this->mWidth; i++)
		this->mGrid[i].resize(this->mHeight);

	this->mPositions.resize(this->mWidth * this->mHeight);

	this->mLandscape = new Landscape(settings.bits_arg,
                                     this->mWidth / settings.spatial_patch_size_arg,
                                     this->mHeight / settings.spatial_patch_size_arg);
	this->mLandscape->distribute(settings.spatial_variance_arg);

	int n = 0;
	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = *it;
		int x = n % this->mWidth;
		int y = (int) n / this->mHeight;

		this->mGrid[x][y] = agent;
		this->mPositions[n].set(x, y);

		n += 1;
	}
}


vector <Agent *> EnvironmentCA2D::get_neighbours(const Agent *agent)
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

void EnvironmentCA2D::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	int parent_index = this->select_parent();
	Agent *parent = mAgents[parent_index];
	Agent *child = parent->replicate();

	/*--------------------------------------------------------------------*
	 * If we are using non-adjacent reproduction, pick a random site to
	 * produce our child; otherwise, pick a site next to the parent.
	 *--------------------------------------------------------------------*/
	int child_index;
	Point2Di child_loc;
	Point2Di parent_loc = mPositions[parent_index];

	if (!settings.ca_non_adjacent_birth_flag)
	{
		child_loc = parent_loc;
		int location = rng_randint(4);
		switch (location)
		{
			case 0: child_loc.x = this->px(child_loc.x); break;
			case 1: child_loc.x = this->nx(child_loc.x); break;
			case 2: child_loc.y = this->py(child_loc.y); break;
			case 3: child_loc.y = this->ny(child_loc.y); break;
		}
		child_index = this->mWidth * child_loc.y + child_loc.x;
	}
	else
	{
		int popsize = this->get_popsize();
		child_index = rng_randint(popsize);
		child_loc = mPositions[child_index];
	}

	// cout << "parent " << parent_index << " (" << parent_loc.x << ", " << parent_loc.y << "): " << *parent << " [" << parent << "]" << endl;
	// cout << " -> " << child_index << " (" << child_loc.x << ", " << child_loc.y << "): " << *child << " [" << child << "]" << endl;

	delete mAgents[child_index];

	mAgents[child_index] = child;
	mGrid[child_loc.x][child_loc.y] = child;
}

Task EnvironmentCA2D::goal_for(Agent *agent)
{
    /*-----------------------------------------------------------------------*
     * In a well-mixed environment, the goal is the same for every agent.
     * Subclasses should override this with a topography-specific method.
     *-----------------------------------------------------------------------*/
	int index = agent->get_index();
	Point2Di position = this->mPositions[index];
    Task task = this->mLandscape->taskAt(position.x, position.y);

	return task;
}


} /* namespace sim */

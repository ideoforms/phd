/*--------------------------------------------------------------------*
 * class EnvironmentCA2DMulti
 *--------------------------------------------------------------------*/

#include "sim/EnvironmentCA2DMulti.h"

#include <iostream>
#include <math.h>

namespace sim
{

EnvironmentCA2DMulti::EnvironmentCA2DMulti() : Environment()
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

		this->mGrid[x][y].push_back(agent);
		this->mPositions[n].set(x, y);

		n += 1;
	}
}


vector <Agent *> EnvironmentCA2DMulti::get_neighbours(const Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * Return a vector containing the 4 neighbouring agents in the 
	 * von Neumann neighbourhood.
	 *--------------------------------------------------------------------*/

	int index = agent->get_index();
	Point2Di position = this->mPositions[index];
	vector <Agent *> neighbours;
	neighbours.reserve(8);
	// TODO: remove self from neighbours list
	neighbours.insert(neighbours.end(), this->mGrid[position.x][position.y].begin(), this->mGrid[position.x][position.y].end());
	neighbours.insert(neighbours.end(), this->mGrid[this->px(position.x)][position.y].begin(), this->mGrid[this->px(position.x)][position.y].end());
	neighbours.insert(neighbours.end(), this->mGrid[this->nx(position.x)][position.y].begin(), this->mGrid[this->nx(position.x)][position.y].end());
	neighbours.insert(neighbours.end(), this->mGrid[position.x][this->py(position.y)].begin(), this->mGrid[position.x][this->py(position.y)].end());
	neighbours.insert(neighbours.end(), this->mGrid[position.x][this->ny(position.y)].begin(), this->mGrid[position.x][this->ny(position.y)].end());

	return neighbours;
}

void EnvironmentCA2DMulti::reproduce()
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

	/*--------------------------------------------------------------------*
	 * If we are using non-adjacent reproduction, pick a random site to
	 * produce our child; otherwise, pick a site next to the parent.
	 *--------------------------------------------------------------------*/
	Point2Di child_loc;
	Point2Di parent_loc = mPositions[parent_index];

	assert(!settings.ca_non_adjacent_birth_flag);

		child_loc = parent_loc;
		int location = rng_randint(4);
		switch (location)
		{
			case 0: child_loc.x = this->px(child_loc.x); break;
			case 1: child_loc.x = this->nx(child_loc.x); break;
			case 2: child_loc.y = this->py(child_loc.y); break;
			case 3: child_loc.y = this->ny(child_loc.y); break;
		}

	int dead_index = rng_randint(popsize);

	// cout << "parent " << parent_index << " (" << parent_loc.x << ", " << parent_loc.y << "): " << *parent << " [" << parent << "]" << endl;
	// cout << " -> " << dead_index << " (" << child_loc.x << ", " << child_loc.y << "): " << *child << " [" << child << "]" << endl;

	Agent *dead_agent = mAgents[dead_index];
	Point2Di dead_loc = mPositions[dead_index];
	// HOW DO WE PROPERLY GET A REFERENCE?
	// This assigns by value so we are modifying a copy. Hmm..
	// vector <Agent *> dead_cell = &mGrid[dead_loc.x][dead_loc.y];
	std::vector<Agent *>::iterator position = std::find(mGrid[dead_loc.x][dead_loc.y].begin(), mGrid[dead_loc.x][dead_loc.y].end(), dead_agent);
	// printf("index %d, agents %d\n", position - mGrid[dead_loc.x][dead_loc.y].begin(), mGrid[dead_loc.x][dead_loc.y].size());
	assert(position != mGrid[dead_loc.x][dead_loc.y].end());
    mGrid[dead_loc.x][dead_loc.y].erase(position);
	delete mAgents[dead_index];

	mAgents[dead_index] = child;
	mGrid[child_loc.x][child_loc.y].push_back(child);
	mPositions[dead_index] = child_loc;
	// printf("cell now has %d agents\n", mGrid[child_loc.x][child_loc.y].size());
}

Task EnvironmentCA2DMulti::goal_for(Agent *agent)
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

double EnvironmentCA2DMulti::payoff(Agent *agent, Task phenotype)
{
    double payoff = Environment::payoff(agent, phenotype);

    if (settings.frequency_inverse_payoff_flag)
    {
        int index = agent->get_index();
        Point2Di position = this->mPositions[index];
        vector <Agent *> cohabitors = this->mGrid[position.x][position.y];
        payoff /= cohabitors.size();
    }
    
    return payoff;
}


} /* namespace sim */

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
	// assert(settings.popsize_arg == settings.ca_width_arg * settings.ca_width_arg);

	this->mGrid.resize(this->mWidth);
	for (int i = 0; i < this->mWidth; i++)
		this->mGrid[i].resize(this->mHeight);

	this->mPositions.resize(settings.popsize_arg);

	this->mLandscape = new Landscape(settings.bits_arg,
                                     this->mWidth / settings.spatial_patch_size_arg,
                                     this->mHeight / settings.spatial_patch_size_arg);
	this->mLandscape->distribute(settings.spatial_variance_arg);

	int n = 0;
    
    // now set initial positions randomly
	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = *it;
        
		// int x = n % this->mWidth;
		// int y = (int) n / this->mHeight;

        int x = rng_randint(0, this->mWidth);
        int y = rng_randint(0, this->mHeight);
        
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
    
void EnvironmentCA2DMulti::update()
{
    Environment::update();
    
    if (settings.payoff_depletion_rate_arg)
    {
        for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
        {
            Agent *agent = *it;
            int index = agent->get_index();
            Point2Di position = this->mPositions[index];
            this->mLandscape->deplete_at(position.x, position.y);
        }
    }

    this->mLandscape->regenerate();
}

void EnvironmentCA2DMulti::move(Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * move an agent randomly to a neighbouring cell
	 *--------------------------------------------------------------------*/
	int direction;
    int index = agent->get_index();
    Point2Di position = this->mPositions[index];

	if (settings.movement_cohesion_genetic_arg)
	{
		/*------------------------------------------------------------------------*
		 * calculate neighbour counts (as doubles for roulette wheel algorithm)
		 *------------------------------------------------------------------------*/
		double neighbour_counts[4];
		neighbour_counts[0] = this->mGrid[this->px(position.x)][position.y].size();
		neighbour_counts[1] = this->mGrid[this->nx(position.x)][position.y].size();
		neighbour_counts[2] = this->mGrid[position.x][this->py(position.y)].size();
		neighbour_counts[3] = this->mGrid[position.x][this->ny(position.y)].size();
        // neighbour_counts[4] = this->mGrid[position.x][position.y].size();

		/*------------------------------------------------------------------------*
		 * weight our selection 
		 *------------------------------------------------------------------------*/
		direction = wroulette(neighbour_counts, 4, agent->mMCoh);
		// printf("neighbour counts %.0f, %.0f, %.0f, %.0f, selected %.0f\n", neighbour_counts[0], neighbour_counts[1], neighbour_counts[2], neighbour_counts[3], neighbour_counts[direction]);
	}
	else
	{
		direction = rng_randint(4);
	}

    /*--------------------------------------------------------------------*
	 * remove agent from the grid
	 *--------------------------------------------------------------------*/
    std::vector<Agent *>::iterator offset = std::find(mGrid[position.x][position.y].begin(), mGrid[position.x][position.y].end(), agent);
	assert(offset != mGrid[position.x][position.y].end());
    mGrid[position.x][position.y].erase(offset);
    
    switch (direction)
    {
        case 0:
            this->mPositions[index].x = this->px(this->mPositions[index].x); break;
        case 1:
            this->mPositions[index].x = this->nx(this->mPositions[index].x); break;
        case 2:
            this->mPositions[index].y = this->py(this->mPositions[index].y); break;
        case 3:
            this->mPositions[index].y = this->ny(this->mPositions[index].y); break;
        case 4:
            // stay where we are
            break;
    }
    // printf("position: (%d, %d)\n", this->mPositions[index].x, this->mPositions[index].y);
    
    /*--------------------------------------------------------------------*
	 * re-add agent to grid in new position
	 *--------------------------------------------------------------------*/
    position = this->mPositions[index];
    mGrid[position.x][position.y].push_back(agent);
}

void EnvironmentCA2DMulti::reproduce()
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
	Point2Di child_loc;
	Point2Di parent_loc = mPositions[parent_index];

	assert(!settings.ca_non_adjacent_birth_arg);

	child_loc = parent_loc;
	if (!settings.ca_colocated_birth_arg)
	{
		int location = rng_randint(4);
		switch (location)
		{
			case 0: child_loc.x = this->px(child_loc.x); break;
			case 1: child_loc.x = this->nx(child_loc.x); break;
			case 2: child_loc.y = this->py(child_loc.y); break;
			case 3: child_loc.y = this->ny(child_loc.y); break;
		}
	}

	int popsize = this->get_popsize();
	int dead_index = rng_randint(popsize);

	// cout << "parent " << parent_index << " (" << parent_loc.x << ", " << parent_loc.y << "): " << *parent << " [" << parent << "]" << endl;
	// cout << " -> " << dead_index << " (" << child_loc.x << ", " << child_loc.y << "): " << *child << " [" << child << "]" << endl;

	Agent *dead_agent = mAgents[dead_index];
	Point2Di dead_loc = mPositions[dead_index];

	// HOW DO WE PROPERLY GET A REFERENCE?
	// This assigns by value so we are modifying a copy. Hmm..
	// vector <Agent *> dead_cell = &mGrid[dead_loc.x][dead_loc.y];
	std::vector<Agent *>::iterator position = std::find(mGrid[dead_loc.x][dead_loc.y].begin(), mGrid[dead_loc.x][dead_loc.y].end(), dead_agent);
	assert(position != mGrid[dead_loc.x][dead_loc.y].end());
    mGrid[dead_loc.x][dead_loc.y].erase(position);

	delete mAgents[dead_index];

	mAgents[dead_index] = child;
	mGrid[child_loc.x][child_loc.y].push_back(child);
	mPositions[dead_index] = child_loc;
	// printf("cell now has %d agents\n", mGrid[child_loc.x][child_loc.y].size());
}

PayoffVector EnvironmentCA2DMulti::payoffs_for(Agent *agent)
{
	int index = agent->get_index();
	Point2Di position = this->mPositions[index];
    PayoffVector payoffs = this->mLandscape->payoffs_at(position.x, position.y);
    return payoffs;
}

TaskVector EnvironmentCA2DMulti::tasks_for(Agent *agent)
{
	int index = agent->get_index();
	Point2Di position = this->mPositions[index];
    TaskVector tasks = this->mLandscape->tasks_at(position.x, position.y);
	return tasks;
}

double EnvironmentCA2DMulti::payoff(Agent *agent, TaskVector phenotype)
{
    double payoff = Environment::payoff(agent, phenotype);
    
    int index = agent->get_index();
    Point2Di position = this->mPositions[index];
    
    if (settings.frequency_inverse_payoff_arg)
    {
        vector <Agent *> cohabitors = this->mGrid[position.x][position.y];
        payoff /= cohabitors.size();
    }
    
    // payoff = payoff * mLandscape->payoffAt(position.x, position.y);
    
    return payoff;
}

stats_t EnvironmentCA2DMulti::stats()
{
	stats_t stats = Environment::stats();

	if (settings.log_dispersion_flag)
	{
		double dispersion = 0.0;
		double N = settings.popsize_arg;
		double NN = N * (N - 1);

		for (int x = 0; x < this->mWidth; x++)
		for (int y = 0; y < this->mWidth; y++)
		{
			int count = this->mGrid[x][y].size();
			dispersion += (double) (count * (count - 1)) / NN;
		}

		double q = this->mWidth * this->mWidth;
		stats.dispersion = dispersion * q;
	}

	return stats;
}


} /* namespace sim */

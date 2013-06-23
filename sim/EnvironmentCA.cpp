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

	int n = 0;
	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = &(*it);
		int x = n % width;
		int y = (int) n / width;

		this->mGrid[x][y] = agent;
		this->mPositions[n].set(x, y);

		n += 1;
	}
}


vector <Agent *> EnvironmentCA::get_neighbours(Agent &agent)
{
	/*--------------------------------------------------------------------*
	 * return a vector containing the 4 neighbouring agents in the 
	 * von Neumann neighbourhood.
	 *--------------------------------------------------------------------*/

	int index = agent.get_index();
	Point2Di position = this->mPositions[index];
	vector <Agent *> neighbours;
	printf("index %d, position %d, %d\n", index, position.x, position.y);
	neighbours.push_back(this->mGrid[this->px(position.x)][this->py(position.y)]);
	neighbours.push_back(this->mGrid[this->nx(position.x)][this->py(position.y)]);
	neighbours.push_back(this->mGrid[this->px(position.x)][this->ny(position.y)]);
	neighbours.push_back(this->mGrid[this->nx(position.x)][this->ny(position.y)]);

	return neighbours;
}

//Agent EnvironmentCA::agent_at(Point2Di &point)
//{
//	return *(this->mGrid[point.x][point.y]);
//}

stats_t EnvironmentCA::stats()
{
	/*--------------------------------------------------------------------*
	 * calculate and return the current environmental statistics:
	 *  - minimum, maximum and mean fitness
	 *  - mean values for each of the b_evo, b_ind and b_soc traits.
	 *--------------------------------------------------------------------*/
	stats_t 	stats;

	int 		popsize = this->get_popsize();

	double 		min_fitness = INT_MAX;
	double 		max_fitness = 0.0;
	double 		total_fitness = 0;
	double 		total_bevo = 0;
	double 		total_bind = 0;
	double 		total_bsoc = 0;

	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		double fitness   = it->fitness();
		if (fitness > max_fitness) max_fitness = fitness;
		if (fitness < min_fitness) min_fitness = fitness;

		total_fitness	+= fitness;
		total_bevo		+= it->mBEvo;
		total_bind		+= it->mBInd;
		total_bsoc		+= it->mBSoc;
	}

	stats.fitness_min	= min_fitness;
	stats.fitness_max	= max_fitness;
	stats.fitness_mean	= total_fitness / popsize;

	stats.bevo_mean		= total_bevo / popsize;
	stats.bind_mean		= total_bind / popsize;
	stats.bsoc_mean		= total_bsoc / popsize;

	return stats;
}

} // namespace sim


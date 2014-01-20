/*--------------------------------------------------------------------*
 * class EnvironmentGraph
 *--------------------------------------------------------------------*/

#include "sim/EnvironmentGraph.h"

#include <iostream>
#include <math.h>

namespace sim
{

EnvironmentGraph::EnvironmentGraph() : Environment()
{
	int N = settings.popsize_arg;
	int K = settings.graph_degree_arg;
	this->mGraph = new Graph();
	this->mGraph->make_regular(N, K);
	for (int i = 0; i < N; i++)
		this->mGraph->mNodes[i]->mData = this->mAgents[i];
}

vector <Agent *> EnvironmentGraph::get_neighbours(const Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * Return a vector containing the 2 neighbouring agents in the 
	 * von Neumann neighbourhood.
	 *--------------------------------------------------------------------*/

	int index = agent->get_index();
	vector <Agent *> neighbours;
	neighbours.reserve(settings.graph_degree_arg);

	vector <Node *> neighbour_nodes = this->mGraph->mNodes[index]->mNeighbours;
	for (Node *node : neighbour_nodes)
		neighbours.push_back((Agent *) node->mData);

	return neighbours;
}

void EnvironmentGraph::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	int parent_index = this->select_parent();
	Agent *parent = mAgents[parent_index];
	Agent *child = parent->replicate();

	vector <Agent *> neighbours = this->get_neighbours(parent);
	Agent *neighbour = neighbours[rng_randint(0, neighbours.size())];
	int child_index = neighbour->get_index();

	delete mAgents[child_index];
	mAgents[child_index] = child;
	mGraph->mNodes[child_index]->mData = child;
}

} /* namespace sim */


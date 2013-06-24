/*--------------------------------------------------------------------*
 * class Environment
 *--------------------------------------------------------------------*/

#include "sim/Environment.h"

#include <iostream>
#include <math.h>

namespace sim
{

Environment::Environment(int population)
{
	this->mTask = Task(settings.bits_arg).set();
	this->mSwitchIndex = 0;
	this->mAge = 0;

	cout << "bits is " << settings.bits_arg << std::endl;

	printf("creating %d agents\n", population);
	for (int i = 0; i < population; i++)
	{
		Agent *agent = new Agent(this);
		mAgents.push_back(agent);
	}
}


void Environment::update()
{
	this->mAge++;
	if (settings.debug_given)
		cout << *this << std::endl;

	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = *it;
		agent->update();
		// cout << "fitness: " << it->fitness() << std::endl;
	}

	this->reproduce();
	this->fluctuate();
}

void Environment::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	int popsize = this->get_popsize();
	if (!settings.metabolism_given)
	{
		double fitnesses[popsize];
		for (int i = 0; i < popsize; i++)
			fitnesses[i] = mAgents[i]->get_fitness();

		int parent_index = roulette(fitnesses, popsize);
		int child_index = rng_randint(popsize);
		// cout << "replacing " << child_index << " with " << parent_index << "( fitness = " << mAgents[parent_index].fitness() << ")" << std::endl;
		Agent *parent = mAgents[parent_index];
		Agent *child = parent->replicate();

		// TODO: check if we're handling memory OK here
		delete mAgents[child_index];
		mAgents[child_index] = child;
	}
	/*--------------------------------------------------------------------*
	 * metabolic reproduction
	 *--------------------------------------------------------------------*/
	else
	{
		int n = 0;
		for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it, ++n)
		{
			Agent *agent = *it;
			if (agent->mOmega > 2 * settings.omega0_arg)
			{
				// cout << "agent " << n << " metabolism = " << it->mOmega << ", reproducing" << endl;
				agent->mOmega -= settings.omega0_arg;

				int child_index = rng_randint(popsize);
				Agent *child = agent->replicate();

				delete mAgents[child_index];
				mAgents[child_index] = child;
			}
		}
	}
}

void Environment::fluctuate()
{
	/*--------------------------------------------------------------------*
	 * fluctuations
	 *--------------------------------------------------------------------*/
	if (rng_coin(settings.p_switch_arg))
	{
		mTask.flip(mSwitchIndex);
		mSwitchIndex = (mSwitchIndex + 1) % settings.bits_arg;
	}
}

void Environment::perturb(double amount)
{
	/*-----------------------------------------------------------------------*
	 * cause a sudden perturbation in the environment, flipping each bit
	 * with a probability of _amount_
	 *-----------------------------------------------------------------------*/
	for (int i = 0; i < settings.bits_arg; i++)
	{
		if (rng_coin(amount))
		{
			mTask.flip(i);
		}
	}
}


double Environment::payoff(Task phenotype)
{
	/*-----------------------------------------------------------------------*
	 * returns the fitness for a given phenotype.
	 * alpha_arg is used for a rapid falloff from peak, to create a sharper
	 * fitness differential.
	 *-----------------------------------------------------------------------*/
	double distance = (double) (phenotype ^ mTask).count() / settings.bits_arg;
	double proximity = 1.0 - distance;
	double fitness = pow(proximity, 1.0 / settings.alpha_arg);

	// cout << "task = " << task << ", distance = " << distance << ", proximity = " << proximity << ", fitness = " << fitness << std::endl;
	return fitness;
}

int Environment::get_popsize()
{
	return this->mAgents.size();
}

vector <Agent *> Environment::get_neighbours(const Agent *agent)
{
	/*--------------------------------------------------------------------*
	 * at present, simply returns a vector containing all agents.
	 *--------------------------------------------------------------------*/

	vector <Agent *> neighbours;
	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = *it;
		neighbours.push_back(agent);
	}

	return neighbours;
}

stats_t Environment::stats()
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
		Agent *agent = *it;
		double fitness   = agent->get_fitness();
		if (fitness > max_fitness) max_fitness = fitness;
		if (fitness < min_fitness) min_fitness = fitness;

		total_fitness	+= fitness;
		total_bevo		+= agent->mBEvo;
		total_bind		+= agent->mBInd;
		total_bsoc		+= agent->mBSoc;
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


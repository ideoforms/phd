/*--------------------------------------------------------------------*
 * class Environment
 *--------------------------------------------------------------------*/

#include "sim/Environment.h"

#include <iostream>
#include <math.h>

namespace sim
{

Environment::Environment()
{
	this->mAge = 0;

    assert (settings.bits_arg > 0);

	/*--------------------------------------------------------------------*
	 * For a well-mixed case, create a Landscape comprising of one cell.
	 *--------------------------------------------------------------------*/
	this->mLandscape = new Landscape(settings.bits_arg, 1, 1);

	int population = settings.popsize_arg;
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
    // printf("age now %d\n", this->mAge);
	if (settings.debug_flag)
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

int Environment::select_parent()
{
	int parent_index;
	int popsize = this->get_popsize();

	double fitnesses[popsize];
	for (int i = 0; i < popsize; i++)
		fitnesses[i] = mAgents[i]->get_fitness();

	parent_index = roulette(fitnesses, popsize);
	if (parent_index < 0)
	{
		printf("couldn't find parent with non-zero fitness, selecting random...\n");
		parent_index = rng_randint(popsize);
	}

	return parent_index;
}

void Environment::reproduce()
{
	/*--------------------------------------------------------------------*
	 * birth-death reproduction
	 *--------------------------------------------------------------------*/
	for (int i = 0; i < settings.reproduction_count_arg; i++)
	{
		int popsize = this->get_popsize();
		if (!settings.metabolism_flag)
		{
			int parent_index = this->select_parent();
			int child_index = rng_randint(popsize);
			// cout << "replacing " << child_index << " with " << parent_index << "( fitness = " << mAgents[parent_index]->get_fitness() << ")" << std::endl;

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
}

void Environment::fluctuate()
{
	/*--------------------------------------------------------------------*
	 * fluctuations
	 *--------------------------------------------------------------------*/
	if (rng_coin(settings.p_switch_arg))
	{
		this->mLandscape->fluctuate();
		// printf("-------------------- fluctuate ----------------------\n");
		// this->mLandscape->dump();
	}
}

void Environment::perturb(double amount)
{
	/*-----------------------------------------------------------------------*
	 * cause a sudden perturbation in the environment, flipping each bit
	 * with a probability of _amount_
	 *-----------------------------------------------------------------------*/
	this->mLandscape->perturb(amount);
}


Task Environment::goal_for(Agent *agent)
{
	/*-----------------------------------------------------------------------*
	 * In a well-mixed environment, the goal is the same for every agent.
	 * Subclasses should override this with a topography-specific method.
	 *-----------------------------------------------------------------------*/
	return this->mLandscape->taskAt(0, 0);
}
    
void Environment::move(Agent *agent)
{
    /*-----------------------------------------------------------------------*
	 * Movement is meaningless in this environment.
	 *-----------------------------------------------------------------------*/
    return;
}


double Environment::payoff(Agent *agent, Task phenotype)
{
	/*-----------------------------------------------------------------------*
	 * returns the fitness for a given phenotype.
	 * alpha_arg is used for a rapid falloff from peak, to create a sharper
	 * fitness differential.
	 *-----------------------------------------------------------------------*/
	Task goal = this->goal_for(agent);
	double distance = (double) (phenotype ^ goal).count() / settings.bits_arg;
	double proximity = 1.0 - distance;
	double fitness = pow(proximity, 1.0 / settings.alpha_arg);

	/*-----------------------------------------------------------------------*
	 * If we are using a bimodal distribution, create a secondary peak.
	 *-----------------------------------------------------------------------*/
	if (settings.fitness_objective_bimodal_arg)
	{
		Task mask = Task(settings.bits_arg, pow(2, settings.bits_arg / 2) - 1);
		Task secondary = goal ^ mask;
		// cout << "task = " << goal << ", secondary = " << secondary << std::endl;
		proximity = 1.0 - (phenotype ^ goal).count() / settings.bits_arg;
		double fitness_secondary = pow(proximity, 1.0 / settings.alpha_arg);
		fitness_secondary *= settings.fitness_objective_bimodal_arg;
		fitness = max(fitness, fitness_secondary);
	}

	// cout << "task = " << goal << ", distance = " << distance << ", proximity = " << proximity << ", fitness = " << fitness << std::endl;
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

    // printf("USING BASIC GET_NEIGHBOURS\n");

	int neighbourhood_size = settings.neighbourhood_size_arg;
	vector <Agent *> neighbours;
	neighbours.reserve(this->get_popsize());

	if (neighbourhood_size == 0)
	{
		for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
		{
			Agent *other = *it;
			if (agent != other)
				neighbours.push_back(other);
		}
	}
	else
	{
		while (neighbours.size() < neighbourhood_size)
		{
			int index = rng_randint(mAgents.size());
			Agent *other = mAgents[index];
			if (other == agent)
				continue;
			/*--------------------------------------------------------------------*
			 * Make sure each agent is only in our neighbourhood group once.
			 * This is probably very costly! can we omit (and just risk including
			 * a neighbour twice?)
			 *--------------------------------------------------------------------*/
			if (std::find(neighbours.begin(), neighbours.end(), other) != neighbours.end())
				continue;
			neighbours.push_back(other);
		}
	}

    // printf("done (got %d neighbours)\n", neighbours.size());

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
	double 		total_age = 0;
	double 		total_bevo = 0;
	double 		total_bind = 0;
	double 		total_bsoc = 0;

	double 		total_mrate = 0;
	double 		total_mcoh = 0;

	double		total_geno_dist = 0.0;
	double		total_pheno_dist = 0.0;

	for (agent_iterator it = mAgents.begin(); it != mAgents.end(); ++it)
	{
		Agent *agent = *it;
		double fitness   = agent->get_fitness();
		if (fitness > max_fitness) max_fitness = fitness;
		if (fitness < min_fitness) min_fitness = fitness;

		total_fitness	+= fitness;
		total_age		+= agent->mAge;
		total_bevo		+= agent->mBEvo;
		total_bind		+= agent->mBInd;
		total_bsoc		+= agent->mBSoc;

		total_mrate		+= agent->mMRate;
		total_mcoh		+= agent->mMCoh;

		Task task = this->goal_for(agent);
		total_geno_dist += (task ^ agent->mGenotype).count();
		total_pheno_dist += (task ^ agent->mPhenotype).count();
		// cout << "Phenotype " << agent->mPhenotype << ", task " << task << ", distance: " << pheno_dist << endl;
	}

	stats.step          = this->mAge;

	stats.fitness_min	= min_fitness;
	stats.fitness_max	= max_fitness;
	stats.fitness_mean	= total_fitness / popsize;

	stats.age_mean		= total_age / popsize;

	stats.bevo_mean		= total_bevo / popsize;
	stats.bind_mean		= total_bind / popsize;
	stats.bsoc_mean		= total_bsoc / popsize;

	stats.mrate_mean	= total_mrate / popsize;
	stats.mcoh_mean		= total_mcoh / popsize;

	stats.geno_mean_dist = total_geno_dist /= (settings.bits_arg * mAgents.size());
	stats.pheno_mean_dist = total_pheno_dist /= (settings.bits_arg * mAgents.size());

	return stats;
}

} // namespace sim


/*--------------------------------------------------------------------*
 * class Agent
 *  - represents a single agent object
 *--------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>

#include "sim/Agent.h"
#include "sim/Environment.h"
#include "sim/Util.h"


namespace sim
{

extern struct settings_t settings;

Agent::Agent(Environment *env)
{
	this->mEnv = env;

	this->mGenotype = TaskVector();
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
		this->mGenotype.push_back(Task(settings.bits_arg));

	/*--------------------------------------------------------------------*
	 * Option: Set precisely N bits to 1
	 *--------------------------------------------------------------------*/
	if (settings.initial_geno_bits_given)
	{
		for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
		{
			this->mGenotype[taskIndex].reset();

			/*--------------------------------------------------------------------*
			 * Set first bits to 1, then shuffle whole genome
			 * so that exactly initial-geno-bits are set to 1
			 *--------------------------------------------------------------------*/
			this->mGenotype[taskIndex] |= Task(settings.bits_arg, pow(2, (int) (settings.bits_arg * settings.initial_geno_bits_arg)) - 1);
			if (settings.initial_geno_bits_arg == 1)
				this->mGenotype[taskIndex].set();

			for (int i = settings.bits_arg - 1; i >= 0; i--)
			{
				int j = rng_randint(i + 1);
				int tmp = this->mGenotype[taskIndex][i];
				this->mGenotype[taskIndex][i] = this->mGenotype[taskIndex][j];
				this->mGenotype[taskIndex][j] = tmp;
			}
		}
	}
	/*--------------------------------------------------------------------*
	 * Default: Set bits to uniform random value
	 *--------------------------------------------------------------------*/
	else
	{
		for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
			for (int i = 0; i < settings.bits_arg; i++)
				this->mGenotype[taskIndex][i] = rng_coin(0.5);
	}

	this->mPhenotype = TaskVector();
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
		this->mPhenotype.push_back(Task(settings.bits_arg));

	this->mBEvo = settings.initial_b_evo_given ? settings.initial_b_evo_arg : rng_uniformuf();
	this->mBInd = settings.initial_b_ind_given ? settings.initial_b_ind_arg : rng_uniformuf();
	this->mBSoc = settings.initial_b_soc_given ? settings.initial_b_soc_arg : rng_uniformuf();

	this->mMRate = rng_uniformuf();
	this->mMCoh = rng_uniform(-1, 1);

	this->normalize();

	this->reset();
}

Agent::Agent(Agent *parent)
{
	// not sure if this is the best way to clone -- should maybe
	// call normal constructor and then set constants?
	this->mEnv = parent->mEnv;

	// IMPORTANT: Must initialise an object or we risk making a copy of the parent's geno (ptr)!
	// Still confused by this ...
    this->mGenotype = TaskVector();
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	{
		this->mGenotype.push_back(Task(settings.bits_arg));
		this->mGenotype[taskIndex] = parent->mGenotype[taskIndex];
	}
	// printf("created new agent with %d bits (parent has %d)\n", this->mGenotype.size(), parent->mGenotype.size());
	// printf("(my geno ptr = %p, parent geno ptr = %p)\n", &this->mGenotype, &parent->mGenotype);

    this->mPhenotype = TaskVector();
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
		this->mPhenotype.push_back(Task(settings.bits_arg));

	this->mBEvo = parent->mBEvo;
	this->mBInd = parent->mBInd;
	this->mBSoc = parent->mBSoc;

	this->mMRate = parent->mMRate;
	this->mMCoh = parent->mMCoh;

	if (settings.metabolism_flag)
		this->mOmega = settings.omega0_arg;

	this->normalize();

	this->reset();
}

void Agent::reset()
{
	/*--------------------------------------------------------------------*
	 * set initial delta to 1, as we want all agents to look equally fit
	 * on their first timestep.
	 *--------------------------------------------------------------------*/
	this->mPhenotype = this->mGenotype;
	this->mAge = 0;
	this->mDelta = 0;
	this->mLastAction = 0;
}

int Agent::get_index() const
{
	for (unsigned int i = 0; i < mEnv->mAgents.size(); i++)
	{
		if (mEnv->mAgents[i] == this)
			return i;
	}

	return -1;
}

void Agent::normalize()
{
	/*------------------------------------------------------------------------
	 * If certain behavioural modes are suppressed, zero them out.
	 *-----------------------------------------------------------------------*/
	if (settings.suppress_b_evo_arg)
		this->mBEvo = 0;
	else if (settings.fixed_b_evo_given)
		this->mBEvo = settings.fixed_b_evo_arg;

	if (settings.suppress_b_ind_arg)
		this->mBInd = 0;
	else if (settings.fixed_b_ind_given)
		this->mBInd = settings.fixed_b_ind_arg;

	if (settings.suppress_b_soc_arg)
		this->mBSoc = 0;
	else if (settings.fixed_b_soc_given)
		this->mBSoc = settings.fixed_b_soc_arg;

	if (!settings.movement_arg)
	{
		this->mMRate = 0;
		this->mMCoh = 0;
	}

	/*------------------------------------------------------------------------
	 * Thoroughbred mode: One trait = 1, all others = 0.
	 *-----------------------------------------------------------------------*/
	if (settings.thoroughbred_arg)
	{
		if (this->mBEvo > this->mBInd && this->mBEvo > this->mBSoc)
		{
			this->mBEvo = 1;
			this->mBInd = 0;
			this->mBSoc = 0;
		}
		if (this->mBInd > this->mBEvo && this->mBInd > this->mBSoc)
		{
			this->mBEvo = 0;
			this->mBInd = 1;
			this->mBSoc = 0;
		}
		if (this->mBSoc > this->mBEvo && this->mBSoc > this->mBInd)
		{
			this->mBEvo = 0;
			this->mBInd = 0;
			this->mBSoc = 1;
		}
	}

	double sum = this->mBEvo + this->mBInd + this->mBSoc;
	this->mBEvo /= sum;
	this->mBInd /= sum;
	this->mBSoc /= sum;
}

double Agent::get_fitness() const
{
	// return this->mEnv->fitness(*this);
	return this->mDelta;
}


TaskVector Agent::learn_ind()
{
	TaskVector tasks = this->mPhenotype;
	int taskIndex = rng_randint(settings.tasks_arg);
	int bit = rng_randint(settings.bits_arg);
	tasks[taskIndex].flip(bit);
	return tasks;
}

TaskVector Agent::learn_soc()
{
	int bit, index;
	TaskVector tasks = this->mPhenotype;
	int taskIndex = rng_randint(settings.tasks_arg);

	vector <Agent *> neighbours = this->mEnv->get_neighbours(this);

	if (neighbours.size() > 0)
	{
		if (settings.strategy_copy_random_neighbour_arg)
		{
			/*------------------------------------------------------------*
			 * Strategy: Alternatively, pick a neighbour at random.
			 *------------------------------------------------------------*/
			index = rng_randint(neighbours.size());
		}
		else if (settings.strategy_copy_best_neighbour_arg)
		{
			double max_fitness = -1;
			double max_fitness_idx = 0;
			for (unsigned int i = 0; i < neighbours.size(); i++)
			{
				double fitness = neighbours[i]->get_fitness();
				if (fitness > max_fitness)
				{
					max_fitness = fitness;
					max_fitness_idx = i;
				}
			}
			index = max_fitness_idx;
		}
		else
		{
			/*------------------------------------------------------------*
			 * Strategy: By default, copy a neighbour weighted by their
			 * fitness.
			 *------------------------------------------------------------*/
			double fitnesses[neighbours.size()];
			for (unsigned int i = 0; i < neighbours.size(); i++)
				fitnesses[i] = neighbours[i]->get_fitness();
			index = roulette(fitnesses, neighbours.size());
		}

		if (index < 0)
		{
			/*------------------------------------------------------------*
			 * Couldn't find a single non-zero-fitness neighbour -- bail.
			 * XXX: DOES THIS MAKE SENSE IN OUR MODEL?
			 * SHOULDN'T WE STILL BE ABLE TO COPY FROM ZERO-FITNESS NEIGHBOURS?
			 *------------------------------------------------------------*/
			// printf("got neighbours but non with non-zero fitness, bailing...\n");
			return tasks;
		}

		Agent *exemplar = neighbours[index];
		TaskVector exemplar_pheno = exemplar->mPhenotype;

		if (!settings.strategy_copy_novel_trait_arg)
		{
			/*------------------------------------------------------------*
			 * Strategy: By default, copy a random trait.
			 *------------------------------------------------------------*/
			bit = rng_randint(settings.bits_arg);
			tasks[taskIndex].set(bit, exemplar_pheno[taskIndex].test(bit));
			if (rng_coin(settings.p_noise_arg))
			{
				tasks[taskIndex].flip(bit);
			}
		}
		else
		{
			/*------------------------------------------------------------*
			 * Alternate strategy: search for a novel trait and copy that.
			 *------------------------------------------------------------*/
			bool found = false;
			int indices[settings.bits_arg];
			for (int i = 0; i < settings.bits_arg; i++)
				indices[i] = i;
			rng_shuffle(indices, settings.bits_arg);

			for (int i = 0; i < settings.bits_arg && !found; i++)
			{
				bit = indices[i];
				if (tasks[taskIndex].test(bit) != exemplar_pheno[taskIndex].test(bit))
				{
					// cout << "copying bit " << bit << " (mine = " << action << ", theirs = " << exemplar_pheno << ")" << std::endl;
					found = true;
					tasks[taskIndex].set(bit, exemplar_pheno[taskIndex].test(bit));
					if (rng_coin(settings.p_noise_arg))
						tasks[taskIndex].flip(bit);
				}
			}

			if (!found)
			{
				if (rng_coin(settings.p_noise_arg))
				{
					bit = rng_randint(settings.bits_arg);
					tasks[taskIndex].flip(bit);
				}
			}
		}
	}

	return tasks;
}

void Agent::update()
{
	this->mAge++;
	int EPOCHS = 1;

	this->mDelta = 0;
	for (int epoch = 0; epoch < EPOCHS; epoch++)
	{

		double modes[] = { this->mBEvo, this->mBInd, this->mBSoc };
		int mode = roulette(modes, 3);

		TaskVector action = this->mPhenotype;

		switch (mode)
		{
			case MODE_EVO:
				break;

			case MODE_IND:
				action = this->learn_ind();
				break;

			case MODE_SOC:
				action = this->learn_soc();
				break;
		}

		this->mLastAction = mode;

		float payoff = mEnv->payoff(this, action);
		this->mDelta += payoff;

		if (mode == MODE_IND || mode == MODE_SOC)
		{
			if (settings.strategy_always_assimilate_arg)
			{
				/*--------------------------------------------------------------------*
				 * Strategy: If we're set to always assimilate newly-learned bits, 
				 * modify our phenotype accordingly.
				 *--------------------------------------------------------------------*/
				mPhenotype = action;
			}
			else
			{
				/*--------------------------------------------------------------------*
				 * if this action gives a better payoff than our current phenotype,
				 * modify our phenotype accordingly (learning).
				 *--------------------------------------------------------------------*/
				double curFitness = mEnv->payoff(this, mPhenotype);
				if (payoff > curFitness)
					mPhenotype = action;
			}
		}

		if (settings.movement_arg)
		{
			if (settings.movement_rate_genetic_arg)
			{
				bool move = rng_coin(this->mMRate);
				// printf("move: %d (rate %f)\n", move, this->mMRate);
				if (move)
				{
					this->mEnv->move(this);
				}
			}
			else
			{
				// printf("move (i always move)\n");
				this->mEnv->move(this);
			}
		}
	}

	this->mDelta /= EPOCHS;
	if (settings.metabolism_flag)
		this->mOmega += this->mDelta;
}


void Agent::mutate()
{
	this->mBEvo += rng_gaussian(0, settings.mu_arg);
	this->mBEvo  = clip(this->mBEvo, 0, 1);
	if (settings.thoroughbred_mu_arg && rng_coin(settings.thoroughbred_mu_arg))
		this->mBEvo  = 1 - this->mBEvo;

	this->mBInd += rng_gaussian(0, settings.mu_arg);
	this->mBInd  = clip(this->mBInd, 0, 1);
	if (settings.thoroughbred_mu_arg && rng_coin(settings.thoroughbred_mu_arg))
		this->mBInd  = 1 - this->mBInd;

	this->mBSoc += rng_gaussian(0, settings.mu_arg);
	this->mBSoc  = clip(this->mBSoc, 0, 1);
	if (settings.thoroughbred_mu_arg && rng_coin(settings.thoroughbred_mu_arg))
		this->mBSoc  = 1 - this->mBSoc;

	this->mMRate += rng_gaussian(0, settings.mu_arg);
	this->mMRate = clip(this->mMRate, 0, 1);

	this->mMCoh += rng_gaussian(0, settings.mu_arg);
	this->mMCoh = clip(this->mMCoh, -1, 1);

	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	for (int i = 0; i < settings.bits_arg; i++)
	{
		if (rng_coin(settings.p_mut_arg))
		{
			this->mGenotype[taskIndex].flip(i);
		}
	}
	this->mPhenotype = this->mGenotype;

	this->normalize();
}

Agent *Agent::replicate()
{
	Agent *child = new Agent(this);
	child->mutate();
	return child;
}

ostream& operator<< (ostream &stream, Agent &object)
{
	// stream << setprecision(16) << fixed;
	// 	TODO: print out all tasks
	stream << "[agent ([ " << object.mBEvo << ", " << object.mBInd << ", " << object.mBSoc << " | " << object.mLastAction << ": " << object.get_fitness() << " | " << object.mGenotype[0] << " | " << object.mPhenotype[0] << "]) ]";

	return stream;
}

void Agent::dump()
{
	cout << "[agent ([ " << mBEvo << ", " << mBInd << ", " << mBSoc << " ])]:" << get_fitness() << std::endl;
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	{
		cout << " - " << taskIndex << ": " << mGenotype[taskIndex] << " | " << mPhenotype[taskIndex] << endl;
	}
}

} // namespace sim

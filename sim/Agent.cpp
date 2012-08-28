/*--------------------------------------------------------------------*
 * class Agent
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

	this->mGenotype = Task(settings.bits_arg);
	for (int i = 0; i < settings.bits_arg; i++)
		this->mGenotype[i] = rng_coin(0.5);
	// this->mGenotype.set();
	// this->mGenotype = Task(rng_randint(1L << settings.bits_arg));

	this->mBEvo = rng_uniformuf();
	this->mBInd = rng_uniformuf();
	this->mBSoc = rng_uniformuf();

	this->normalize();

	this->reset();
}

Agent::Agent(Agent *parent)
{
	// not sure if this is the best way to clone -- should maybe
	// call normal constructor and then set constants?
	this->mEnv = parent->mEnv;

	this->mGenotype  = parent->mGenotype;

	this->mBEvo = parent->mBEvo;
	this->mBInd = parent->mBInd;
	this->mBSoc = parent->mBSoc;

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
	this->mDelta = 1;
	this->mOmega = settings.omega0_arg;
	this->mLastAction = 0;
}

int Agent::index()
{
	for (unsigned int i = 0; i < mEnv->mAgents.size(); i++)
	{
		if (&mEnv->mAgents[i] == this)
			return i;
	}

	return -1;
}

void Agent::normalize()
{
	double sum = this->mBEvo + this->mBInd + this->mBSoc;
	this->mBEvo /= sum;
	this->mBInd /= sum;
	this->mBSoc /= sum;
}

double Agent::fitness()
{
	// return this->mEnv->fitness(*this);
	return this->mDelta;
}

void Agent::update()
{
	// printf("[%d] update\n", this->index());
	double modes[] = { this->mBEvo, this->mBInd, this->mBSoc };
	int mode = roulette(modes, 3);
	// cout << "roulette [" << modes[0] << ", " << modes[1] << ", " << modes[2] << "]: " << mode << std::endl;

	Task action = this->mPhenotype;

	switch (mode)
	{
		int bit,
			index;

		case MODE_EVO:
			break;
		case MODE_IND:
			bit = rng_randint(settings.bits_arg);
			action.flip(bit);
			break;
		case MODE_SOC:
			vector <Agent *> neighbours = this->mEnv->get_neighbours(*this);
			if (neighbours.size() > 0)
			{
				double fitnesses[neighbours.size()];
				for (unsigned int i = 0; i < neighbours.size(); i++)
					fitnesses[i] = neighbours[i]->fitness();
				index = roulette(fitnesses, neighbours.size());

				Agent *exemplar = neighbours[index];
				Task exemplar_pheno = exemplar->mPhenotype;

				/*
				bit = rng_randint(settings.bits_arg);
				action.set(bit, exemplar_pheno.test(bit));
				*/

				bool found = false;
				int indices[settings.bits_arg];
				for (int i = 0; i < settings.bits_arg; i++)
					indices[i] = i;
				rng_shuffle(indices, settings.bits_arg);

				for (int i = 0; i < settings.bits_arg && !found; i++)
				{
					bit = indices[i];
					if (action.test(bit) != exemplar_pheno.test(bit))
					{
						// cout << "copying bit " << bit << " (mine = " << action << ", theirs = " << exemplar_pheno << ")" << std::endl;
						found = true;
						action.set(bit, exemplar_pheno.test(bit));
						if (rng_coin(settings.p_noise_arg))
							action.flip(bit);
					}
				}

				if (!found)
				{
					if (rng_coin(settings.p_noise_arg))
					{
						bit = rng_randint(settings.bits_arg);
						action.flip(bit);
					}
				}

				// printf("copying bit %d (was %d, now %d)\n", bit, mPhenotype.test(bit), action.test(bit));

			}
			break;
	}

	this->mAge++;
	this->mLastAction = mode;
	this->mDelta = mEnv->payoff(action);

	if (settings.debug_given)
		std::cout << *this << std::endl;

	if (mode != MODE_EVO)
	{
		/*--------------------------------------------------------------------*
		 * if this action gives a better payoff than our current phenotype,
		 * modify our phenotype accordingly (learning).
		 *--------------------------------------------------------------------*/
		double curFitness = mEnv->payoff(mPhenotype);
		// cout << "action " << mode << ": my " << mPhenotype << " = " << curFitness << ", new " << action << " = " << this->mDelta << std::endl;
		if (mDelta > curFitness)
			mPhenotype = action;
	}

	// XXX: testing fixed social cost
	if (mode == MODE_SOC)
	{
		this->mDelta -= settings.cost_soc_arg;
		if (this->mDelta < 0)
			this->mDelta = 0;
	}

	this->mOmega += this->mDelta;
}

void Agent::mutate()
{
	this->mBEvo += rng_gaussian(0, settings.mu_arg);
	this->mBEvo  = clip(this->mBEvo, 0, 1);
	this->mBInd += rng_gaussian(0, settings.mu_arg);
	this->mBInd  = clip(this->mBInd, 0, 1);
	this->mBSoc += rng_gaussian(0, settings.mu_arg);
	this->mBSoc  = clip(this->mBSoc, 0, 1);

	for (int i = 0; i < settings.bits_arg; i++)
	{
		if (rng_coin(settings.p_mut_arg))
			this->mGenotype.flip(i);
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
	stream << setprecision(4) << fixed;
	stream << "[agent ([ " << object.mBEvo << ", " << object.mBInd << ", " << object.mBSoc << " | " << object.mLastAction << ": " << object.fitness() << " | " << object.mGenotype << " | " << object.mPhenotype << "]) ]";
	return stream;
}

} // namespace sim
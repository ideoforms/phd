/*--------------------------------------------------------------------*B	C VBNN 	WB1NM
 * class Landscape
 *--------------------------------------------------------------------*/

#include "sim/Landscape.h"
#include "sim/LandscapeGenerator.h"

#include <iostream>

#define DETERMINISTIC_VARIANCE

using namespace sim;

namespace sim
{

extern struct settings_t settings;

Landscape::Landscape(unsigned bits, unsigned width, unsigned height)
{
	this->mBits = bits;
	this->mWidth = width;
	this->mHeight = height;

	this->mObjective.resize(this->mWidth);
	for (int x = 0; x < this->mWidth; x++)
	{
		this->mObjective[x].resize(this->mHeight);
		for (int y = 0; y < this->mHeight; y++)
		{
			this->mObjective[x][y] = TaskVector();
			for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
			{
				this->mObjective[x][y].push_back(Task(settings.bits_arg));
				this->mObjective[x][y][taskIndex].reset();
			}
		}
	}
	
	this->mPayoff.resize(this->mWidth);
	for (int x = 0; x < this->mWidth; x++)
	{
		this->mPayoff[x].resize(this->mHeight);
	}
	this->distribute_payoffs();
}

void Landscape::distribute_payoffs()
{
	LandscapeGenerator *generator;
	if (strcmp(settings.payoff_distribution_arg, "structured") == 0)
	{
		float detail = settings.structured_landscape_detail_arg;
		float gradient = settings.structured_landscape_gradient_arg;
		float abundance = settings.structured_landscape_abundance_arg;
		generator = new LandscapeGenerator(detail, gradient, abundance);
	}

	for (int x = 0; x < this->mWidth; x++)
	for (int y = 0; y < this->mHeight; y++)
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	{
		double payoff = 0;

		if (settings.tasks_arg == 2 && taskIndex == 1)
		{
			payoff = 1.0 - this->mPayoff[x][y][0];
			printf("*** setting to inverse of other task: %f -> %f\n", this->mPayoff[x][y][0], payoff);
		}
		else if (strcmp(settings.payoff_distribution_arg, "uniform") == 0)
		{
			payoff = 1.0 / settings.tasks_arg;
		}
		else if (strcmp(settings.payoff_distribution_arg, "random") == 0)
		{
			payoff = rng_uniform(0, 1) / settings.tasks_arg;
		}
		else if (strcmp(settings.payoff_distribution_arg, "anticorrelated") == 0)
		{
			/*-----------------------------------------------------------------------*
			 * Chequerboard pattern -- pairwise correlation is -1
			 *-----------------------------------------------------------------------*/
			payoff = (x + y) % 2;
			payoff /= settings.tasks_arg;
		}
		else if (strcmp(settings.payoff_distribution_arg, "correlated") == 0)
		{
			if (x == 0)
			{
				if (y == 0)
					payoff = 0.5;
				else
					payoff = this->mPayoff[x][y - 1][taskIndex] + rng_gaussian(0, settings.payoff_correlation_mu_arg);
			}
			else
			{
				float mu = (y == 0) ? rng_gaussian(0, settings.payoff_correlation_mu_arg) : (this->mPayoff[x][0][taskIndex] - this->mPayoff[x - 1][0][taskIndex]);
				
				payoff = this->mPayoff[x - 1][y][taskIndex] + mu;
			}
			payoff = clip(payoff, 0, 1);
			payoff /= settings.tasks_arg;
		}
		else if (strcmp(settings.payoff_distribution_arg, "structured") == 0)
		{
            // DJJ 2014-03-23 - changed from "16" - is this why we are not getting such strong results?
			payoff = generator->get((float) x / mWidth, (float) y / mWidth);
			printf("generator payoff: %f\n", payoff);
		}
		else
		{
			throw("invalid payoff distribution type");
		}

		this->mPayoff[x][y].push_back(payoff);
	}
}
	
void Landscape::distribute(unsigned heterogeneity)
{
	if (settings.tasks_arg > 1)
		printf("WARNING: Distribute broken for >1 tasks\n");
	/*-----------------------------------------------------------------------*
	 * Creates a 2D grid landscape where the pairwise Hamming distance
	 * between any two cells is equal to heterogeneity.
	 * TODO: This is dodgy - varies both tasks simultaneously - should just
	 *       vary 1 bit
	 *-----------------------------------------------------------------------*/
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	{
		for (int y = 1; y < this->mHeight; y++)
		{
			this->mObjective[0][y][taskIndex] = this->mObjective[0][y - 1][taskIndex];
			for (int h = 0; h < heterogeneity; h++)
			{
				#ifdef DETERMINISTIC_VARIANCE
					this->mObjective[0][y][taskIndex].flip(h);
				#else
					int index = rng_randint(this->mBits);
					this->mObjective[0][y][taskIndex].flip(index);
				#endif
			}
		}

		for (int x = 1; x < this->mWidth; x++)
		{
			this->mObjective[x][0][taskIndex] = this->mObjective[x - 1][0][taskIndex];
			for (int h = 0; h < heterogeneity; h++)
			{
				#ifdef DETERMINISTIC_VARIANCE
					this->mObjective[x][0][taskIndex].flip(h);
				#else
					int index = rng_randint(this->mBits);
					this->mObjective[x][0][taskIndex].flip(index);
				#endif
			}

			for (int y = 1; y < this->mHeight; y++)
			{
				this->mObjective[x][y][taskIndex] = this->mObjective[x][0][taskIndex] ^ this->mObjective[0][y][taskIndex];
			}
		}
	}
}

void Landscape::dump()
{
	for (int y = 0; y < this->mHeight; y++)
	{
		for (int x = 0; x < this->mWidth; x++)
		{
			cout << this->mObjective[x][y][0] << " ";
		}
		cout << std::endl;
	}
}

void Landscape::perturb(float perturbance)
{
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	{
		for (int index = 0; index < this->mBits; index++)
		{
			if (rng_coin(perturbance))
			{
				for (int x = 0; x < this->mWidth; x++)
				for (int y = 0; y < this->mHeight; y++)
					this->mObjective[x][y][taskIndex].flip(index);
			}
		}
	}
}

void Landscape::fluctuate()
{
	/*--------------------------------------------------------------------*
	 * fluctuations
	 *--------------------------------------------------------------------*/
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
	{
		unsigned index = rng_randint(this->mBits);
		for (int x = 0; x < this->mWidth; x++)
		for (int y = 0; y < this->mHeight; y++)
			this->mObjective[x][y][taskIndex].flip(index);
	}
}
	
void Landscape::deplete_at(unsigned x, unsigned y)
{
	for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
		this->mPayoff[x][y][taskIndex] *= (1 - settings.payoff_depletion_rate_arg);
}
	
void Landscape::regenerate()
{
	if (!settings.payoff_regeneration_rate_arg)
		return;
	else
		for (int x = 0; x < this->mWidth; x++)
		for (int y = 0; y < this->mHeight; y++)
		for (int taskIndex = 0; taskIndex < settings.tasks_arg; taskIndex++)
		{
			this->mPayoff[x][y][taskIndex] *= (1 + settings.payoff_regeneration_rate_arg);
			this->mPayoff[x][y][taskIndex] = clip(this->mPayoff[x][y][taskIndex], 0, 1);
		}
}


TaskVector Landscape::tasks_at(unsigned x, unsigned y)
{
	if (settings.spatial_patch_size_arg == 1)
	{
		// cout << "Landscape::tasks_at " << x << ", " << y << ": " << this->mObjective[x][y][0] << ", " << this->mObjective[x][y][1] << endl;
		return this->mObjective[x][y];
	}
	else
	{
		unsigned patch_x = x / settings.spatial_patch_size_arg;
		unsigned patch_y = y / settings.spatial_patch_size_arg;
		return this->mObjective[patch_x][patch_y];
	}
}
	
PayoffVector Landscape::payoffs_at(unsigned x, unsigned y)
{
	unsigned patch_x = x / settings.spatial_patch_size_arg;
	unsigned patch_y = y / settings.spatial_patch_size_arg;
	return this->mPayoff[patch_x][patch_y];
}


} /* namespace sim */


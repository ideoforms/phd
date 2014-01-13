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
			this->mObjective[x][y] = Task(this->mBits);
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
	{
		if (strcmp(settings.payoff_distribution_arg, "uniform") == 0)
		{
			this->mPayoff[x][y] = 1.0;
		}
		else if (strcmp(settings.payoff_distribution_arg, "random") == 0)
		{
			this->mPayoff[x][y] = rng_uniform(0, 1);
		}
		else if (strcmp(settings.payoff_distribution_arg, "correlated") == 0)
		{
			if (x == 0)
			{
				if (y == 0)
					this->mPayoff[x][y] = 0.5;
				else
					this->mPayoff[x][y] = this->mPayoff[x][y - 1] + rng_gaussian(0, settings.payoff_correlation_mu_arg);
			}
			else
			{
				float mu = (y == 0) ? rng_gaussian(0, settings.payoff_correlation_mu_arg) : (this->mPayoff[x][0] - this->mPayoff[x - 1][0]);
				
				this->mPayoff[x][y] = this->mPayoff[x - 1][y] + mu;
			}
			this->mPayoff[x][y] = clip(this->mPayoff[x][y], 0, 1);
		}
		else if (strcmp(settings.payoff_distribution_arg, "structured") == 0)
		{
			this->mPayoff[x][y] = generator->get((float) x / mWidth, (float) y / mHeight);
		}
		else
		{
			throw("invalid payoff distribution type");
		}
	}
}

void Landscape::distribute(unsigned heterogeneity)
{
	/*-----------------------------------------------------------------------*
	 * Creates a 2D grid landscape where the pairwise Hamming distance
	 * between any two cells is equal to heterogeneity.
	 *-----------------------------------------------------------------------*/
	for (int y = 1; y < this->mHeight; y++)
	{
		this->mObjective[0][y] = this->mObjective[0][y - 1];
		for (int h = 0; h < heterogeneity; h++)
		{
			#ifdef DETERMINISTIC_VARIANCE
				this->mObjective[0][y].flip(h);
			#else
				int index = rng_randint(this->mBits);
				this->mObjective[0][y].flip(index);
			#endif
		}
	}

	for (int x = 1; x < this->mWidth; x++)
	{
		this->mObjective[x][0] = this->mObjective[x - 1][0];
		for (int h = 0; h < heterogeneity; h++)
		{
			#ifdef DETERMINISTIC_VARIANCE
				this->mObjective[x][0].flip(h);
			#else
				int index = rng_randint(this->mBits);
				this->mObjective[x][0].flip(index);
			#endif
		}

		for (int y = 1; y < this->mHeight; y++)
			this->mObjective[x][y] = this->mObjective[x][0] ^ this->mObjective[0][y];
	}
}

void Landscape::dump()
{
	for (int y = 0; y < this->mHeight; y++)
	{
		for (int x = 0; x < this->mWidth; x++)
		{
			cout << this->mObjective[x][y] << " ";
		}
		cout << std::endl;
	}
}

void Landscape::perturb(float perturbance)
{
	for (int index = 0; index < this->mBits; index++)
	{
		if (rng_coin(perturbance))
		{
			for (int x = 0; x < this->mWidth; x++)
			for (int y = 0; y < this->mHeight; y++)
				this->mObjective[x][y].flip(index);
		}
	}
}

void Landscape::fluctuate()
{
	/*--------------------------------------------------------------------*
	 * fluctuations
	 *--------------------------------------------------------------------*/
	unsigned index = rng_randint(this->mBits);
	for (int x = 0; x < this->mWidth; x++)
	for (int y = 0; y < this->mHeight; y++)
		this->mObjective[x][y].flip(index);
}
	
void Landscape::deplete_at(unsigned x, unsigned y)
{
	this->mPayoff[x][y] *= (1 - settings.payoff_depletion_rate_arg);
}
	
void Landscape::regenerate()
{
	if (!settings.payoff_regeneration_rate_arg)
		return;
	else
		for (int x = 0; x < this->mWidth; x++)
		for (int y = 0; y < this->mHeight; y++)
		{
			this->mPayoff[x][y] *= (1 + settings.payoff_regeneration_rate_arg);
			this->mPayoff[x][y] = clip(this->mPayoff[x][y], 0, 1);
		}
}


Task Landscape::taskAt(unsigned x, unsigned y)
{
	if (settings.spatial_patch_size_arg == 1)
	{
		return this->mObjective[x][y];
	}
	else
	{
		unsigned patch_x = x / settings.spatial_patch_size_arg;
		unsigned patch_y = y / settings.spatial_patch_size_arg;
		return this->mObjective[patch_x][patch_y];
	}
}
	
float Landscape::payoffAt(unsigned x, unsigned y)
{
	unsigned patch_x = x / settings.spatial_patch_size_arg;
	unsigned patch_y = y / settings.spatial_patch_size_arg;
	return this->mPayoff[patch_x][patch_y];
}


} /* namespace sim */


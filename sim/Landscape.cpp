/*--------------------------------------------------------------------*B	C VBNN 	WB1NM
 * class Landscape
 *--------------------------------------------------------------------*/

#include "sim/Landscape.h"
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

	this->mGrid.resize(this->mWidth);
	for (int x = 0; x < this->mWidth; x++)
	{
		this->mGrid[x].resize(this->mHeight);
		for (int y = 0; y < this->mHeight; y++)
			this->mGrid[x][y] = Task(this->mBits);
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
		this->mGrid[0][y] = this->mGrid[0][y - 1];
		for (int h = 0; h < heterogeneity; h++)
		{
			#ifdef DETERMINISTIC_VARIANCE
				this->mGrid[0][y].flip(h);
			#else
				int index = rng_randint(this->mBits);
				this->mGrid[0][y].flip(index);
			#endif
		}
	}

	for (int x = 1; x < this->mWidth; x++)
	{
		this->mGrid[x][0] = this->mGrid[x - 1][0];
		for (int h = 0; h < heterogeneity; h++)
		{
			#ifdef DETERMINISTIC_VARIANCE
				this->mGrid[x][0].flip(h);
			#else
				int index = rng_randint(this->mBits);
				this->mGrid[x][0].flip(index);
			#endif
		}

		for (int y = 1; y < this->mHeight; y++)
			this->mGrid[x][y] = this->mGrid[x][0] ^ this->mGrid[0][y];
	}
}

void Landscape::dump()
{
	for (int y = 0; y < this->mHeight; y++)
	{
		for (int x = 0; x < this->mWidth; x++)
		{
			cout << this->mGrid[x][y] << " ";
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
				this->mGrid[x][y].flip(index);
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
		this->mGrid[x][y].flip(index);
}


Task Landscape::taskAt(unsigned x, unsigned y)
{
	if (settings.spatial_patch_size_arg == 1)
	{
		return this->mGrid[x][y];
	}
	else
	{
		unsigned patch_x = x / settings.spatial_patch_size_arg;
		unsigned patch_y = y / settings.spatial_patch_size_arg;
		return this->mGrid[patch_x][patch_y];
	}
}


} /* namespace sim */


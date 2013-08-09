/*--------------------------------------------------------------------*B    C VBNN 	WB1NM
 * class Landscape
 *--------------------------------------------------------------------*/

#include "sim/Landscape.h"
#include <iostream>

using namespace sim;

#define GRID_SIZE 8

namespace sim
{

LandscapeTiled::LandscapeTiled(unsigned width, unsigned height)
{
	this->mWidth = width;
	this->mHeight = height;
}

void LandscapeTiled::distribute(unsigned correlation)
{
    this->mGrid.resize(this->mWidth);
    for (int x = 0; x < this->mWidth; x++)
	{
        this->mGrid[x].resize(this->mHeight);
		for (int y = 0; y < this->mHeight; y++)
			this->mGrid[x][y] = Task(GRID_SIZE);
	}

	for (int y = 1; y < this->mHeight; y++)
	{
		this->mGrid[0][y] = this->mGrid[0][y - 1];
		int index = rng_randint(GRID_SIZE - 1);
		this->mGrid[0][y].flip(index);
	}

	for (int x = 1; x < this->mWidth; x++)
	{
		this->mGrid[x][0] = this->mGrid[x - 1][0];
		int index = rng_randint(GRID_SIZE - 1);
		this->mGrid[x][0].flip(index);

		for (int y = 1; y < this->mHeight; y++)
			this->mGrid[x][y] = this->mGrid[x][0] ^ this->mGrid[0][y];
	}
}

void LandscapeTiled::dump()
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


void LandscapeTiled::fluctuate()
{
	/*--------------------------------------------------------------------*
	 * fluctuations
	 *--------------------------------------------------------------------*/
	unsigned index = rng_randint(GRID_SIZE - 1);
	for (int x = 0; x < this->mWidth; x++)
	for (int y = 0; y < this->mHeight; y++)
	{
		this->mGrid[x][y].flip(index);
	}
}


Task LandscapeTiled::taskAt(unsigned x, unsigned y)
{
	return Task();
}


} /* namespace sim */

#pragma once

#include <vector>
#include "sim/Util.h"

using namespace std;
using namespace sim;

namespace sim
{

class Node
{
	public:
		unsigned mIndex;
		void *mData;
		vector <Node *> mNeighbours;

		Node(unsigned index) 
		{
			this->mIndex = index;
			this->mData = 0;
			this->mNeighbours.reserve(8);
		}
};

class Graph
{
	public:
		vector <Node *> mNodes;
		
		Graph()
		{
			mNodes.reserve(256);
		}

		void make_regular(int N, int K)
		{
			for (int i = 0; i < N; i++)
			{
				Node *node = new Node(i);
				mNodes.push_back(node);
			}

			while (true)
			{
				for (Node *node : mNodes)
					node->mNeighbours.clear();

				bool failed = false;
				for (int n = 0; n < N * K / 2; n++)
				{
					Node *nodeA, *nodeB;
					Node *options[N];
					int option_count;

					option_count = 0;
					for (Node *node : mNodes)
						if (node->mNeighbours.size() < K)
							options[option_count++] = node;
					nodeA = options[rng_randint(0, option_count)];

					option_count = 0;
					for (Node *node : mNodes)
						if (node->mNeighbours.size() < K && node != nodeA)
						{
							// check that this node isn't already in A's bucket
							bool found = false;
							for (Node *linked : nodeA->mNeighbours)
								if (linked == node)
									found = true;
							if (!found)
								options[option_count++] = node;
						}
					if (option_count == 0)
					{
						printf("got no options, restarting\n");
						failed = true;
						break;
					}

					nodeB = options[rng_randint(0, option_count)];
					nodeA->mNeighbours.push_back(nodeB);
					nodeB->mNeighbours.push_back(nodeA);
				}

				if (!failed)
					break;
			}
		}
}; /* class */

}; /* namespace */


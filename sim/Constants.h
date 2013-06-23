#pragma once

#include <vector>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include "sim/config/settings.h"

namespace sim
{

extern struct settings_t settings;

class Agent;

enum behaviour_t
{
	MODE_EVO,
	MODE_IND,
	MODE_SOC
};

struct stats_t
{
	double bevo_mean,
		   bind_mean,
		   bsoc_mean;

	double fitness_min,
		   fitness_mean,
	       fitness_max;

	double geno_mean,
		   pheno_mean;
};

typedef boost::dynamic_bitset <> Task;

typedef std::vector<Agent>::iterator agent_iterator;
typedef std::vector<Agent *>::iterator agent_p_iterator;

} // namespace sim



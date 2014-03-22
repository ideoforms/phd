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

enum neighbourhood_type_t
{
    NEIGHBOURHOOD_TYPE_METRIC,
    NEIGHBOURHOOD_TYPE_TOPOLOGICAL,
    NEIGHBOURHOOD_TYPE_SOCIAL
};

struct stats_t
{
	unsigned step;

	double bevo_mean,
		   bind_mean,
		   bsoc_mean;

	double mrate_mean,
		   mcoh_mean;

	double age_mean;

	double fitness_min,
		   fitness_mean,
	       fitness_max;

	double geno_mean_dist,
		   pheno_mean_dist;

	double dispersion;
};

typedef boost::dynamic_bitset <> Task;

typedef std::vector<Agent *>::iterator agent_iterator;

} // namespace sim



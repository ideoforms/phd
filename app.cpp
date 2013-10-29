#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "sim/Simulation.h"
#include "app-utils.h"

using namespace std;
using namespace sim;

#if defined(TOPOLOGY_CA)
	#define ENV_CLASS sim::EnvironmentCA
#elif defined(TOPOLOGY_1D)
	#define ENV_CLASS sim::EnvironmentCA1D
#elif defined(TOPOLOGY_ABM)
	#define ENV_CLASS sim::EnvironmentABM
#else
	#define ENV_CLASS sim::Environment
#endif

settings_t 			sim::settings;
ENV_CLASS			*env;

int main (int argc, char *argv[])
{
	init_config(argc, argv);
	init_simulator();
	init_logging();

	env = new ENV_CLASS();

	printf("      env: %s\n",   env->classname());
	printf("    steps: %d\n",   settings.steps_arg);
	printf("     bits: %d\n",   settings.bits_arg);
	printf(" p_switch: %.1f\n", settings.p_switch_arg);
	printf("      log: %d\n",   settings.log_given);
	printf("  logfile: %s\n",   settings.logfile_arg);

	for (int step = 0; step < settings.steps_arg; step++)
	{
		if (step % 1000 == 0)
			printf(" - step %05d\n", step);

		/*---------------------------------------------------------------------*
		 * if we're in batch mode, redirect stdout to the null device -- 
		 * we don't want to output anything except our final state.
		 *---------------------------------------------------------------------*/
		if (settings.perturbation_given && settings.perturbation_time_arg == step)
			env->perturb(settings.perturbation_size_arg);

		if (settings.log_phenotypes_at_arg == step && step > 0)
			log_phenotypes();

		/*---------------------------------------------------------------------*
		 * update the environment, and generate stats.
		 *---------------------------------------------------------------------*/
		env->update();

		/*---------------------------------------------------------------------*
		 * if we're logging each timestep, output accordingly.
		 *---------------------------------------------------------------------*/
		if (settings.log_given)
			log_states();
	}

	close_logging();

	return 0;
}


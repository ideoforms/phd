#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "sim/Simulation.h"
#include "app-utils.h"

using namespace std;
using namespace sim;

/*-----------------------------------------------------------------------*
 * (c) Daniel Jones 2013
 *-----------------------------------------------------------------------*/

settings_t 			sim::settings;
Environment			*env;

int main (int argc, char *argv[])
{
	init_config(argc, argv);
	init_simulator();

	if (!settings.batch_flag)
	{
		printf(" topology: %s\n",   settings.topology_arg);
		printf("    steps: %d\n",   settings.steps_arg);
		printf("   trials: %d\n",   settings.trials_arg);
		printf("     bits: %d\n",   settings.bits_arg);
		printf(" p_switch: %.1f\n", settings.p_switch_arg);
	}

	for (int trial = 0; trial < settings.trials_arg; trial++)
	{
		init_logging();

		if (strcmp(settings.topology_arg, "numeric") == 0)
			env = new sim::Environment();
		else if (strcmp(settings.topology_arg, "ca-1d") == 0)
			env = new sim::EnvironmentCA1D();
		else if (strcmp(settings.topology_arg, "ca-2d") == 0)
			env = new sim::EnvironmentCA2D();
		else if (strcmp(settings.topology_arg, "ca-2d-multi") == 0)
			env = new sim::EnvironmentCA2DMulti();
		else if (strcmp(settings.topology_arg, "abm") == 0)
			env = new sim::EnvironmentABM();
		else
		{
			printf("invalid topology type: %s\n", settings.topology_arg);
			exit(1);
		}

		for (int step = 0; step < settings.steps_arg; step++)
		{
			if (step % 1000 == 0)
				printf(" - step %05d\n", step);

			/*---------------------------------------------------------------------*
			 * if we're in batch mode, redirect stdout to the null device -- 
			 * we don't want to output anything except our final state.
			 *---------------------------------------------------------------------*/
			if (settings.perturbation_flag && settings.perturbation_time_arg == step)
				env->perturb(settings.perturbation_size_arg);

			// if (settings.log_phenotypes_at_arg == step && step > 0)
			//	log_phenotypes();

			/*---------------------------------------------------------------------*
			 * update the environment, and generate stats.
			 *---------------------------------------------------------------------*/
			env->update();

			/*---------------------------------------------------------------------*
			 * if we're logging each timestep, output accordingly.
			 *---------------------------------------------------------------------*/
			if (settings.log_flag && step % settings.log_every_arg == 0)
				log_states();
		}

		if (trial < settings.trials_arg - 1)
			print_elapsed_time(trial + 1, settings.trials_arg);

		close_logging();
	}

	return 0;
}


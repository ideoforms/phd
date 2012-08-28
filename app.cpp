#include "sim/Simulation.h"
#include "sim/io/csv.h"

#include "app.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;
using namespace sim;

settings_t sim::settings;

int stdout_orig;
sim::Environment *env;
csvwriter *logger;

int main (int argc, char *argv[])
{
	init_config(argc, argv);
	init_simulator();
	init_logging();

	env = new sim::Environment(settings.popsize_arg);

	printf("    steps: %d\n", settings.steps_arg);
	printf("    bits: %d\n", settings.bits_arg);
	printf(" p_switch: %.1f\n", settings.p_switch_arg);
	printf("      log: %d\n", settings.log_given);
	printf("  logfile: %s\n", settings.logfile_arg);

	for (int step = 0; step < settings.steps_arg; step++)
	{
		if (step % 1000 == 0)
			printf(" - step %05d\n", step);

		/*---------------------------------------------------------------------*
		 * if we're in batch mode, redirect stdout to the null device -- 
		 * we don't want to output anything except our final state.
		 *---------------------------------------------------------------------*/
		if (settings.perturbation_given && settings.perturbation_time_arg == step)
		{
			env->perturb(settings.perturbation_size_arg);
		}

		/*---------------------------------------------------------------------*
		 * update the environment, and generate stats.
		 *---------------------------------------------------------------------*/
		env->update();
		stats_t stats = env->stats();

		/*---------------------------------------------------------------------*
		 * if we're logging each timestep, output accordingly.
		 *---------------------------------------------------------------------*/
		if (settings.log_given)
		{
			logger->write
			(
			 	step,

				stats.bevo_mean,
				stats.bind_mean,
				stats.bsoc_mean,

    			stats.fitness_min,
    			stats.fitness_mean,
    			stats.fitness_max,

				0.0,
				0.0
			);

		}
	}

	close_logging();

	return 0;
}

void init_config(int argc, char *argv[])
{
	if (config_parser (argc, argv, &settings) != 0)
		exit(1);

	if (config_parser_configfile(settings.conf_file_arg, &settings, 0, 0, 1) != 0)
		exit(1);
}

void init_simulator()
{
	sim::rng_init();
}

void init_logging()
{
	if (settings.batch_given)
	{
		/*---------------------------------------------------------------------*
		 * if we're in batch mode, redirect stdout to the null device -- 
		 * we don't want to output anything except our final state.
		 *---------------------------------------------------------------------*/
		stdout_orig = dup(1);
		int stdout_new = open("/dev/null", O_WRONLY);
		dup2(stdout_new, 1);
		close(stdout_new);

		/*---------------------------------------------------------------------*
		 * also disable logging to file.
		 *---------------------------------------------------------------------*/
		settings.log_given = false;
	}

	if (settings.log_given)
	{
		/*---------------------------------------------------------------------*
		 * construct logfile from the format string specified, replacing %s
		 * with the current timestamp.
		 *---------------------------------------------------------------------*/
		char *logfile_format = settings.logfile_arg;
		char *logfile = (char *) malloc(64);
		char timestr[32];
		sprintf(timestr, "%ld", (long) time(NULL));
		sprintf(logfile, logfile_format, timestr);
		settings.logfile_arg = logfile;

		/*---------------------------------------------------------------------*
		 * if the directory does not already exist, create it.
		 *---------------------------------------------------------------------*/
		/*
		if (access(logfile, F_OK) != 0)
		{
			int rv = mkdir(logfile, 0755);
			if (rv)
			{
				printf("could not make logfile %s\n", logfile);
				exit(1);
			}
		}
		*/

		logger = new csvwriter(logfile);
		logger->start
			(
			 	"iffffffff",

				"t",
				"evo",
				"ind",
				"soc",
				"d_min",
				"d_mean",
				"d_max",
				"geno",
				"pheno"
			);
	}
}

void close_logging()
{
	if (settings.batch_given)
	{
		fflush(stdout);
		dup2(stdout_orig, 1);
		close(stdout_orig);

		stats_t stats = env->stats();

		csvwriter batchlog(stdout);
		batchlog.format("iiiffffff");
		batchlog.write
			(
				settings.popsize_arg,
				settings.steps_arg,
				settings.bits_arg,
				settings.p_switch_arg,
				settings.p_noise_arg,
				stats.fitness_mean,
				stats.bevo_mean,
				stats.bind_mean,
				stats.bsoc_mean
			);
	}

	if (settings.log_given)
	{
		logger->close();
	}

	free(settings.logfile_arg);
}
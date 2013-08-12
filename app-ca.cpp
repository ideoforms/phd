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
sim::EnvironmentCA *env;
csvwriter *logger;

void log_phenotypes();

int main (int argc, char *argv[])
{
	init_config(argc, argv);
	init_simulator();
	init_logging();

	env = new sim::EnvironmentCA(settings.ca_width_arg, settings.ca_width_arg);

	settings.popsize_arg = settings.ca_width_arg * settings.ca_width_arg;

	// sim = new sim::Simulation(env);
	// sim.init_config(argc, argv);
	// sim.dump();

	printf("    steps: %d\n",   settings.steps_arg);
	printf("     bits: %d\n",   settings.bits_arg);
	printf(" p_switch: %.3f\n", settings.p_switch_arg);
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
		{
			env->perturb(settings.perturbation_size_arg);
		}

		if (settings.log_phenotypes_at_arg == step && step > 0)
		{
			log_phenotypes();
		}

		/*---------------------------------------------------------------------*
		 * update the environment, and generate stats.
		 *---------------------------------------------------------------------*/
		env->update();

		/*---------------------------------------------------------------------*
		 * if we're logging each timestep, output accordingly.
		 *---------------------------------------------------------------------*/
		if (settings.log_given)
		{
			stats_t stats = env->stats();
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
	/*---------------------------------------------------------------------*
	 * general simulator setup (necessary for all simulation types):
	 *  - initialise random number generator
	 *  - redirect output to /dev/null if in batch mode
	 *---------------------------------------------------------------------*/
	rng_init();

	if (settings.batch_given)
	{
		/*---------------------------------------------------------------------*
		 * if we're in batch mode, redirect stdout to the null device -- 
		 * we don't want to output anything except our final state.
		 *---------------------------------------------------------------------*/
		stdout_orig = dup(1);
		int stdout_new = open("/dev/null", O_WRONLY);

		/*---------------------------------------------------------------------*
		 * dup2 does the job of assigning the new (null) fd to descriptor 1,
		 * so any subsequent printf statements will go into the void.
		 *---------------------------------------------------------------------*/
		dup2(stdout_new, 1);
		close(stdout_new);

		/*---------------------------------------------------------------------*
		 * also disable logging to file.
		 *---------------------------------------------------------------------*/
		settings.log_given = false;
	}

}

void init_logging()
{
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

		printf("logging to file %s\n", logfile);

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
	}

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

	if (settings.log_given)
	{
		logger->close();
	}

	free(settings.logfile_arg);
}

void log_phenotypes()
{
	/*---------------------------------------------------------------------*
	 *---------------------------------------------------------------------*/
	char *logfile_format = "logs/phenotypes-%s.csv";
	char *logfile = (char *) malloc(64);
	char timestr[32];
	sprintf(timestr, "%ld", (long) time(NULL));
	sprintf(logfile, logfile_format, timestr);
	settings.logfile_arg = logfile;

	printf("logging to file %s\n", logfile);

	csvwriter logger(logfile);
	logger.start
	(
		"iiifff",

		"index",
		"x",
		"y",
		"b_evo",
		"b_ind",
		"b_soc",
		"fitness"
	);
	for (int i = 0; i < env->mAgents.size(); i++)
	{
		Agent *agent = env->mAgents[i];
		logger.write(i, env->mPositions[i].x, env->mPositions[i].y,
			agent->mBEvo, agent->mBInd, agent->mBSoc, agent->mDelta);
	}
	logger.close();
}


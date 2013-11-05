#include "sim/Simulation.h"
#include "sim/io/csv.h"

#include "app-utils.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

using namespace std;
using namespace sim;

extern settings_t sim::settings;
extern sim::EnvironmentCA *env;

int stdout_orig;
csvwriter *logger;

double t0;

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
	 *  - capture initial timestamp, to output estimated time to completion
	 *---------------------------------------------------------------------*/
	rng_init();
	t0 = millitime();
}

void init_logging()
{
	/*---------------------------------------------------------------------*
	 * logging setup:
	 *  - redirect output to /dev/null if in batch mode
	 *---------------------------------------------------------------------*/
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

	if (settings.log_given)
	{
		/*---------------------------------------------------------------------*
		 * construct logfile from the format string specified, replacing %s
		 * with the current timestamp.
		 *---------------------------------------------------------------------*/
		const char *logfile_format = settings.logfile_arg;
		char *logfile = (char *) malloc(64);
		char timestr[32];
		sprintf(timestr, "%ld", (long) time(NULL));
		sprintf(logfile, logfile_format, timestr);

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

	// free(settings.logfile_arg);
}


void log_states()
{
	/*---------------------------------------------------------------------*
	 * if we're logging each timestep, output accordingly.
	 *---------------------------------------------------------------------*/
	stats_t stats = env->stats();
	logger->write
	(
		stats.step,

		stats.bevo_mean,
		stats.bind_mean,
		stats.bsoc_mean,

		stats.fitness_min,
		stats.fitness_mean,
		stats.fitness_max,

		stats.geno_mean_dist,
		stats.pheno_mean_dist
	);

	if (settings.debug_given)
	{
		printf("(%05d) %.4f %.4f %.4f (mean fitness = %.3f)\n", stats.step, stats.bevo_mean, stats.bind_mean, stats.bsoc_mean, stats.fitness_mean);
	}
}

void log_phenotypes()
{
	/*---------------------------------------------------------------------*
	 *---------------------------------------------------------------------*/
	const char *logfile_format = "logs/phenotypes-%s.csv";
	char *logfile = (char *) malloc(64);
	char timestr[32];
	sprintf(timestr, "%ld", (long) time(NULL));
	sprintf(logfile, logfile_format, timestr);
	settings.logfile_arg = logfile;

	// printf("logging to file %s\n", logfile);

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

void print_elapsed_time(int trial_index, int trial_total)
{
	double elapsed = millitime() - t0;
	double predicted = predict(elapsed, trial_index, trial_total);

    int minutes = (int) predicted / 60;
    printf("completed trial %d/%d (elapsed %.6f, remaining %dm%.3fs)\n", trial_index, trial_total, elapsed, minutes, predicted - (60 * minutes));
}

double millitime()
{
	struct timeval tv;
	int rv = gettimeofday(&tv, NULL);
	if (rv)
		return 0.0;
	double time = (double) tv.tv_sec + tv.tv_usec * 0.000001f;
	return time;
}

double predict(double elapsed, int index, int total)
{
	double ratio = (double) index / (double) total;
	double ratio_time = (elapsed / ratio) * (1.0 - ratio);
	return ratio_time;
}


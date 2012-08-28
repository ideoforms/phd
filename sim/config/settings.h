/** @file settings.h
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.22.4
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt by Lorenzo Bettini */

#ifndef SETTINGS_H
#define SETTINGS_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CONFIG_PARSER_PACKAGE
/** @brief the program name (used for printing errors) */
#define CONFIG_PARSER_PACKAGE "imitation-cpp"
#endif

#ifndef CONFIG_PARSER_PACKAGE_NAME
/** @brief the complete program name (used for help and version) */
#define CONFIG_PARSER_PACKAGE_NAME "imitation-cpp"
#endif

#ifndef CONFIG_PARSER_VERSION
/** @brief the program version */
#define CONFIG_PARSER_VERSION "1.0"
#endif

/** @brief Where the command line options are stored */
struct settings_t
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  int popsize_arg;	/**< @brief population size.  */
  char * popsize_orig;	/**< @brief population size original value given at command line.  */
  const char *popsize_help; /**< @brief population size help description.  */
  int bits_arg;	/**< @brief environmental bits.  */
  char * bits_orig;	/**< @brief environmental bits original value given at command line.  */
  const char *bits_help; /**< @brief environmental bits help description.  */
  int steps_arg;	/**< @brief steps to run.  */
  char * steps_orig;	/**< @brief steps to run original value given at command line.  */
  const char *steps_help; /**< @brief steps to run help description.  */
  double mu_arg;	/**< @brief s.d. of mutations.  */
  char * mu_orig;	/**< @brief s.d. of mutations original value given at command line.  */
  const char *mu_help; /**< @brief s.d. of mutations help description.  */
  double alpha_arg;	/**< @brief fitness rolloff.  */
  char * alpha_orig;	/**< @brief fitness rolloff original value given at command line.  */
  const char *alpha_help; /**< @brief fitness rolloff help description.  */
  double omega0_arg;	/**< @brief initial metabolism.  */
  char * omega0_orig;	/**< @brief initial metabolism original value given at command line.  */
  const char *omega0_help; /**< @brief initial metabolism help description.  */
  double p_mut_arg;	/**< @brief prob. of bit mutation.  */
  char * p_mut_orig;	/**< @brief prob. of bit mutation original value given at command line.  */
  const char *p_mut_help; /**< @brief prob. of bit mutation help description.  */
  double p_switch_arg;	/**< @brief prob. of environment change.  */
  char * p_switch_orig;	/**< @brief prob. of environment change original value given at command line.  */
  const char *p_switch_help; /**< @brief prob. of environment change help description.  */
  double p_move_arg;	/**< @brief prob. of agent movement.  */
  char * p_move_orig;	/**< @brief prob. of agent movement original value given at command line.  */
  const char *p_move_help; /**< @brief prob. of agent movement help description.  */
  double p_noise_arg;	/**< @brief prob. of social learning noise.  */
  char * p_noise_orig;	/**< @brief prob. of social learning noise original value given at command line.  */
  const char *p_noise_help; /**< @brief prob. of social learning noise help description.  */
  double cost_soc_arg;	/**< @brief prob. of social learning noise.  */
  char * cost_soc_orig;	/**< @brief prob. of social learning noise original value given at command line.  */
  const char *cost_soc_help; /**< @brief prob. of social learning noise help description.  */
  char * logfile_arg;	/**< @brief path to store log file.  */
  char * logfile_orig;	/**< @brief path to store log file original value given at command line.  */
  const char *logfile_help; /**< @brief path to store log file help description.  */
  int log_flag;	/**< @brief logging on/off (default=on).  */
  const char *log_help; /**< @brief logging on/off help description.  */
  int debug_flag;	/**< @brief debug on/off (default=off).  */
  const char *debug_help; /**< @brief debug on/off help description.  */
  int batch_flag;	/**< @brief batch mode on/off (default=off).  */
  const char *batch_help; /**< @brief batch mode on/off help description.  */
  int metabolism_flag;	/**< @brief metabolism on/off (default=off).  */
  const char *metabolism_help; /**< @brief metabolism on/off help description.  */
  int perturbation_flag;	/**< @brief perturbation on/off (default=off).  */
  const char *perturbation_help; /**< @brief perturbation on/off help description.  */
  int perturbation_time_arg;	/**< @brief perturbation timestep.  */
  char * perturbation_time_orig;	/**< @brief perturbation timestep original value given at command line.  */
  const char *perturbation_time_help; /**< @brief perturbation timestep help description.  */
  double perturbation_size_arg;	/**< @brief perturbation magnitude.  */
  char * perturbation_size_orig;	/**< @brief perturbation magnitude original value given at command line.  */
  const char *perturbation_size_help; /**< @brief perturbation magnitude help description.  */
  char * conf_file_arg;	/**< @brief config file to read (default='settings.conf').  */
  char * conf_file_orig;	/**< @brief config file to read original value given at command line.  */
  const char *conf_file_help; /**< @brief config file to read help description.  */
  
  unsigned int help_given ;	/**< @brief Whether help was given.  */
  unsigned int version_given ;	/**< @brief Whether version was given.  */
  unsigned int popsize_given ;	/**< @brief Whether popsize was given.  */
  unsigned int bits_given ;	/**< @brief Whether bits was given.  */
  unsigned int steps_given ;	/**< @brief Whether steps was given.  */
  unsigned int mu_given ;	/**< @brief Whether mu was given.  */
  unsigned int alpha_given ;	/**< @brief Whether alpha was given.  */
  unsigned int omega0_given ;	/**< @brief Whether omega0 was given.  */
  unsigned int p_mut_given ;	/**< @brief Whether p_mut was given.  */
  unsigned int p_switch_given ;	/**< @brief Whether p_switch was given.  */
  unsigned int p_move_given ;	/**< @brief Whether p_move was given.  */
  unsigned int p_noise_given ;	/**< @brief Whether p_noise was given.  */
  unsigned int cost_soc_given ;	/**< @brief Whether cost_soc was given.  */
  unsigned int logfile_given ;	/**< @brief Whether logfile was given.  */
  unsigned int log_given ;	/**< @brief Whether log was given.  */
  unsigned int debug_given ;	/**< @brief Whether debug was given.  */
  unsigned int batch_given ;	/**< @brief Whether batch was given.  */
  unsigned int metabolism_given ;	/**< @brief Whether metabolism was given.  */
  unsigned int perturbation_given ;	/**< @brief Whether perturbation was given.  */
  unsigned int perturbation_time_given ;	/**< @brief Whether perturbation_time was given.  */
  unsigned int perturbation_size_given ;	/**< @brief Whether perturbation_size was given.  */
  unsigned int conf_file_given ;	/**< @brief Whether conf-file was given.  */

} ;

/** @brief The additional parameters to pass to parser functions */
struct config_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure settings_t (default 1) */
  int check_required; /**< @brief whether to check that all required options were provided (default 1) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure settings_t (default 0) */
  int print_errors; /**< @brief whether getopt_long should print an error message for a bad option (default 1) */
} ;

/** @brief the purpose string of the program */
extern const char *settings_t_purpose;
/** @brief the usage string of the program */
extern const char *settings_t_usage;
/** @brief all the lines making the help output */
extern const char *settings_t_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int config_parser (int argc, char **argv,
  struct settings_t *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use config_parser_ext() instead
 */
int config_parser2 (int argc, char **argv,
  struct settings_t *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int config_parser_ext (int argc, char **argv,
  struct settings_t *args_info,
  struct config_parser_params *params);

/**
 * Save the contents of the option struct into an already open FILE stream.
 * @param outfile the stream where to dump options
 * @param args_info the option struct to dump
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int config_parser_dump(FILE *outfile,
  struct settings_t *args_info);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int config_parser_file_save(const char *filename,
  struct settings_t *args_info);

/**
 * Print the help
 */
void config_parser_print_help(void);
/**
 * Print the version
 */
void config_parser_print_version(void);

/**
 * Initializes all the fields a config_parser_params structure 
 * to their default values
 * @param params the structure to initialize
 */
void config_parser_params_init(struct config_parser_params *params);

/**
 * Allocates dynamically a config_parser_params structure and initializes
 * all its fields to their default values
 * @return the created and initialized config_parser_params structure
 */
struct config_parser_params *config_parser_params_create(void);

/**
 * Initializes the passed settings_t structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void config_parser_init (struct settings_t *args_info);
/**
 * Deallocates the string fields of the settings_t structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void config_parser_free (struct settings_t *args_info);

/**
 * The config file parser (deprecated version)
 * @param filename the name of the config file
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use config_parser_config_file() instead
 */
int config_parser_configfile (const char *filename,
  struct settings_t *args_info,
  int override, int initialize, int check_required);

/**
 * The config file parser
 * @param filename the name of the config file
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int config_parser_config_file (const char *filename,
  struct settings_t *args_info,
  struct config_parser_params *params);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int config_parser_required (struct settings_t *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SETTINGS_H */
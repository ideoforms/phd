/*
  File autogenerated by gengetopt version 2.22.5
  generated with the following command:
  gengetopt --output-dir=sim/config -f config_parser -F settings -a settings_t -C 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "settings.h"

const char *settings_t_purpose = "";

const char *settings_t_usage = "Usage: imitation-cpp [OPTIONS]...";

const char *settings_t_description = "";

const char *settings_t_help[] = {
  "  -h, --help                    Print help and exit",
  "  -V, --version                 Print version and exit",
  "  -N, --popsize=INT             population size",
  "  -B, --bits=INT                environmental bits",
  "  -s, --steps=INT               steps to run",
  "  -m, --mu=DOUBLE               s.d. of mutations",
  "  -a, --alpha=DOUBLE            fitness rolloff",
  "  -O, --omega0=DOUBLE           initial metabolism",
  "      --p_mut=DOUBLE            prob. of bit mutation",
  "      --p_switch=DOUBLE         prob. of environment change",
  "      --p_move=DOUBLE           prob. of agent movement",
  "      --p_noise=DOUBLE          prob. of social learning noise",
  "      --cost_soc=DOUBLE         prob. of social learning noise",
  "  -L, --logfile=STRING          path to store log file",
  "  -l, --log                     logging on/off  (default=on)",
  "  -d, --debug                   debug on/off  (default=off)",
  "  -b, --batch                   batch mode on/off  (default=off)",
  "      --metabolism              metabolism on/off  (default=off)",
  "      --perturbation            perturbation on/off  (default=off)",
  "      --perturbation_time=INT   perturbation timestep",
  "      --perturbation_size=DOUBLE\n                                perturbation magnitude",
  "  -C, --conf-file=STRING        config file to read  (default=`settings.conf')",
    0
};

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_INT
  , ARG_DOUBLE
} config_parser_arg_type;

static
void clear_given (struct settings_t *args_info);
static
void clear_args (struct settings_t *args_info);

static int
config_parser_internal (int argc, char **argv, struct settings_t *args_info,
                        struct config_parser_params *params, const char *additional_error);

struct line_list
{
  char * string_arg;
  struct line_list * next;
};

static struct line_list *cmd_line_list = 0;
static struct line_list *cmd_line_list_tmp = 0;

static void
free_cmd_list(void)
{
  /* free the list of a previous call */
  if (cmd_line_list)
    {
      while (cmd_line_list) {
        cmd_line_list_tmp = cmd_line_list;
        cmd_line_list = cmd_line_list->next;
        free (cmd_line_list_tmp->string_arg);
        free (cmd_line_list_tmp);
      }
    }
}


static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct settings_t *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->popsize_given = 0 ;
  args_info->bits_given = 0 ;
  args_info->steps_given = 0 ;
  args_info->mu_given = 0 ;
  args_info->alpha_given = 0 ;
  args_info->omega0_given = 0 ;
  args_info->p_mut_given = 0 ;
  args_info->p_switch_given = 0 ;
  args_info->p_move_given = 0 ;
  args_info->p_noise_given = 0 ;
  args_info->cost_soc_given = 0 ;
  args_info->logfile_given = 0 ;
  args_info->log_given = 0 ;
  args_info->debug_given = 0 ;
  args_info->batch_given = 0 ;
  args_info->metabolism_given = 0 ;
  args_info->perturbation_given = 0 ;
  args_info->perturbation_time_given = 0 ;
  args_info->perturbation_size_given = 0 ;
  args_info->conf_file_given = 0 ;
}

static
void clear_args (struct settings_t *args_info)
{
  FIX_UNUSED (args_info);
  args_info->popsize_orig = NULL;
  args_info->bits_orig = NULL;
  args_info->steps_orig = NULL;
  args_info->mu_orig = NULL;
  args_info->alpha_orig = NULL;
  args_info->omega0_orig = NULL;
  args_info->p_mut_orig = NULL;
  args_info->p_switch_orig = NULL;
  args_info->p_move_orig = NULL;
  args_info->p_noise_orig = NULL;
  args_info->cost_soc_orig = NULL;
  args_info->logfile_arg = NULL;
  args_info->logfile_orig = NULL;
  args_info->log_flag = 1;
  args_info->debug_flag = 0;
  args_info->batch_flag = 0;
  args_info->metabolism_flag = 0;
  args_info->perturbation_flag = 0;
  args_info->perturbation_time_orig = NULL;
  args_info->perturbation_size_orig = NULL;
  args_info->conf_file_arg = gengetopt_strdup ("settings.conf");
  args_info->conf_file_orig = NULL;
  
}

static
void init_args_info(struct settings_t *args_info)
{


  args_info->help_help = settings_t_help[0] ;
  args_info->version_help = settings_t_help[1] ;
  args_info->popsize_help = settings_t_help[2] ;
  args_info->bits_help = settings_t_help[3] ;
  args_info->steps_help = settings_t_help[4] ;
  args_info->mu_help = settings_t_help[5] ;
  args_info->alpha_help = settings_t_help[6] ;
  args_info->omega0_help = settings_t_help[7] ;
  args_info->p_mut_help = settings_t_help[8] ;
  args_info->p_switch_help = settings_t_help[9] ;
  args_info->p_move_help = settings_t_help[10] ;
  args_info->p_noise_help = settings_t_help[11] ;
  args_info->cost_soc_help = settings_t_help[12] ;
  args_info->logfile_help = settings_t_help[13] ;
  args_info->log_help = settings_t_help[14] ;
  args_info->debug_help = settings_t_help[15] ;
  args_info->batch_help = settings_t_help[16] ;
  args_info->metabolism_help = settings_t_help[17] ;
  args_info->perturbation_help = settings_t_help[18] ;
  args_info->perturbation_time_help = settings_t_help[19] ;
  args_info->perturbation_size_help = settings_t_help[20] ;
  args_info->conf_file_help = settings_t_help[21] ;
  
}

void
config_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(CONFIG_PARSER_PACKAGE_NAME) ? CONFIG_PARSER_PACKAGE_NAME : CONFIG_PARSER_PACKAGE),
     CONFIG_PARSER_VERSION);
}

static void print_help_common(void) {
  config_parser_print_version ();

  if (strlen(settings_t_purpose) > 0)
    printf("\n%s\n", settings_t_purpose);

  if (strlen(settings_t_usage) > 0)
    printf("\n%s\n", settings_t_usage);

  printf("\n");

  if (strlen(settings_t_description) > 0)
    printf("%s\n\n", settings_t_description);
}

void
config_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (settings_t_help[i])
    printf("%s\n", settings_t_help[i++]);
}

void
config_parser_init (struct settings_t *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);
}

void
config_parser_params_init(struct config_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct config_parser_params *
config_parser_params_create(void)
{
  struct config_parser_params *params = 
    (struct config_parser_params *)malloc(sizeof(struct config_parser_params));
  config_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}


static void
config_parser_release (struct settings_t *args_info)
{

  free_string_field (&(args_info->popsize_orig));
  free_string_field (&(args_info->bits_orig));
  free_string_field (&(args_info->steps_orig));
  free_string_field (&(args_info->mu_orig));
  free_string_field (&(args_info->alpha_orig));
  free_string_field (&(args_info->omega0_orig));
  free_string_field (&(args_info->p_mut_orig));
  free_string_field (&(args_info->p_switch_orig));
  free_string_field (&(args_info->p_move_orig));
  free_string_field (&(args_info->p_noise_orig));
  free_string_field (&(args_info->cost_soc_orig));
  free_string_field (&(args_info->logfile_arg));
  free_string_field (&(args_info->logfile_orig));
  free_string_field (&(args_info->perturbation_time_orig));
  free_string_field (&(args_info->perturbation_size_orig));
  free_string_field (&(args_info->conf_file_arg));
  free_string_field (&(args_info->conf_file_orig));
  
  

  clear_given (args_info);
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  FIX_UNUSED (values);
  if (arg) {
    fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
config_parser_dump(FILE *outfile, struct settings_t *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CONFIG_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->popsize_given)
    write_into_file(outfile, "popsize", args_info->popsize_orig, 0);
  if (args_info->bits_given)
    write_into_file(outfile, "bits", args_info->bits_orig, 0);
  if (args_info->steps_given)
    write_into_file(outfile, "steps", args_info->steps_orig, 0);
  if (args_info->mu_given)
    write_into_file(outfile, "mu", args_info->mu_orig, 0);
  if (args_info->alpha_given)
    write_into_file(outfile, "alpha", args_info->alpha_orig, 0);
  if (args_info->omega0_given)
    write_into_file(outfile, "omega0", args_info->omega0_orig, 0);
  if (args_info->p_mut_given)
    write_into_file(outfile, "p_mut", args_info->p_mut_orig, 0);
  if (args_info->p_switch_given)
    write_into_file(outfile, "p_switch", args_info->p_switch_orig, 0);
  if (args_info->p_move_given)
    write_into_file(outfile, "p_move", args_info->p_move_orig, 0);
  if (args_info->p_noise_given)
    write_into_file(outfile, "p_noise", args_info->p_noise_orig, 0);
  if (args_info->cost_soc_given)
    write_into_file(outfile, "cost_soc", args_info->cost_soc_orig, 0);
  if (args_info->logfile_given)
    write_into_file(outfile, "logfile", args_info->logfile_orig, 0);
  if (args_info->log_given)
    write_into_file(outfile, "log", 0, 0 );
  if (args_info->debug_given)
    write_into_file(outfile, "debug", 0, 0 );
  if (args_info->batch_given)
    write_into_file(outfile, "batch", 0, 0 );
  if (args_info->metabolism_given)
    write_into_file(outfile, "metabolism", 0, 0 );
  if (args_info->perturbation_given)
    write_into_file(outfile, "perturbation", 0, 0 );
  if (args_info->perturbation_time_given)
    write_into_file(outfile, "perturbation_time", args_info->perturbation_time_orig, 0);
  if (args_info->perturbation_size_given)
    write_into_file(outfile, "perturbation_size", args_info->perturbation_size_orig, 0);
  if (args_info->conf_file_given)
    write_into_file(outfile, "conf-file", args_info->conf_file_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
config_parser_file_save(const char *filename, struct settings_t *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CONFIG_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = config_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
config_parser_free (struct settings_t *args_info)
{
  config_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
config_parser (int argc, char **argv, struct settings_t *args_info)
{
  return config_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
config_parser_ext (int argc, char **argv, struct settings_t *args_info,
                   struct config_parser_params *params)
{
  int result;
  result = config_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      config_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
config_parser2 (int argc, char **argv, struct settings_t *args_info, int override, int initialize, int check_required)
{
  int result;
  struct config_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = config_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      config_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
config_parser_required (struct settings_t *args_info, const char *prog_name)
{
  FIX_UNUSED (args_info);
  FIX_UNUSED (prog_name);
  return EXIT_SUCCESS;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see config_parser_params.check_ambiguity
 * @param override @see config_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               config_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  FIX_UNUSED (default_value);
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_FLAG:
    *((int *)field) = !*((int *)field);
    break;
  case ARG_INT:
    if (val) *((int *)field) = strtol (val, &stop_char, 0);
    break;
  case ARG_DOUBLE:
    if (val) *((double *)field) = strtod (val, &stop_char);
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_INT:
  case ARG_DOUBLE:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
  case ARG_FLAG:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}


int
config_parser_internal (
  int argc, char **argv, struct settings_t *args_info,
                        struct config_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error = 0;
  struct settings_t local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    config_parser_init (args_info);

  config_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "popsize",	1, NULL, 'N' },
        { "bits",	1, NULL, 'B' },
        { "steps",	1, NULL, 's' },
        { "mu",	1, NULL, 'm' },
        { "alpha",	1, NULL, 'a' },
        { "omega0",	1, NULL, 'O' },
        { "p_mut",	1, NULL, 0 },
        { "p_switch",	1, NULL, 0 },
        { "p_move",	1, NULL, 0 },
        { "p_noise",	1, NULL, 0 },
        { "cost_soc",	1, NULL, 0 },
        { "logfile",	1, NULL, 'L' },
        { "log",	0, NULL, 'l' },
        { "debug",	0, NULL, 'd' },
        { "batch",	0, NULL, 'b' },
        { "metabolism",	0, NULL, 0 },
        { "perturbation",	0, NULL, 0 },
        { "perturbation_time",	1, NULL, 0 },
        { "perturbation_size",	1, NULL, 0 },
        { "conf-file",	1, NULL, 'C' },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVN:B:s:m:a:O:L:ldbC:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          config_parser_print_help ();
          config_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          config_parser_print_version ();
          config_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'N':	/* population size.  */
        
        
          if (update_arg( (void *)&(args_info->popsize_arg), 
               &(args_info->popsize_orig), &(args_info->popsize_given),
              &(local_args_info.popsize_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "popsize", 'N',
              additional_error))
            goto failure;
        
          break;
        case 'B':	/* environmental bits.  */
        
        
          if (update_arg( (void *)&(args_info->bits_arg), 
               &(args_info->bits_orig), &(args_info->bits_given),
              &(local_args_info.bits_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "bits", 'B',
              additional_error))
            goto failure;
        
          break;
        case 's':	/* steps to run.  */
        
        
          if (update_arg( (void *)&(args_info->steps_arg), 
               &(args_info->steps_orig), &(args_info->steps_given),
              &(local_args_info.steps_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "steps", 's',
              additional_error))
            goto failure;
        
          break;
        case 'm':	/* s.d. of mutations.  */
        
        
          if (update_arg( (void *)&(args_info->mu_arg), 
               &(args_info->mu_orig), &(args_info->mu_given),
              &(local_args_info.mu_given), optarg, 0, 0, ARG_DOUBLE,
              check_ambiguity, override, 0, 0,
              "mu", 'm',
              additional_error))
            goto failure;
        
          break;
        case 'a':	/* fitness rolloff.  */
        
        
          if (update_arg( (void *)&(args_info->alpha_arg), 
               &(args_info->alpha_orig), &(args_info->alpha_given),
              &(local_args_info.alpha_given), optarg, 0, 0, ARG_DOUBLE,
              check_ambiguity, override, 0, 0,
              "alpha", 'a',
              additional_error))
            goto failure;
        
          break;
        case 'O':	/* initial metabolism.  */
        
        
          if (update_arg( (void *)&(args_info->omega0_arg), 
               &(args_info->omega0_orig), &(args_info->omega0_given),
              &(local_args_info.omega0_given), optarg, 0, 0, ARG_DOUBLE,
              check_ambiguity, override, 0, 0,
              "omega0", 'O',
              additional_error))
            goto failure;
        
          break;
        case 'L':	/* path to store log file.  */
        
        
          if (update_arg( (void *)&(args_info->logfile_arg), 
               &(args_info->logfile_orig), &(args_info->logfile_given),
              &(local_args_info.logfile_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "logfile", 'L',
              additional_error))
            goto failure;
        
          break;
        case 'l':	/* logging on/off.  */
        
        
          if (update_arg((void *)&(args_info->log_flag), 0, &(args_info->log_given),
              &(local_args_info.log_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "log", 'l',
              additional_error))
            goto failure;
        
          break;
        case 'd':	/* debug on/off.  */
        
        
          if (update_arg((void *)&(args_info->debug_flag), 0, &(args_info->debug_given),
              &(local_args_info.debug_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "debug", 'd',
              additional_error))
            goto failure;
        
          break;
        case 'b':	/* batch mode on/off.  */
        
        
          if (update_arg((void *)&(args_info->batch_flag), 0, &(args_info->batch_given),
              &(local_args_info.batch_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "batch", 'b',
              additional_error))
            goto failure;
        
          break;
        case 'C':	/* config file to read.  */
        
        
          if (update_arg( (void *)&(args_info->conf_file_arg), 
               &(args_info->conf_file_orig), &(args_info->conf_file_given),
              &(local_args_info.conf_file_given), optarg, 0, "settings.conf", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "conf-file", 'C',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          /* prob. of bit mutation.  */
          if (strcmp (long_options[option_index].name, "p_mut") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->p_mut_arg), 
                 &(args_info->p_mut_orig), &(args_info->p_mut_given),
                &(local_args_info.p_mut_given), optarg, 0, 0, ARG_DOUBLE,
                check_ambiguity, override, 0, 0,
                "p_mut", '-',
                additional_error))
              goto failure;
          
          }
          /* prob. of environment change.  */
          else if (strcmp (long_options[option_index].name, "p_switch") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->p_switch_arg), 
                 &(args_info->p_switch_orig), &(args_info->p_switch_given),
                &(local_args_info.p_switch_given), optarg, 0, 0, ARG_DOUBLE,
                check_ambiguity, override, 0, 0,
                "p_switch", '-',
                additional_error))
              goto failure;
          
          }
          /* prob. of agent movement.  */
          else if (strcmp (long_options[option_index].name, "p_move") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->p_move_arg), 
                 &(args_info->p_move_orig), &(args_info->p_move_given),
                &(local_args_info.p_move_given), optarg, 0, 0, ARG_DOUBLE,
                check_ambiguity, override, 0, 0,
                "p_move", '-',
                additional_error))
              goto failure;
          
          }
          /* prob. of social learning noise.  */
          else if (strcmp (long_options[option_index].name, "p_noise") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->p_noise_arg), 
                 &(args_info->p_noise_orig), &(args_info->p_noise_given),
                &(local_args_info.p_noise_given), optarg, 0, 0, ARG_DOUBLE,
                check_ambiguity, override, 0, 0,
                "p_noise", '-',
                additional_error))
              goto failure;
          
          }
          /* prob. of social learning noise.  */
          else if (strcmp (long_options[option_index].name, "cost_soc") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->cost_soc_arg), 
                 &(args_info->cost_soc_orig), &(args_info->cost_soc_given),
                &(local_args_info.cost_soc_given), optarg, 0, 0, ARG_DOUBLE,
                check_ambiguity, override, 0, 0,
                "cost_soc", '-',
                additional_error))
              goto failure;
          
          }
          /* metabolism on/off.  */
          else if (strcmp (long_options[option_index].name, "metabolism") == 0)
          {
          
          
            if (update_arg((void *)&(args_info->metabolism_flag), 0, &(args_info->metabolism_given),
                &(local_args_info.metabolism_given), optarg, 0, 0, ARG_FLAG,
                check_ambiguity, override, 1, 0, "metabolism", '-',
                additional_error))
              goto failure;
          
          }
          /* perturbation on/off.  */
          else if (strcmp (long_options[option_index].name, "perturbation") == 0)
          {
          
          
            if (update_arg((void *)&(args_info->perturbation_flag), 0, &(args_info->perturbation_given),
                &(local_args_info.perturbation_given), optarg, 0, 0, ARG_FLAG,
                check_ambiguity, override, 1, 0, "perturbation", '-',
                additional_error))
              goto failure;
          
          }
          /* perturbation timestep.  */
          else if (strcmp (long_options[option_index].name, "perturbation_time") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->perturbation_time_arg), 
                 &(args_info->perturbation_time_orig), &(args_info->perturbation_time_given),
                &(local_args_info.perturbation_time_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "perturbation_time", '-',
                additional_error))
              goto failure;
          
          }
          /* perturbation magnitude.  */
          else if (strcmp (long_options[option_index].name, "perturbation_size") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->perturbation_size_arg), 
                 &(args_info->perturbation_size_orig), &(args_info->perturbation_size_given),
                &(local_args_info.perturbation_size_given), optarg, 0, 0, ARG_DOUBLE,
                check_ambiguity, override, 0, 0,
                "perturbation_size", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CONFIG_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */




  config_parser_release (&local_args_info);

  if ( error )
    return (EXIT_FAILURE);

  return 0;

failure:
  
  config_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}

#ifndef CONFIG_FILE_LINE_SIZE
#define CONFIG_FILE_LINE_SIZE 2048
#endif
#define ADDITIONAL_ERROR " in configuration file "

#define CONFIG_FILE_LINE_BUFFER_SIZE (CONFIG_FILE_LINE_SIZE+3)
/* 3 is for "--" and "=" */

static int
_config_parser_configfile (const char *filename, int *my_argc)
{
  FILE* file;
  char my_argv[CONFIG_FILE_LINE_BUFFER_SIZE+1];
  char linebuf[CONFIG_FILE_LINE_SIZE];
  int line_num = 0;
  int result = 0, equal;
  char *fopt, *farg;
  char *str_index;
  size_t len, next_token;
  char delimiter;

  if ((file = fopen(filename, "r")) == 0)
    {
      fprintf (stderr, "%s: Error opening configuration file '%s'\n",
               CONFIG_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  while ((fgets(linebuf, CONFIG_FILE_LINE_SIZE, file)) != 0)
    {
      ++line_num;
      my_argv[0] = '\0';
      len = strlen(linebuf);
      if (len > (CONFIG_FILE_LINE_BUFFER_SIZE-1))
        {
          fprintf (stderr, "%s:%s:%d: Line too long in configuration file\n",
                   CONFIG_PARSER_PACKAGE, filename, line_num);
          result = EXIT_FAILURE;
          break;
        }

      /* find first non-whitespace character in the line */
      next_token = strspn (linebuf, " \t\r\n");
      str_index  = linebuf + next_token;

      if ( str_index[0] == '\0' || str_index[0] == '#')
        continue; /* empty line or comment line is skipped */

      fopt = str_index;

      /* truncate fopt at the end of the first non-valid character */
      next_token = strcspn (fopt, " \t\r\n=");

      if (fopt[next_token] == '\0') /* the line is over */
        {
          farg  = 0;
          equal = 0;
          goto noarg;
        }

      /* remember if equal sign is present */
      equal = (fopt[next_token] == '=');
      fopt[next_token++] = '\0';

      /* advance pointers to the next token after the end of fopt */
      next_token += strspn (fopt + next_token, " \t\r\n");

      /* check for the presence of equal sign, and if so, skip it */
      if ( !equal )
        if ((equal = (fopt[next_token] == '=')))
          {
            next_token++;
            next_token += strspn (fopt + next_token, " \t\r\n");
          }
      str_index  += next_token;

      /* find argument */
      farg = str_index;
      if ( farg[0] == '\"' || farg[0] == '\'' )
        { /* quoted argument */
          str_index = strchr (++farg, str_index[0] ); /* skip opening quote */
          if (! str_index)
            {
              fprintf
                (stderr,
                 "%s:%s:%d: unterminated string in configuration file\n",
                 CONFIG_PARSER_PACKAGE, filename, line_num);
              result = EXIT_FAILURE;
              break;
            }
        }
      else
        { /* read up the remaining part up to a delimiter */
          next_token = strcspn (farg, " \t\r\n#\'\"");
          str_index += next_token;
        }

      /* truncate farg at the delimiter and store it for further check */
      delimiter = *str_index, *str_index++ = '\0';

      /* everything but comment is illegal at the end of line */
      if (delimiter != '\0' && delimiter != '#')
        {
          str_index += strspn(str_index, " \t\r\n");
          if (*str_index != '\0' && *str_index != '#')
            {
              fprintf
                (stderr,
                 "%s:%s:%d: malformed string in configuration file\n",
                 CONFIG_PARSER_PACKAGE, filename, line_num);
              result = EXIT_FAILURE;
              break;
            }
        }

    noarg:
      if (!strcmp(fopt,"include")) {
        if (farg && *farg) {
          result = _config_parser_configfile(farg, my_argc);
        } else {
          fprintf(stderr, "%s:%s:%d: include requires a filename argument.\n",
                  CONFIG_PARSER_PACKAGE, filename, line_num);
        }
        continue;
      }
      len = strlen(fopt);
      strcat (my_argv, len > 1 ? "--" : "-");
      strcat (my_argv, fopt);
      if (len > 1 && ((farg && *farg) || equal))
        strcat (my_argv, "=");
      if (farg && *farg)
        strcat (my_argv, farg);
      ++(*my_argc);

      cmd_line_list_tmp = (struct line_list *) malloc (sizeof (struct line_list));
      cmd_line_list_tmp->next = cmd_line_list;
      cmd_line_list = cmd_line_list_tmp;
      cmd_line_list->string_arg = gengetopt_strdup(my_argv);
    } /* while */

  if (file)
    fclose(file);
  return result;
}

int
config_parser_configfile (
  const char *filename,
                           struct settings_t *args_info,
                           int override, int initialize, int check_required)
{
  struct config_parser_params params;

  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;
  
  return config_parser_config_file (filename, args_info, &params);
}

int
config_parser_config_file (const char *filename,
                           struct settings_t *args_info,
                           struct config_parser_params *params)
{
  int i, result;
  int my_argc = 1;
  char **my_argv_arg;
  char *additional_error;

  /* store the program name */
  cmd_line_list_tmp = (struct line_list *) malloc (sizeof (struct line_list));
  cmd_line_list_tmp->next = cmd_line_list;
  cmd_line_list = cmd_line_list_tmp;
  cmd_line_list->string_arg = gengetopt_strdup (CONFIG_PARSER_PACKAGE);

  result = _config_parser_configfile(filename, &my_argc);

  if (result != EXIT_FAILURE) {
    my_argv_arg = (char **) malloc((my_argc+1) * sizeof(char *));
    cmd_line_list_tmp = cmd_line_list;

    for (i = my_argc - 1; i >= 0; --i) {
      my_argv_arg[i] = cmd_line_list_tmp->string_arg;
      cmd_line_list_tmp = cmd_line_list_tmp->next;
    }

    my_argv_arg[my_argc] = 0;

    additional_error = (char *)malloc(strlen(filename) + strlen(ADDITIONAL_ERROR) + 1);
    strcpy (additional_error, ADDITIONAL_ERROR);
    strcat (additional_error, filename);
    result =
      config_parser_internal (my_argc, my_argv_arg, args_info,
                              params,
                              additional_error);

    free (additional_error);
    free (my_argv_arg);
  }

  free_cmd_list();
  if (result == EXIT_FAILURE)
    {
      config_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

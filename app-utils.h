//int main(int argc, char *argv[]);

void init_config(int argc, char *argv[]);
void init_simulator();
void init_logging();

void log_states();
void log_phenotypes();

void close_logging();

void print_elapsed_time(int trial_index, int trial_total);
double millitime();
double predict(double elapsed, int index, int total);

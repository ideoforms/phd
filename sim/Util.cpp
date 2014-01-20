/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *--------------------------------------------------------------------*/

#include "sim/Util.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"

#include <limits.h>

namespace sim
{

/*--------------------------------------------------------------------*
 * Maintain a single global RNG state object.
 *--------------------------------------------------------------------*/
gsl_rng *rng;


/*--------------------------------------------------------------------*
 * rng_init(): Initialise pseudo-random number generator.
 *
 * Uses the GNU Scientific Library:
 *   <http://www.gnu.org/software/gsl/>
 *
 *--------------------------------------------------------------------*/
void rng_init()
{
	struct timeval tv;
	
	gsl_rng_env_setup();
	rng = gsl_rng_alloc(gsl_rng_default);
	
	/*--------------------------------------------------------------------*
	 * Seed with current time multiplied by microsecond part, to give
	 * a pretty decent non-correlated seed.
	 *--------------------------------------------------------------------*/
	gettimeofday(&tv, 0);

	gsl_rng_set(rng, tv.tv_sec * tv.tv_usec);
}

/*--------------------------------------------------------------------*
 * Generate from gaussian distribution.
 *--------------------------------------------------------------------*/
double rng_gaussian(double mean, double sd)
{
	double value = mean + gsl_ran_gaussian(rng, sd);
	return value;
}

double rng_gaussian()
{
	double value = gsl_ran_gaussian(rng, 1);
	return value;
}


/*--------------------------------------------------------------------*
 * Generate uniform rand.
 *--------------------------------------------------------------------*/
double rng_uniformuf()
{
	double value = gsl_rng_uniform(rng);
	return value;
}

double rng_uniform(double to)
{
	double value = gsl_rng_uniform(rng) * to;
	return value;
}

double rng_uniform(double from, double to)
{
	double value = gsl_rng_uniform(rng);
	value = value * (to - from) + from;
	return value;
}

bool rng_coin(double limit = 0.5)
{
	double value = gsl_rng_uniform(rng);
	return value < limit;
}


/*--------------------------------------------------------------------*
 * rng_randint: random int, up to (but excluding) to
 *--------------------------------------------------------------------*/
unsigned long rng_randint(unsigned long to)
{
	return gsl_rng_get(rng) % to;
}

unsigned long rng_randint(unsigned long from, unsigned long to)
{
	return from + (gsl_rng_get(rng) % (to - from));
}

/*--------------------------------------------------------------------*
 * rng_shuffle: fisher-yates shuffle, in-place
 *--------------------------------------------------------------------*/
void rng_shuffle(int *values, int count)
{
	for (int i = count - 1; i >= 0; i--)
	{
		int j = rng_randint(i + 1);
		if (i == j)
			continue;

		int tmp = values[i];
		values[i] = values[j];
		values[j] = tmp;
	}
}

/*--------------------------------------------------------------------*
 * roulette(): roulette-wheel selector.
 * Returns an index of values() weighted by value.
 *--------------------------------------------------------------------*/
int roulette(double *values, int count)
{
	double total = 0;
	for (int i = 0; i < count; i++)
		total += values[i];

	double target = rng_uniform(total);
	double cumsum = 0;

	for (int i = 0; i < count; i++)
	{
		cumsum += values[i];
		if (cumsum > target)
			return i;
	}

	return -1;
}

/*--------------------------------------------------------------------*
 * wroulette(): weighted roulette-wheel.
 *--------------------------------------------------------------------*/
int wroulette(double *values, int count, double weight)
{
	double weights[count];
	if (weight >= 0)
	{
		for (int i = 0; i < count; i++)
			weights[i] = 1.0 + weight * values[i];
	}
	else
	{
		/* fabs not needed as w is always < 0 - right ?? */
		for (int i = 0; i < count; i++)
			weights[i] = 1.0 / (1.0 + fabs(weight) * values[i]);
	}
	return roulette(weights, count);
}

/*--------------------------------------------------------------------*
 * timestamp(): Return microsecond-accurate timestamp.
 * Useful for monitoring function runtimes.
 *--------------------------------------------------------------------*/
double timestamp()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (tv.tv_usec / 1000000.0);	
}

/*--------------------------------------------------------------------*
 * clip(): Constrain a value within two bounds.
 *--------------------------------------------------------------------*/
double clip(double value, double min, double max)
{
	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}

/*--------------------------------------------------------------------*
 * map(): Map a value onto a linear range.
 *--------------------------------------------------------------------*/
double map(double value, double fromA, double fromB, double toA, double toB)
{
	double norm = (value - fromA) / (fromB - fromA);
	return toA + norm * (toB - toA);
}

/*--------------------------------------------------------------------*
 * linexp(): Map a value onto an exponential range.
 *--------------------------------------------------------------------*/
double linexp (double value, double a, double b, double c, double d)
{
    if (value <= a) return c;
    if (value >= b) return d;
    return pow(d/c, (value-a)/(b-a)) * c;
}


/*--------------------------------------------------------------------*
 * normalize(): In-place array normalize 
 *--------------------------------------------------------------------*/
void normalize(double *values, int count)
{
	double min = INT_MAX, max = -INT_MAX;

	for (int i = 0; i < count; i++)
	{
		if (values[i] < min)
			min = values[i];
		if (values[i] > max)
			max = values[i];
	}

	for (int i = 0; i < count; i++)
		values[i] = map(values[i], min, max, 0, 1);
}

/*--------------------------------------------------------------------*
 * mean(): Array mean
 *--------------------------------------------------------------------*/
    
double mean(double *values, int count)
{
    double total = 0.0;
    for (int i = 0; i < count; i++)
        total += values[i];
    return total / count;
}

    
double stddev(double *values, int count, double mean)
{
    double total = 0.0;
    for (int i = 0; i < count; i++)
    {
        double diff = values[i] - mean;
        total += diff * diff;
    };
    return sqrt(total);
}

    
double stddev(double *values, int count)
{
    double m = mean(values, count);
    return stddev(values, count, m);
}

/*--------------------------------------------------------------------*
 * trace(): Output status info to stdout.
 *--------------------------------------------------------------------*/
void trace(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}


/*--------------------------------------------------------------------*
 * debug(): Output status info to stdout if DEBUG is set.
 *--------------------------------------------------------------------*/
void debug(const char *fmt, ...)
{
#ifdef DEBUG
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

}

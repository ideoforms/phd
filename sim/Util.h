/*--------------------------------------------------------------------*
 * Util.cpp: Helper utilities.
 *
 * $Id: util.h 15 2010-08-19 14:25:10Z daniel $
 *--------------------------------------------------------------------*/

#pragma once

namespace sim
{

void rng_init();

double rng_gaussian(double mean, double sd);
double rng_gaussian();
double rng_uniform(double to = 1.0);
double rng_uniform(double from, double to);
double rng_uniformuf();
unsigned long rng_randint(unsigned long to);
unsigned long rng_randint(unsigned long from, unsigned long to);
bool rng_coin(double limit);
void rng_shuffle(int *values, int count);

int roulette(double *values, int count);

double timestamp();

double clip(double value, double min, double max);
double map(double value, double fromA, double fromB, double toA, double toB);
double linexp (double value, double a, double b, double c, double d);

void normalize(double *values, int count);
    
double mean(double *values, int count);
double stddev(double *values, int count, double mean);
double stddev(double *values, int count);

void trace(const char *fmt, ...);
void debug(const char *fmt, ...);

}

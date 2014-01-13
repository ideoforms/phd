//
//  Perlin2D.cpp
//  LandscapeGenerator
//
//  Created by Daniel Jones on 12/01/2014.
//
//

#include "LandscapeGenerator.h"
#include "sim/Util.h"
#include <math.h>

using namespace sim;

LandscapeGenerator::LandscapeGenerator(float detail, float gradient, float abundance)
{
    this->mDetail = detail;
    this->mGradient = gradient;
    this->mAbundance = abundance;
    
    this->mPerlin = new Perlin2D(16);
    this->mPerlin->mOctaves = 1;
    this->mPerlin->mRolloff = 0.5;
    this->generate();
}

void LandscapeGenerator::generate()
{
    this->mPerlin->init();
}

float LandscapeGenerator::get(float x, float y)
{
    float value = mPerlin->get((float) x * 16.0 * mDetail, (float) y * 16.0 * mDetail);
    if (value < -0.5) value = -0.5;
    if (value > 0.5) value = 0.5;
    // value = tanh(value * (1 + 5 * (1 - mGradient))) + mAbundance;
    value = tanh(value * pow(10, mGradient * 2 - 1)) + mAbundance;
    return value;
}

Perlin2D::Perlin2D(int width)
{
    this->mWidth = width;
}

void Perlin2D::init()
{
    int resolution = (int) pow(2, this->mOctaves - 1);
    
    this->mSeeds = new float** [this->mOctaves];
    for (int i = 0; i < this->mOctaves; i++)
    {
        int sz = this->mWidth * resolution + 1;
        this->mSeeds[i] = new float * [sz];
        for (int j = 0; j < sz; j++)
            this->mSeeds[i][j] = new float[sz];
    }
    
    for (int octave = 0; octave < this->mOctaves; octave++)
    for (int x = 0; x < this->mWidth * resolution + 1; x++)
    for (int y = 0; y < this->mWidth * resolution + 1; y++)
    {
        this->mSeeds[octave][x][y] = rng_uniform(-0.5, 0.5);
    }
}

float Perlin2D::get(float x, float y)
{
    float value = 0;
    
    for (int octave = 0; octave < this->mOctaves; octave++)
    {
        int resolution = (int) pow(2.0, octave);
        // println(octave + ", " + int(x) + ", " + int(y));
        float x0y0 = this->mSeeds[octave][int(x * resolution)][int(y * resolution)];
        float x1y0 = this->mSeeds[octave][int(x * resolution) + 1][int(y * resolution)];
        float x0y1 = this->mSeeds[octave][int(x * resolution)][int(y * resolution) + 1];
        float x1y1 = this->mSeeds[octave][int(x * resolution) + 1][int(y * resolution) + 1];
        float fx = (x * resolution) - int(x * resolution);
        float fy = (y * resolution) - int(y * resolution);
        
        float lerpy0 = interpolate(x0y0, x1y0, fx);
        float lerpy1 = interpolate(x0y1, x1y1, fx);
        float lerpx = interpolate(lerpy0, lerpy1, fy);
        float v = lerpx * pow(this->mRolloff, octave);
        
        // cool glitchy gfx
        // float v = (((1 - fx) * x0y0) + (fx * x1y0)) * (1 - fy) +
        //          (((1 - fx) * x0y1) * (fx * x1y1)) * fy;
        value += v;
    }
    return value;
}

float Perlin2D::interpolate(float y1, float y2, float x)
{
    // return cos_lerp(y1, y2, x);
    return lerp(y1, y2, x);
}

float lerp(float y1, float y2, float x)
{
    return y1 + (y2 - y1) * x;
}

float cos_lerp(float y1, float y2, float x)
{
    float mu = (1.0 - cos(x * M_PI)) * 0.5;
    return (y1 * (1 - mu)) + (y2 * (mu));
}


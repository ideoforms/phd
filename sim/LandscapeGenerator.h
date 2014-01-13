//
//  Perlin2D.h
//  LandscapeGenerator
//
//  Created by Daniel Jones on 12/01/2014.
//
//

#pragma once

#include <iostream>

class Perlin2D
{
    
public:
    Perlin2D(int width);
    
    void init();
    float get(float x, float y);
    float interpolate(float y1, float y2, float x);
    
    int mWidth;
    int mOctaves = 4;
    float mRolloff = 0.5;
    
    float ***mSeeds;
    
};


class LandscapeGenerator
{
    
public:
    
    LandscapeGenerator(float detail, float gradient, float abundance);
    void generate();
    float get(float x, float y);
    
    int mWidth;
    float mDetail;
    float mGradient;
    float mAbundance;
    
    Perlin2D *mPerlin;
};


float lerp(float y1, float y2, float x);
float cos_lerp(float y1, float y2, float x);

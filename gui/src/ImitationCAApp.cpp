#include "ImitationCAApp.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

using namespace sim;


settings_t sim::settings;

void ImitationCAApp::setup()
{
	mRate = 1;
    
    setFrameRate(30);
    
    rng_init();
    
    fprintf(stderr, "logging from %s\n", settings.conf_file_arg);
    
    config_parser_init(&settings);

    if (config_parser_configfile(getResourcePath("settings.conf").string().c_str(), &settings, 0, 0, 1) != 0)
        exit(1);
    
    printf("bits %d, patch size %d\n", settings.bits_arg, settings.spatial_patch_size_arg);

    #ifdef MODEL_ABM
        mEnvironment = new EnvironmentABM();
        mOSC = new OSCClient("localhost", 7000);
    #else
        mEnvironment = new EnvironmentCA2D();
    #endif
}

void ImitationCAApp::update()
{
    // mEnvironment->update();
}

void ImitationCAApp::draw()
{
    for (int x = 0; x < mRate; x++)
        mEnvironment->update();
    
	gl::clear();
    
    ci::Area area = getWindowBounds();
    float width = area.getWidth();
    float height = area.getHeight();
    
    #ifdef MODEL_ABM
    
    
        for (int i = 0; i < 7; i++)
        {
            Agent *agent = mEnvironment->mAgents[i];
            mOSC->send("/sim/position", "ifff", i,
                       mEnvironment->mPositions[i].x / settings.abm_width_arg,
                       mEnvironment->mPositions[i].y / settings.abm_width_arg,
                       mEnvironment->mPositions[i].x / settings.abm_width_arg);
        }
    
        gl::color(Color(0.8, 0.8, 0.8));
        gl::drawSolidRect(Rectf(0, 0, width, height));
        int popsize = mEnvironment->get_popsize();
        // printf("drawing %d agents\n", popsize);
        // printf("0: %.f, %.f\n", mEnvironment->mPositions[0].x, mEnvironment->mPositions[0].y);
        // printf("1: %.f, %.f\n", mEnvironment->mPositions[1].x, mEnvironment->mPositions[1].y);
        // printf("2: %.f, %.f\n", mEnvironment->mPositions[2].x, mEnvironment->mPositions[2].y);
        for (int i = 0; i < popsize; i++)
        {
            Agent *agent = mEnvironment->mAgents[i];
            Point2Df location = mEnvironment->mPositions[i];
            Color color(agent->mBEvo, agent->mBInd, agent->mBSoc);
            // Color color((float) x / mEnvironment->mWidth, (float) x / mEnvironment->mWidth, (float) y / mEnvironment->mHeight);
            gl::color(color);
            gl::drawSolidCircle(Vec2f(location.x, location.y), 5);
        }

    #else
    
        float cellWidth = width / mEnvironment->mWidth;
        float cellHeight = height / mEnvironment->mHeight;
            
        for (int x = 0; x < mEnvironment->mWidth; x++)
        for (int y = 0; y < mEnvironment->mHeight; y++)
        {
            Agent *agent = mEnvironment->mGrid[x][y];
            Color color(agent->mBEvo, agent->mBInd, agent->mBSoc);
            // Color color((float) x / mEnvironment->mWidth, (float) x / mEnvironment->mWidth, (float) y / mEnvironment->mHeight);
            gl::color(color);
            gl::drawSolidRect(Rectf(x * cellWidth, y * cellHeight, (x + 1) * cellWidth, (y + 1) * cellHeight));
        }
    
    #endif
}

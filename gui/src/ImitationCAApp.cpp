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
    
    if (strcmp(settings.topology_arg, "numeric") == 0)
        mEnvironment = new sim::Environment();
    else if (strcmp(settings.topology_arg, "ca-1d") == 0)
        mEnvironment = new sim::EnvironmentCA1D();
    else if (strcmp(settings.topology_arg, "ca-2d") == 0)
        mEnvironment = new sim::EnvironmentCA2D();
    else if (strcmp(settings.topology_arg, "ca-2d-multi") == 0)
        mEnvironment = new sim::EnvironmentCA2DMulti();
//    else if (strcmp(settings.topology_arg, "abm") == 0)
//        mEnvironment = new sim::EnvironmentABM();
    else
    {
        printf("invalid topology type: %s\n", settings.topology_arg);
        exit(1);
    }
}

void ImitationCAApp::update()
{
    // mEnvironment->update();
}

void ImitationCAApp::draw()
{
    for (int x = 0; x < mRate; x++)
        mEnvironment->update();
    
	gl::clear(Color(0.7, 0.7, 0.7));
    
    ci::Area area = getWindowBounds();
    float width = area.getWidth();
    float height = area.getHeight();
    
    if (strcmp(settings.topology_arg, "abm") == 0)
    {
        EnvironmentABM *env = (EnvironmentABM *) mEnvironment;
        
        gl::color(Color(0.8, 0.8, 0.8));
        gl::drawSolidRect(Rectf(0, 0, width, height));
        int popsize = mEnvironment->get_popsize();

        for (int i = 0; i < popsize; i++)
        {
            Agent *agent = mEnvironment->mAgents[i];
            Point2Df location = env->mPositions[i];
            Color color(agent->mBEvo, agent->mBInd, agent->mBSoc);
            // Color color((float) x / mEnvironment->mWidth, (float) x / mEnvironment->mWidth, (float) y / mEnvironment->mHeight);
            gl::color(color);
            gl::drawSolidCircle(Vec2f(location.x, location.y), 5);
        }
    }
    else if (strcmp(settings.topology_arg, "ca-2d-multi") == 0)
    {
        EnvironmentCA2DMulti *env = (EnvironmentCA2DMulti *) mEnvironment;
        
        float cellWidth = width / env->mWidth;
        float cellHeight = height / env->mHeight;
    
        for (int x = 0; x < env->mWidth; x++)
        for (int y = 0; y < env->mHeight; y++)
        {
            float payoff = mEnvironment->mLandscape->payoffAt(x, y);
            gl::color(Color(payoff, payoff, payoff));
            gl::drawSolidRect(Rectf(x * cellWidth, y * cellHeight, (x + 1) * cellWidth, (y + 1) * cellHeight));
        }

        for (int x = 0; x < env->mWidth; x++)
        for (int y = 0; y < env->mHeight; y++)
        {
            vector <Agent *> cell = env->mGrid[x][y];
            int cellAgentsAcross = (int) ceil(sqrt(cell.size()));
            // if (cellAgentsAcross < 2) cellAgentsAcross = 2;
            float cellAgentsRadius = cellWidth / (cellAgentsAcross * 2.0);

            for (int i = 0; i < cell.size(); i++)
            {
                Agent *agent = cell[i];
                Color color(agent->mBEvo, agent->mBInd, agent->mBSoc);
                // Color color((float) x / mEnvironment->mWidth, (float) x / mEnvironment->mWidth, (float) y / mEnvironment->mHeight);
                gl::color(color);
                float ax = (cellAgentsRadius * 2) * ((i % cellAgentsAcross) + 0.5);
                float ay = (cellAgentsRadius * 2) * ((int) (i / cellAgentsAcross) + 0.5);
                // gl::drawSolidRect(Rectf(x * cellWidth, y * cellHeight, (x + 1) * cellWidth, (y + 1) * cellHeight));
                gl::drawSolidCircle(Vec2f(x * cellWidth + ax, y * cellHeight + ay), cellAgentsRadius * 1.0);
            }
        }
    }
    else if (strcmp(settings.topology_arg, "ca-2d") == 0)
    {
        EnvironmentCA2D *env = (EnvironmentCA2D *) mEnvironment;
        
        float cellWidth = width / env->mWidth;
        float cellHeight = height / env->mHeight;
        
        for (int x = 0; x < env->mWidth; x++)
            for (int y = 0; y < env->mHeight; y++)
            {
                float payoff = mEnvironment->mLandscape->payoffAt(x, y);
                gl::color(Color(payoff, payoff, payoff));
                gl::drawSolidRect(Rectf(x * cellWidth, y * cellHeight, (x + 1) * cellWidth, (y + 1) * cellHeight));
            }

            
        for (int x = 0; x < env->mWidth; x++)
        for (int y = 0; y < env->mHeight; y++)
        {
            Agent *agent = env->mGrid[x][y];
            Color color(agent->mBEvo, agent->mBInd, agent->mBSoc);
            // Color color((float) x / mEnvironment->mWidth, (float) x / mEnvironment->mWidth, (float) y / mEnvironment->mHeight);
            gl::color(color);
            // gl::drawSolidRect(Rectf(x * cellWidth, y * cellHeight, (x + 1) * cellWidth, (y + 1) * cellHeight));
            gl::drawSolidCircle(Vec2f((x + 0.5) * cellWidth, (y + 0.5) * cellHeight), 0.5 * cellWidth);
        }
    }
    else if (strcmp(settings.topology_arg, "ca-1d") == 0 || strcmp(settings.topology_arg, "numeric") == 0)
    {
        // EnvironmentCA1D *env = (EnvironmentCA1D *) mEnvironment;
        Environment *env = mEnvironment;
        
        int popsize = settings.popsize_arg;
        int displayWidth = (int) sqrt(popsize);
        
        float cellWidth = width / displayWidth;
        float cellHeight = height / displayWidth;
        
//        for (int index = 0; index < popsize; index++)
//        {
//            float payoff = mEnvironment->mLandscape->payoffAt(index, 0);
//            int x = index % displayWidth;
//            int y =  (int) index / displayWidth;
//            gl::color(Color(payoff, payoff, payoff));
//            gl::drawSolidRect(Rectf(x * cellWidth, y * cellHeight, (x + 1) * cellWidth, (y + 1) * cellHeight));
//        }
        
        for (int index = 0; index < popsize; index++)
        {
            int x = index % displayWidth;
            int y =  (int) index / displayWidth;
            Agent *agent = env->mAgents[index];
            float fitness;
            if (settings.metabolism_flag)
            {
                fitness = agent->mOmega / 4;
            } else {
                fitness = agent->get_fitness();
            }
            Color color(agent->mBEvo, agent->mBInd, agent->mBSoc);
            gl::color(color);
            gl::drawSolidCircle(Vec2f((x + 0.5) * cellWidth, (y + 0.5) * cellHeight), 0.5 * cellWidth * (0.5 + fitness * 0.5));
        }
    }
}

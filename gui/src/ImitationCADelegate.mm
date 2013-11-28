#include "cinder/Cinder.h"
#include "cinder/Rand.h"
#import "ImitationCADelegate.h"

@implementation ImitationCADelegate

@synthesize window;

- (void)dealloc
{
    [super dealloc];
}
	
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	mApp = new ImitationCAApp;
	mApp->prepareLaunch();
	mApp->setupCinderView(cinderView, cinder::app::RendererGl::create());
	mApp->launch();
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:0.05 target:self selector:@selector(updateTime) userInfo:nil repeats:YES];
    
}

- (void)updateTime
{
    int time = mApp->getAge();
    // NSLog(@"updateTime %d", time);
    textStepCount.stringValue = [NSString stringWithFormat:@"%d", time];
    // [textStepCount setNeedsDisplay];
}

- (IBAction)rateSliderChanged:(id)sender
{
    
    mApp->mRate = [sender intValue];
    if ([sender intValue] > 0)
        mApp->mRate = pow(2, (mApp->mRate - 1));
    textRate.stringValue = [NSString stringWithFormat:@"%d", mApp->mRate];
}


@end

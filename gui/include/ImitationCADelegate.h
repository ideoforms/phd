
#include "cinder/app/CinderView.h"
#include "ImitationCAApp.h"
#import <Cocoa/Cocoa.h>

@interface ImitationCADelegate : NSObject <NSApplicationDelegate>
{
	IBOutlet CinderView		*cinderView;
	IBOutlet NSWindow		*window;
	IBOutlet NSTextField	*textRate;
    
    IBOutlet NSTextField    *textStepCount;
	
	ImitationCAApp          *mApp;
    
    NSTimer                 *mTimer;
}

@property (assign) IBOutlet NSWindow *window;

- (IBAction)rateSliderChanged:(id)sender;

@end

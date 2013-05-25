#import <Foundation/Foundation.h>

@interface MyEvent : NSObject
- (void) receiveMyEvent: (NSNotification *)notification;
+ (void) postTimedMyEvent: (NSTimeInterval)ti;
+ (void) timerFireMethod: (NSTimer *)theTimer;
@end


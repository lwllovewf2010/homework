//
//  main.m
//  HelloObjectiveC
//
//  Created by Merck Hung on 7/31/12.
//  Copyright (c) 2012 Merck Hung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Greeter.h"
#import "TestObj.h"
#import "MyEvent.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool {
 
        NSString *testString;
        Greeter* myGreeter = [[Greeter alloc] init];
        TestObj *testObj = [[TestObj alloc] init];
        MyEvent *myEvent = [[MyEvent alloc] init];
        
        testString = [NSString alloc];
        testString = [testString init];
        testString = @"Here's a test string in testString!";
        
        NSLog( @"testString: %@", testString );  
        
        [myGreeter setGreetingText: @"Hello Objective-C!!"
                              test: @"My New Test Text!!" ];
        
        [myGreeter issueGreeting];
        
        NSLog( @"Enter main loop\n" );
        
        NSNotification *notification = [NSNotification notificationWithName:@"MyEventClass" object: nil];
        
        [[NSNotificationQueue defaultQueue] enqueueNotification:notification
                                                   postingStyle:NSPostWhenIdle
                                                   coalesceMask:NSNotificationNoCoalescing
                                                       forModes:nil];
        for( ; ; );
    }

    return 0;
}


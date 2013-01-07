//
//  main.m
//  HelloObjectiveC
//
//  Created by Merck Hung on 7/31/12.
//  Copyright (c) 2012 Merck Hung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Greeter.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool {
 
        NSString *testString;
        Greeter* myGreeter = [[Greeter alloc] init];
        
        testString = [NSString alloc];
        testString = [testString init];
        testString = @"Here's a test string in testString!";
        
        NSLog( @"testString: %@", testString );  
        
        [myGreeter setGreetingText: @"Hello Objective-C!!" ];
        [myGreeter issueGreeting];
    }
    return 0;
}


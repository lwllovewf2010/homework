//
//  Greeter.m
//  HelloObjectiveC
//
//  Created by Merck Hung on 7/31/12.
//  Copyright (c) 2012 Merck Hung. All rights reserved.
//

#import "Greeter.h"

@implementation Greeter

- (NSString*) greetingText
{
    return greetingText;
}

- (void) setGreetingText: (NSString*) newText
{
    greetingText = newText;
}

- (void) issueGreeting
{
    NSLog( @"%@", [self greetingText] );
}

@end

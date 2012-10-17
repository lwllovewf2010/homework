//
//  Greeter.h
//  HelloObjectiveC
//
//  Created by Merck Hung on 7/31/12.
//  Copyright (c) 2012 Merck Hung. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Greeter : NSObject
{
    NSString *greetingText;
}

- (NSString*) greetingText;
- (void) setGreetingText: (NSString*) newText;
- (void) issueGreeting;

@end

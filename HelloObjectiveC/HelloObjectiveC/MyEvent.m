//
//  MyEvent.m
//  HelloObjectiveC
//
//  Created by Merck Hung on 1/28/13.
//  Copyright (c) 2013 Merck Hung. All rights reserved.
//

#import "MyEvent.h"

@implementation MyEvent

- (id) init
{
    self = [super init];
    if( self ) {
    
        NSLog( @"MyEvent::init\n" );
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(receiveMyEvent:)
                                                     name:@"MyEventClass"
                                                   object:nil];
    }
    
    return self;
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    //[super dealloc];
}

- (BOOL) acceptsFirstResponder
{
    return YES;
}

- (void) receiveMyEvent:(NSNotification *)notification
{
    NSLog( @"receiveMyEvent()\n" );
}

@end

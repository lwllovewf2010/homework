//
//  TestObj.m
//  HelloObjectiveC
//
//  Created by Merck Hung on 12/27/12.
//  Copyright (c) 2012 Merck Hung. All rights reserved.
//

#import "TestObj.h"

@implementation TestObj

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if( self ) {
    
        NSLog( @"TestObj::initWithCoder\n" );
    }
    
    return self;
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        
        NSLog( @"TestObj::initWithFrame\n" );
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    NSLog( @"TestObj::drawRect\n" );
}

@end

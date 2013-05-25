//
//  Test.m
//  HelloObjectiveC
//
//  Created by Merck Hung on 1/25/13.
//  Copyright (c) 2013 Merck Hung. All rights reserved.
//

#import "Test.h"

@implementation Test

- (id)initWithCoder:(NSCoder *)aDecoder
{
#if 0
    self = [super initWithCoder:aDecoder];
    if( self ) {
        
        NSLog( @"TestObj::initWithCoder\n" );
    }
#endif
    
    return self;
}

@end

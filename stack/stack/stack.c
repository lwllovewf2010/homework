//
//  main.c
//  stack
//
//  Created by Merck Hung on 2/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>

void push( int val );
unsigned int pop( void );


#define STACK_SIZE  1024
static unsigned int stacker[ STACK_SIZE ];
static unsigned int stackp = STACK_SIZE;


void push( int val ) {
    
    if( !stackp ) {
    
        printf( "Stack is full for %d\n", val );
        return;
    }

    stacker[ stackp-- ] = val;
}


unsigned int pop( void ) {
    
    if( stackp == STACK_SIZE ) {
        
        printf( "Stack is empty\n" );
        return 0xFFFFFFFF;
    }
    
    return stacker[ ++stackp ];
}


int main( int argc, const char **argv ) {

    int i;
    
    push( 18 );
    push( 19 );
    push( 98 );
    push( 76 );
    push( 1 );
    push( 5 );
    push( 7 );
    push( 9 );
    push( 34 );
    push( 66 );
    push( 54 );
    push( 87 );
    push( 65 );
    push( 30 );
    push( 42 );
    
    
    for( i = 0 ; i < 10 ; i++ ) {
    
        printf( "POP = %d\n", pop() );
    }
    
    
    return 0;
}


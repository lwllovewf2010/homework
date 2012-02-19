//
//  main.c
//  log2
//
//  Created by Merck Hung on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>


unsigned int log2Ver1( unsigned int val );
unsigned int log2Ver2( unsigned int val );
unsigned int log10Ver1( unsigned int val );
unsigned int log10Ver2( unsigned int val );
unsigned int logVar( unsigned int base, unsigned int val );
void myMutxLock( volatile int *locker );
void myMutxUnlock( volatile int *locker );


static volatile int locker = 0;


void myMutxLock( volatile int *locker ) {

    while( *locker ) {};    // Spin
    *locker = 1;
}


void myMutxUnlock( volatile int *locker ) {
    
    *locker = 0;
}


unsigned int log2Ver1( unsigned int val ) {
    
    unsigned int i;
    
    if( !val )
        return 0;
    
    for( i = 0 ; val >= 2 ; ) {
        
        i++;
        val >>= 1;
    }

    return i;
}


unsigned int log2Ver2( unsigned int val ) {

    if( val < 0x00000080 ) {
        
        // < 2^8
        if( val < 0x00000002 ) return 0;
        if( val < 0x00000004 ) return 1;
        if( val < 0x00000008 ) return 2;
        if( val < 0x00000010 ) return 3;
        if( val < 0x00000020 ) return 4;
        if( val < 0x00000040 ) return 5;
        if( val < 0x00000080 ) return 6;
    }
    else if ( val < 0x00008000 ) {
    
        // < 2^16
        if( val < 0x00000100 ) return 7;
        if( val < 0x00000200 ) return 8;
        if( val < 0x00000400 ) return 9;
        if( val < 0x00000800 ) return 10;
        if( val < 0x00001000 ) return 11;
        if( val < 0x00002000 ) return 12;
        if( val < 0x00004000 ) return 13;
        if( val < 0x00008000 ) return 14;
        
    }
    else if( val < 0x00800000 ) {
        
        // < 2^24
        if( val < 0x00010000 ) return 15;
        if( val < 0x00020000 ) return 16;
        if( val < 0x00040000 ) return 17;
        if( val < 0x00080000 ) return 18;
        if( val < 0x00100000 ) return 19;
        if( val < 0x00200000 ) return 20;
        if( val < 0x00400000 ) return 21;
        if( val < 0x00800000 ) return 22;
    }
    else {
    
        // < 2^32
        if( val < 0x01000000 ) return 23;
        if( val < 0x02000000 ) return 24;
        if( val < 0x04000000 ) return 25;
        if( val < 0x08000000 ) return 26;
        if( val < 0x10000000 ) return 27;
        if( val < 0x20000000 ) return 28;
        if( val < 0x40000000 ) return 29;
        if( val < 0x80000000 ) return 30;
        return 31;
    }
    
    return 0;
}


unsigned int log10Ver1( unsigned int val ) {

    unsigned int i;
    
    if( !val )
        return 0;
    
    for( i = 0 ; val >= 10 ; ) {
    
        i++;
        val /= 10;
    }
    
    return i;
}


unsigned int log10Ver2( unsigned int val ) {
    
    if( val < 100000 ) {
    
        if( val < 10 ) return 0;
        if( val < 100 ) return 1;
        if( val < 1000 ) return 2;
        if( val < 10000 ) return 3;
        if( val < 100000 ) return 4;
    }
    else {
    
        if( val < 1000000 ) return 5;
        if( val < 10000000 ) return 6;
        if( val < 100000000 ) return 7;
        if( val < 1000000000 ) return 8;
        return 9;
    }
    
    return 0;
}


unsigned int logVar( unsigned int base, unsigned int val ) {
    
    unsigned int i;
    
    if( base < 2 || !val )
        return 0;
    
    for( i = 0 ; val >= base ; ) {
        
        i++;
        val /= base;
    }
    
    return i;
}


int main( int argc, const char **argv ) {
    
    unsigned int list[] = { 45, 65543, 7658382, 2, 135, 0, 1000, 0xFFFFFFFF };
    int cell = 8;
    int i, j;
    
    myMutxLock( &locker );
    
    // LOG2 VER1
    printf( "log2Ver1\n\n" );
    for( i = 0 ; i < cell ; i++ )
        printf( "log2( %d ) = %d\n", list[ i ], log2Ver1( list[ i ] ) );
    
    // LOG2 VER2
    printf( "log2Ver2\n\n" );
    for( i = 0 ; i < cell ; i++ )
        printf( "log2( %d ) = %d\n", list[ i ], log2Ver2( list[ i ] ) );

    // LOG10 VER1
    printf( "log10Ver1\n\n" );
    for( i = 0 ; i < cell ; i++ )
        printf( "log10( %d ) = %d\n", list[ i ], log10Ver1( list[ i ] ) );
    
    // LOG10 VER2
    printf( "log10Ver2\n\n" );
    for( i = 0 ; i < cell ; i++ )
        printf( "log10( %d ) = %d\n", list[ i ], log10Ver2( list[ i ] ) );
    
    // logVar
    printf( "logVar\n\n" );
    for( j = 2 ; j < 11 ; j++ ) {
        for( i = 0 ; i < cell ; i++ ) {
        
            printf( "log%d( %d ) = %d\n", j, list[ i ], logVar( j, list[ i ] ) );
        }
    }
    
    myMutxUnlock( &locker );
    
    return 0;
}


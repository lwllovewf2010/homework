//
//  main.c
//  endian
//
//  Created by Merck Hung on 2/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>


int compl2( int val );


int compl2( int val ) {

    unsigned int tmp = (unsigned int)val;
    
    tmp = ~tmp + 1;
    return (int)tmp;
}


int main (int argc, const char * argv[])
{
    unsigned short val = 0x1234;
    unsigned char *p = (unsigned char *)&val;
    unsigned char a = 31, b = 14, c, sum = 0;
    
    if( *p == 0x12 && *(p + 1) == 0x34 ) {
    
        printf( "BIG ENDIAN\n" );
    }
    else if( *p == 0x34 && *(p + 1) == 0x12 ) {
    
        printf( "LITTLE ENDIAN\n" );
    }
    else {
    
        printf( "Internal Error\n" );
    }
    
    for( c = a ^ b ; c != 0 ; c >>= 1 )
        sum += (c & 1);
    
    printf( "Require bits = %d\n", sum );
    
    printf( "compl2(O) = %d\n", 0xF2 );
    printf( "compl2(C) = %d\n", compl2( 0xF2 ) );
    
    return 0;
}


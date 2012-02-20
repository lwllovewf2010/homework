//
//  main.c
//  endian
//
//  Created by Merck Hung on 2/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>

int main (int argc, const char * argv[])
{
    unsigned short val = 0x1234;
    unsigned char *p = (char *)&val;
    
    if( *p = 0x12 && *(p + 1) == 0x34 ) {
    
        printf( "BIG ENDIAN\n" );
    }
    else if( *p = 0x34 && *(p + 1) == 0x12 ) {
    
        printf( "LITTLE ENDIAN\n" );
    }
    else {
    
        printf( "Internal Error\n" );
    }
    
    
    return 0;
}


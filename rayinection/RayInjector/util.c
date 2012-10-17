//
//  util.c
//  RayInjector
//
//  Created by  on 7/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

void dump16Bytes(unsigned char* cipher,const char* prefix)
{
    printf("%s:%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
           prefix,
           cipher[0],cipher[1],cipher[2],cipher[3],
           cipher[4],cipher[5],cipher[6],cipher[7],
           cipher[8],cipher[9],cipher[10],cipher[11],
           cipher[12],cipher[13],cipher[14],cipher[15]);
}

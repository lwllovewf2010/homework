//
//  main.c
//  heap
//
//  Created by Merck Hung on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


typedef struct _heapData {

    struct _heapData    *left;
    struct _heapData    *right;
    struct _heapData    *parent;
    unsigned int        data;

} heapData; 


heapData *headDataHead = NULL;
unsigned int element = 0;


heapData *getNewNode( unsigned int val );


heapData *getNewNode( unsigned int val ) {

    heapData *pHeapData = NULL;
    
    pHeapData = malloc( sizeof( heapData ) );
    if( !pHeapData )
        return NULL;
    
    pHeapData->left = NULL;
    pHeapData->right = NULL;
    pHeapData->parent = NULL;
    pHeapData->data = val;
    
    return pHeapData;
}


void findNewLeaf( void ) {

    
}


int main( int argc, const char **argv ) {
    
    
    
    return 0;
}


//
//  adjlist.c
//  adjlist
//
//  Created by Merck Hung on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adjlist.h"


int initAdjList( adjList_t **ppAdjList ) {

    unsigned long size;
    int i;
    
    // Allocate memory
    size = sizeof( adjList_t ) * MAX_VERTICES;
    *ppAdjList = (adjList_t *)malloc( size );
    if( !*ppAdjList )
        return FALSE;
    
    // Initializing
    memset( (void *)*ppAdjList, 0, size );
    for( i = 0 ; i < MAX_VERTICES ; i++ ) {
        
        ((*ppAdjList) + i)->vertexNo = -1;
        ((*ppAdjList) + i)->visited = FALSE;
    }
    
    return TRUE;
}


void deinitAdjList( adjList_t **ppAdjList ) {
    
    int i;
    adjList_t *pAdjList, *xpAdjList;
    
    if( !ppAdjList || !*ppAdjList )
        return;
    
    // Free resources
    for( i = 0 ; i < MAX_VERTICES ; i++ ) {
            
        if( ((*ppAdjList) + i)->vertexNo != -1 ) {
                
            // FREE EDGES
            pAdjList = ((*ppAdjList) + i)->next;
            for( ; pAdjList ; ) {
                    
                xpAdjList = pAdjList->next;
                //free( pAdjList );
                pAdjList = xpAdjList;
            }
        }
    }
        
    // FREE VERTICES
    free( *ppAdjList );
    *ppAdjList = NULL;
}


int insertVertex( adjList_t *pAdjList, int vertex ) {

    if( vertex >= MAX_VERTICES )
        return FALSE;
    
    if( (pAdjList + vertex)->vertexNo != -1 )
        return FALSE;
    
    (pAdjList + vertex)->vertexNo = vertex;
    return TRUE;
}


int hasVertex( adjList_t *pAdjList, int vertex ) {
    
    if( vertex >= MAX_VERTICES )
        return FALSE;
    
    if( (pAdjList + vertex)->vertexNo == vertex )
        return TRUE;
    
    return FALSE;
}


int appendNode( adjList_t *pAdjList, int vertex ) {
    
    if( !pAdjList )
        return FALSE;
    
    // Find out the least chain
    for( ; pAdjList->next ; pAdjList = pAdjList->next );
    
    // Allocate memory
    pAdjList->next = (adjList_t *)malloc( sizeof( adjList_t ) );
    pAdjList->next->vertexNo = vertex;
    pAdjList->next->next = NULL;
	pAdjList->next->prev = pAdjList;
    
    return TRUE;
}


int hasEdge( adjList_t *pAdjList, int vertex ) {
    
    // Find out the least chain
    for( ; pAdjList ; pAdjList = pAdjList->next )
        if( pAdjList->vertexNo == vertex )
            return TRUE;
    
    return FALSE;
}


int insertEdge( adjList_t *pAdjList, int u, int v ) {
    
    int ret = TRUE; /*, ret1 = TRUE; */
    
    if( hasVertex( pAdjList, u ) != TRUE ||
       hasVertex( pAdjList, v ) != TRUE )
        return FALSE;
    
    // Insert Edges
    if( hasEdge( (pAdjList + u), v ) == FALSE )
        ret = appendNode( (pAdjList + u), v );
    
#if 0
    if( hasEdge( (pAdjList + v), u ) == FALSE )
        ret1 = appendNode( (pAdjList + v), u );
#endif
    
#if 1
	if( ret == TRUE )
#else
    if( ret == TRUE && ret1 == TRUE )
#endif
        return TRUE;
    
    return FALSE;
}


void displayGraph( adjList_t *pAdjList ) {

    int i;
    adjList_t *ppAdjList;
    
    if( !pAdjList )
        return;
        
    for( i = 0 ; i < MAX_VERTICES ; i++ ) {
    
        if( (pAdjList + i)->vertexNo != -1 ) {
         
            // VERTEX
            printf( "(VERTEX %d)\n", (pAdjList + i)->vertexNo );
            
            // EDGE
            ppAdjList = (pAdjList + i)->next;
            for( ; ppAdjList ; ppAdjList = ppAdjList->next ) {

                printf( "\t(EDGE) ( %d, %d )\n",
                       (pAdjList + i)->vertexNo , ppAdjList->vertexNo );
            }
        }
    }
}


unsigned int numOfVertices( adjList_t *pAdjList ) {

    int i, sum = 0;
    
    if( !pAdjList )
        return 0;
    
    for( i = 0 ; i < MAX_VERTICES ; i++ )
        if( (pAdjList + i)->vertexNo != -1 )
            sum++;
    
    return sum;
}


unsigned int numOfEdges( adjList_t *pAdjList ) {

    int i, sum = 0;
    adjList_t *ppAdjList;
    
    if( !pAdjList )
        return 0;
    
    for( i = 0 ; i < MAX_VERTICES ; i++ ) {
        
        if( (pAdjList + i)->vertexNo != -1 ) {
            
            // EDGE
            ppAdjList = (pAdjList + i)->next;
            for( ; ppAdjList ; ppAdjList = ppAdjList->next )
                sum++;
        }
    }
    
    return sum;
}


int packIntoSequentialList( adjList_t *pAdjList ) {

    unsigned int n_vertices, e_edges, slots;
    int *seqList;
    int i, j;
    adjList_t *ppAdjList;
    
    if( !pAdjList )
        return FALSE;
    
    // Calculate numbers of vertices & edges
    n_vertices = numOfVertices( pAdjList );
    e_edges = numOfEdges( pAdjList );
    
    // Size of the sequential list
    slots = n_vertices + e_edges + 1;
    
    // Allocate memory
    seqList = (int *)malloc( sizeof( int ) * slots );
    if( !seqList )
        return FALSE;
    
    // Fill in vertices
    j = 0;
    for( i = 0 ; i < MAX_VERTICES ; i++ )
        if( (pAdjList + i)->vertexNo != -1 )
            seqList[ j++ ] = (pAdjList + i)->vertexNo;
    seqList[ j++ ] = MAX_VERTICES;
    
    // Fill in edges
    for( i = 0 ; i < MAX_VERTICES ; i++ ) {
        
        if( (pAdjList + i)->vertexNo != -1 ) {
            
            ppAdjList = (pAdjList + i)->next;
            for( ; ppAdjList ; ppAdjList = ppAdjList->next ) {
                
                seqList[ j++ ] = ppAdjList->vertexNo;
            }
        }
    }
    
    // Display the sequential list
    for( i = 0 ; i < slots ; i++ )
        printf( "(SEQ) %d => %d\n", i, seqList[ i ] );
    
    // Release resource
    free( seqList );
    
    return TRUE;
}


void reverseOrderDepthFirstSearch( adjList_t *pAdjList, int vertex ) {
    
    adjList_t *ppAdjList;
    int i;
    
    // Sanity check
    if( !pAdjList || vertex >= MAX_VERTICES )
        return;
    
    // Retrieve Vertex
    for( i = 0 ; i < MAX_VERTICES ; i++ ) {
        
        // Find out the location of this vertex
        if( (pAdjList + i)->vertexNo != vertex )
            continue;

        // Check if it's visited, do nothing on it
        if( (pAdjList + i)->visited == TRUE )
            return;
        
        // Indicated as it's visited, first time
        (pAdjList + i)->visited = TRUE;
        
        // Display the just visited vertex on the screen
        printf( "Visited VERTEX %d\n", (pAdjList + i)->vertexNo + 1 );
        
        // Visit its chain
        ppAdjList = (pAdjList + i)->next;
		if( !ppAdjList )
			return;

		// Walk to the least edge
		for( ; ppAdjList->next ; ppAdjList = ppAdjList->next );
        
        // Recursively calling DFS
        for( ; ppAdjList ; ppAdjList = ppAdjList->prev )
            reverseOrderDepthFirstSearch( pAdjList, ppAdjList->vertexNo );
        
        // Finish this vertex
        return;
    }
}


void clearVisited( adjList_t *pAdjList ) {

    int i;
    
    // Sanity check
    if( !pAdjList )
        return;
    
    // Retrieve Vertex
    for( i = 0 ; i < MAX_VERTICES ; i++ )
        if( (pAdjList + i)->visited == TRUE )
            (pAdjList + i)->visited = FALSE;
}


void findDominators( adjList_t *pAdjList, unsigned int max, int vertex, char *idomMap ) {

	int i, j;
    adjList_t *tgtNode;
	char tgtMap[ MAX_VERTICES ], tmpMap[ MAX_VERTICES], findMap[ MAX_VERTICES ];

    // Sanity check
    if( !pAdjList || vertex >= MAX_VERTICES || max >= MAX_VERTICES )
        return;

	// Entry node has itself as the only dominator
	if( !vertex ) {

		idomMap[ 0 ] = 1;
		return;
	}

	// Clear memory
	memset( tgtMap, 0, MAX_VERTICES );

	// Look for arrorws to itself
	for( i = 0 ; i < max ; i++ ) {

		// Whether it has nodes
		if( !(pAdjList + i)->next )
			continue;

		// Find out the arrows to itself
		for( tgtNode = (pAdjList + i)->next ;
			tgtNode ; tgtNode = tgtNode->next ) {

			if( tgtNode->vertexNo == vertex ) {

				//printf( "FOUND (%d,%d)\n", i, vertex );
				tgtMap[ i ] = 1;
				break;
			}
		}
	}

	// Find other vertices' intersected result of dominators
	memset( tmpMap, 1, MAX_VERTICES );
	for( i = 0 ; i < max ; i++ ) {

		if( !tgtMap[ i ] )
			continue;

		// Find other vertices' dominators
		memset( findMap, 0, MAX_VERTICES );
		findDominators( pAdjList, max, i, findMap );

		// Intersect the result
		for( j = 0 ; j < MAX_VERTICES ; j++ ) {

			//printf( "CHK[ %d ] = %d\n", j, findMap[ j ] );
			if( findMap[ j ] == 1 && tmpMap[ j ] == 1 ) {
				tmpMap[ j ] = 1;
			}
			else
				tmpMap[ j ] = 0;
		}
	}

	// Assign itself as one of dominators
	tmpMap[ vertex ] = 1;
	memcpy( idomMap, tmpMap, MAX_VERTICES );
}


unsigned int computeImmedinateDominator( adjList_t *pAdjList, unsigned int max, int vertex ) {

    char domMap[ MAX_VERTICES ];
	int i;
	unsigned int immDom = 0;

    // Compute Immediate Dominators
    findDominators( pAdjList, max, vertex, domMap );
    for( i = 0 ; i < max ; i++ ) {

        if( domMap[ i ] ) {

			if( i >= vertex )
				break;

			immDom = i;
		}
    }

	return immDom;
}



//
//  adjlist.h
//  adjlist
//
//  Created by Merck Hung on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef adjlist_adjlist_h
#define adjlist_adjlist_h


// Definitions
#define MAX_VERTICES        20
#define TRUE                1
#define FALSE               0


// Structures
typedef struct _adjList {
    
    int                 vertexNo;
    struct _adjList     *next;
    
} adjList_t;


// Prototypes
int initAdjList( adjList_t **ppAdjList );
void deinitAdjList( adjList_t **ppAdjList );
int insertVertex( adjList_t *pAdjList, int vertex );
int hasVertex( adjList_t *pAdjList, int vertex );
int appendNode( adjList_t *pAdjList, int vertex );
int hasEdge( adjList_t *pAdjList, int vertex );
int insertEdge( adjList_t *pAdjList, int u, int v );
void display( adjList_t *pAdjList );
unsigned int numOfVertices( adjList_t *pAdjList );
unsigned int numOfEdges( adjList_t *pAdjList );
int packIntoSequentialList( adjList_t *pAdjList );


#endif

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
#define MAX_VERTICES        200
#define MAX_DOMINATORS		((MAX_VERTICES / 8) + 8)
#define TRUE                1
#define FALSE               0


// Structures
typedef struct _adjList {
    
    int                 vertexNo;
    struct _adjList     *next;
	struct _adjList		*prev;
    int                 visited;
	char				dominators[ MAX_VERTICES ];
	int					created;
    
} adjList_t;


// Prototypes
int initAdjList( adjList_t **ppAdjList );
void deinitAdjList( adjList_t **ppAdjList );
int insertVertex( adjList_t *pAdjList, int vertex );
int hasVertex( adjList_t *pAdjList, int vertex );
int appendNode( adjList_t *pAdjList, int vertex );
int hasEdge( adjList_t *pAdjList, int vertex );
int insertEdge( adjList_t *pAdjList, int u, int v );
void displayGraph( adjList_t *pAdjList );
unsigned int numOfVertices( adjList_t *pAdjList );
unsigned int numOfEdges( adjList_t *pAdjList );
int packIntoSequentialList( adjList_t *pAdjList );
void reverseOrderDepthFirstSearch( adjList_t *pAdjList, int vertex );
void clearVisited( adjList_t *pAdjList );
void findDominators( adjList_t *pAdjList, unsigned int max, int vertex, char *idomMap );
unsigned int computeImmedinateDominator( adjList_t *pAdjList, unsigned int max, int vertex );


#endif

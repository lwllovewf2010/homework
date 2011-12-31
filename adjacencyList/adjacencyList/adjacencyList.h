//
//  adjacencyList.h
//  adjacencyList
//
//  Created by Merck Hung on 12/30/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef adjacencyList_adjacencyList_h
#define adjacencyList_adjacencyList_h


typedef struct _adjList {

    int                 vertexNo;
    struct _adjList     *next;

} adjList_t;


class adjacencyList {
    
public:
    adjacencyList( int vertices );
    ~adjacencyList();
    
    bool insertVertex( int v );
    bool deleteVertex( int v );
    bool hasVertex( int v );
    
    bool insertEdge( int v, int u );
    bool deleteEdge( int v, int u );
    bool hasEdge( int v, int u );
    
    void display();
    
private:
    
    adjList_t *pVertices;
    int numVertex;
};

#endif

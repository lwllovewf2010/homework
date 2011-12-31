//
//  adjacencyList.cpp
//  adjacencyList
//
//  Created by Merck Hung on 12/30/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "adjacencyList.h"
using namespace std;


adjacencyList::adjacencyList( int num ) {

    // Allocate memory
    this->pVertices = (adjList_t *)malloc( sizeof( adjList_t ) * num );
    if( !this->pVertices )
        return;
    
    memset( this->pVertices, 0, sizeof( adjList_t ) * num );
}


adjacencyList::~adjacencyList() {

    // Free memory
    if( this->pVertices )
        free( this->pVertices );
}


bool adjacencyList::insertVertex( int v ) {
    
    if( v >= vertices )
        return false;
    
    if( vertex[ v ] )
        return false;
    
    vertex[ v ] = 1;
    return true;
}


bool adjacencyList::deleteVertex( int v ) {
    
    if( v >= vertices )
        return false;
    
    if( !vertex[ v ] )
        return false;
    
    vertex[ v ] = 0;
    return true;
}


bool adjacencyList::hasVertex( int v ) {
    
    if( v >= vertices )
        return false;
    
    if( vertex[ v ] )
        return true;
    
    return false;
}


bool adjacencyList::insertEdge( int v, int u ) {
    
    int *p = this->list;
    
    if( v >= vertices || u >= vertices )
        return false;
    
    if( hasVertex( v ) == false || hasVertex( u ) == false )
        return false;
    
    if( *(p + (v * vertices) + u) )
        return false;
    
    *(p + (v * vertices) + u) = 1;
    
    return true;
}


bool adjacencyList::deleteEdge( int v, int u ) {
    
    int *p = this->list;
    
    if( v >= vertices || u >= vertices )
        return false;
    
    if( hasVertex( v ) == false || hasVertex( u ) == false )
        return false;
    
    if( !*(p + (v * vertices) + u) )
        return false;
    
    *(p + (v * vertices) + u) = 0;
    
    return true;
}


bool adjacencyList::hasEdge( int v, int u ) {
    
    int *p = this->list;
    
    if( v >= vertices || u >= vertices )
        return false;
    
    if( hasVertex( v ) == false || hasVertex( u ) == false )
        return false;
    
    if( *(p + (v * vertices) + u) )
        return true;
    
    return false;
}


void adjacencyList::display() {
    
    int i, j;
    int *p = this->list;
    
    cout << "======================================================\n";
    for( i = 0 ; i < vertices ; i++ ) {
        
        cout << vertex[ i ] << " ";
    }
    cout << endl;
    
    cout << "======================================================\n";
    for( i = 0 ; i < vertices ; i++ ) {
        
        if( i )
            cout << endl;
        
        for( j = 0 ; j < vertices ; j++ ) {
            
            cout << *(p + (i * vertices) + j) << " ";
        }
    }
    cout << "\n======================================================\n";
}


int main( int argc, const char * argv[] ) {

    adjacencyList *am = new adjacencyList( 20 );
    
    am->insertVertex( 1 );
    am->insertVertex( 3 );
    am->insertVertex( 7 );
    am->insertVertex( 8 );
    am->insertVertex( 19 );
    am->insertVertex( 4 );
    am->insertVertex( 6 );
    am->insertVertex( 8 );
    am->insertVertex( 12 );
    am->insertVertex( 13 );
    am->insertVertex( 16 );
    
    am->insertEdge( 1, 3 );
    am->insertEdge( 1, 8 );
    am->insertEdge( 1, 19 );
    am->insertEdge( 1, 4 );
    am->insertEdge( 6, 3 );
    am->insertEdge( 7, 8 );
    am->insertEdge( 8, 3 );
    am->insertEdge( 19, 6 );
    am->insertEdge( 4, 19 );
    am->insertEdge( 7, 1 );
    am->insertEdge( 3, 6 );
    am->insertEdge( 7, 3 );
    am->insertEdge( 12, 13 );
    am->insertEdge( 13, 16 );
    am->insertEdge( 16, 12 );
    am->insertEdge( 8, 16 );
    am->insertEdge( 7, 13 );
    
    // Display the List
    am->display();
    
    delete am;
    
    return 0;
}


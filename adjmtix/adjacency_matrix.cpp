#include <iostream>
#include "adjacency_matrix.h"


using namespace std;


adjacencyMatrix::adjacencyMatrix( int vertices ) {

	matrix = new int[ vertices * vertices ];
	vertex = new int[ vertices ];
	int i;

	// Initializing
	this->vertices = vertices;
	for( i = 0 ; i < (vertices * vertices) ; i++ )
		matrix[ i ] = 0;
	for( i = 0 ; i < vertices ; i++ )
		vertex[ i ] = 0;
}


adjacencyMatrix::~adjacencyMatrix() {

	delete [] matrix;
	delete [] vertex;
}


bool adjacencyMatrix::insertVertex( int v ) {

	if( v >= vertices )
		return false;

	if( vertex[ v ] )
		return false;

	vertex[ v ] = 1;
	return true;
}


bool adjacencyMatrix::deleteVertex( int v ) {

	if( v >= vertices )
		return false;

	if( !vertex[ v ] )
		return false;

	vertex[ v ] = 0;
	return true;
}


bool adjacencyMatrix::hasVertex( int v ) {

	if( v >= vertices )
		return false;

	if( vertex[ v ] )
		return true;

	return false;
}


bool adjacencyMatrix::insertEdge( int v, int u ) {

	int *p = this->matrix;

	if( v >= vertices || u >= vertices )
		return false;

	if( hasVertex( v ) == false || hasVertex( u ) == false )
		return false;

	if( *(p + (v * vertices) + u) )
		return false;

	*(p + (v * vertices) + u) = 1;

	return true;
}


bool adjacencyMatrix::deleteEdge( int v, int u ) {

	int *p = this->matrix;

	if( v >= vertices || u >= vertices )
		return false;

    if( hasVertex( v ) == false || hasVertex( u ) == false )
        return false;

    if( !*(p + (v * vertices) + u) )
        return false;

    *(p + (v * vertices) + u) = 0;

    return true;
}


bool adjacencyMatrix::hasEdge( int v, int u ) {

	int *p = this->matrix;

	if( v >= vertices || u >= vertices )
		return false;

    if( hasVertex( v ) == false || hasVertex( u ) == false )
        return false;

    if( *(p + (v * vertices) + u) )
        return true;

	return false;
}


void adjacencyMatrix::display() {

	int i, j;
	int *p = this->matrix;

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


int main( void ) {

	adjacencyMatrix *am = new adjacencyMatrix( 20 );

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

	// Display the Matrix
	am->display();

	delete am;
	return 0;
}



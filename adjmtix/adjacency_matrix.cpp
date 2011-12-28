#include <iostream>
#include "adjacency_matrix.h"
using namespace std;


adjacencyMatrix::adjacencyMatrix( int vertices ) {

	matrix = new int[ vertices * vertices ];
	int i;

	// Initializing
	this->vertices = vertices;
	for( i = 0 ; i < (vertices * vertices) ; i++ )
		matrix[ i ] = 0;
}


adjacencyMatrix::~adjacencyMatrix() {

	delete [] matrix;
}


void adjacencyMatrix::display() {

	int i, j;
	int *p = this->matrix;

	cout << "======================================================\n";
	for( i = 0 ; i < vertices ; i++ ) {

		if( i )
			cout << endl;

		for( j = 0 ; j < vertices ; j++ ) {

			cout << *(p + i + j) << " ";
		}
	}
	cout << "\n======================================================\n";
}


int main( void ) {

	adjacencyMatrix *am = new adjacencyMatrix( 20 );

	am->display();

	delete am;

	return 0;
}



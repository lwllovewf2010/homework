//
// Advanced Compiler Design Course (Fall 2012)
//   Homework #3: Compute the Immediate Dominators
//
//   Student ID : P01922001
//   Author     : Merck Hung
//   Email      : merckhung@gmail.com
//   Phone      : +886-975-586938
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "adjlist.h"


#define DEBUG

#ifdef DEBUG
#define pdbg( MSG, ARGS... )    printf( "%s: " MSG, __func__, ##ARGS )
#else
#define pdbg( MSG, ARGS... )
#endif


#define BITMASK(x)		(1ULL << x)
#define	SZ_IBUF			4096


typedef struct _property {

	unsigned int		numOfBasicBlocks;
	adjList_t			*BasicBlocks;

} property_t;


int main( void ) {

	int llen;
	char ibuf[ SZ_IBUF ], *p, *idx;
	unsigned int eol, no, rb, nv;
	property_t head;


	// Initializing
	memset( &head, 0, sizeof( property_t ) );
	memset( ibuf, 0, SZ_IBUF );
	p = ibuf;
    if( initAdjList( &head.BasicBlocks ) != TRUE ) {
        
        fprintf( stderr, "Out of memory" );
        return -1;
    }


	// Retrieving the content from stdin
	rb = read( 0, ibuf, SZ_IBUF );
	pdbg( "CONTENT = \"\n%s\"\n", ibuf );


	// Retrieving the numbers of BasicBlocks (The first line)
	idx = index( p, '\n' ); // Look for the new line character
	*idx = '\0'; // truncate the first line
	head.numOfBasicBlocks = strtoul( p, NULL, 10 );
	p = idx + 1; // Point to the start of the next line
	pdbg( "Number of the Basic Blocks = %d\n", head.numOfBasicBlocks );


	// Sanity check for the content of the first two lines
	if( !head.numOfBasicBlocks ) {

		fprintf( stderr, "The numbers of Basic Blocks(%d) cannot be zero\n",
					head.numOfBasicBlocks );
		return -1;
	}


	// Insert vertices
	for( rb = 0 ; rb < head.numOfBasicBlocks ; rb++ )
        insertVertex( head.BasicBlocks, rb );


	//
	// Parsing the content of the Basic Blocks and Variables
	// And then construct the Flow Graph
	//
	for( rb = 0 ; rb < head.numOfBasicBlocks ; rb++ ) {

		// Process a line each time
		idx = index( p, '\n' );
		*idx = '\0';
		pdbg( "Basic Block %d: Processing( \"%s\" )\n", rb, p );

		// Processing a unit
		llen = strlen( p );
		for( eol = 0, nv = 0 ; !eol ; nv++ ) {

			// Convert the decimal number
			idx = index( p, ' ' );
			if( !idx ) {
				idx = p + strlen( p );
				eol = 1;
			}
			no = strtoul( p, NULL, 10 );
			p = idx + 1;

			// Sanity check
			if( no > head.numOfBasicBlocks ) {

				fprintf( stderr, "Exceed the maximum No. of Basic Blocks(%d < %d)\n", no, head.numOfBasicBlocks );
				return -1;
			}

			// Insert an edge
			if( nv ) {

				pdbg( "Add edge (%d, %d)\n", rb, no );
				insertEdge( head.BasicBlocks, rb, no );
			}
		}

		// Move to next line
		p = idx + 1;
	}


#ifdef DEBUG
	// Display graph
	display( head.BasicBlocks );
#endif


	// Compute Immediate Dominators
	depthFirstSearch( head.BasicBlocks, 0 );


	// Free the resources
	deinitAdjList( &head.BasicBlocks );
	

	return 0;
}



//
// Advanced Compiler Design Course (Fall 2012)
//   Homework #1: Compute the Reaching Definition
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


//#define DEBUG

#ifdef DEBUG
#define pdbg( MSG, ARGS... )    printf( "%s: " MSG, __func__, ##ARGS )
#else
#define pdbg( MSG, ARGS... )
#endif


#define BITMASK(x)		(1ULL << x)


#define	SZ_IBUF			4096
#define BB_LEADING		'B'
#define VA_LEADING		'R'


typedef struct _basicBlock {

	unsigned int		blockNo;
	unsigned long long	genB; // 64bits
	unsigned long long	inB;
	unsigned long long	outB;

	struct _basicBlock	*branchLeft;
	struct _basicBlock	*branchRight;

} basicBlock_t;


typedef struct _property {

	unsigned int		numOfBasicBlocks;
	unsigned int		numOfVariables;

	struct _basicBlock	*bbHead;

} property_t;


#ifdef DEBUG
void dumpBasicBlock( basicBlock_t *p, unsigned int num ) {

	int i;

	printf( "\nDumping Basic Blocks\n" );
	for( i = 0 ; i < num ; i++, p++ ) {

		printf( "==========================\n" );
		printf( "blockNo = %d\n", p->blockNo );
		
		if( p->branchLeft )
			printf( "left  = %d\n", p->branchLeft->blockNo );
		else
			printf( "left  = NULL\n" );

		if( p->branchRight )
			printf( "right = %d\n", p->branchRight->blockNo );
		else
			printf( "right = NULL\n" );
	}
	printf( "==========================\n\n" );
}
#endif


void lookupVariableOwner( property_t *pro, unsigned int bNo, unsigned int vNo, unsigned int *left, unsigned int *right ) {

	unsigned long long i;
	basicBlock_t *xBB = pro->bbHead;
	int arr[ 512 ];

	for( i = 0 ; i < bNo ; i++ ) {

		if( (xBB + i)->genB & BITMASK( vNo ) ) {

			printf( "(B%dR%d)\n", i, vNo );
		}
	}
}


int main( void ) {

	int llen;
	char ibuf[ SZ_IBUF ], *p, *idx;
	property_t head;
	basicBlock_t *pBB;

	int bb, var, bbcnt, eol;
	unsigned int no, rb, i;
	int change;
	unsigned long long tmp;


	// Initializing
	memset( &head, 0, sizeof( property_t ) );
	memset( ibuf, 0, SZ_IBUF );
	p = ibuf;


	// Retrieving the content from stdin
	rb = read( 0, ibuf, SZ_IBUF );
	pdbg( "CONTENT = \"\n%s\"\n", ibuf );


	// Retrieving the numbers of BasicBlocks and Variables (First two lines)
	idx = index( p, '\n' ); // Look for the new line character
	*idx = '\0'; // truncate the first line
	head.numOfBasicBlocks = strtoul( p, NULL, 10 );
	p = idx + 1; // Point to the start of the next line
	pdbg( "Number of the Basic Blocks = %d\n", head.numOfBasicBlocks );

	idx = index( p, '\n' );
	*idx = '\0';
	head.numOfVariables = strtoul( p, NULL, 10 );
	p = idx + 1;
	pdbg( "Number of the Variables = %d\n", head.numOfVariables );


	// Sanity check for the content of the first two lines
	if( !head.numOfBasicBlocks || !head.numOfVariables ) {

		fprintf( stderr, "The numbers of Basic Blocks(%d) & Variables(%d) cannot be zero\n",
					head.numOfBasicBlocks,
					head.numOfVariables );
		return -1;
	}


	// Pre-allocate memory for basic blocks
	head.bbHead = (basicBlock_t *)malloc( sizeof( basicBlock_t ) * head.numOfBasicBlocks );
	if( !head.bbHead ) {

		fprintf( stderr, "Out of memory for allocating basic blocks\n" );
		return -1;
	}
	memset( head.bbHead, 0, sizeof( basicBlock_t ) * head.numOfBasicBlocks );
	pBB = head.bbHead;

	// Fill out the block No.
	for( rb = 0 ; rb < head.numOfBasicBlocks ; rb++ )
		(pBB + rb)->blockNo = rb;


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
		eol = bb = var = bbcnt = 0;
		for( ; ; ) {

			// Ignore whitespace
			if( *p == ' ' )
				continue;

			if( *p == BB_LEADING ) {

				// Downwards Basic Block
				bb = 1;
				var = 0;
				bbcnt++;
			}
			else if( *p == VA_LEADING ) {

				// It's a definition of a variable
				bb = 0;
				var = 1;
				bbcnt = 0;
			}
			else {

				// Invalid format
				fprintf( stderr, "Invalid input format of BB or VAR. Near (%s)\n", p );
				return -1;
			}


			// Convert the NO. of either BB or VAR
			p++;
			idx = index( p, ' ' );
			if( !idx ) {
				idx = p + strlen( p );
				eol = 1;
			}
			no = strtoul( p, NULL, 10 );
			p = idx + 1;


			// Sanity check
			if( bb && (no >= head.numOfBasicBlocks) ) {

				fprintf( stderr, "Exceed the maximum number of basic block(%d)\n", no );
				return -1;
			}
			else if( var && (no >= head.numOfVariables) ) {

				fprintf( stderr, "Exceed the maximum number of variable(%d)\n", no );
				return -1;
			}
			

			// Assign downwards BB
			if( bb ) {
				if( bbcnt > 2 ) {

					fprintf( stderr, "Too many downwards basicblocks (%d)\n", bbcnt );
					return -1;
				}
				else if( bbcnt == 2 )
					(pBB + rb)->branchRight = (pBB + no);
				else
					(pBB + rb)->branchLeft = (pBB + no);

				pdbg( "Assign B%d\n", no );
			}
			else {

				// Assign variable
				(pBB + rb)->genB |= BITMASK( no );
				pdbg( "Assign R%d\n", no );
			}

			if( eol )
				break;
		}

		// Move to next line
		p = idx + 1;
	}


#ifdef DEBUG
	dumpBasicBlock( head.bbHead, head.numOfBasicBlocks );
#endif


	// Iterative Algorithm for Reaching Definitions
	for( i = 0, change = 0 ; i < head.numOfBasicBlocks ; i++ ) {


		// Transfer Function: {gen - (x - kill)}
		tmp = (pBB + i)->inB | (pBB + i)->genB;
		if( tmp != (pBB + i)->outB )
			change = 1;
		(pBB + i)->outB = tmp;
		pdbg( "B%d, Gen = %d\n", (pBB + i)->blockNo, (pBB + i)->genB );
		pdbg( "B%d, in[B] = %d, out[B] = %d\n", (pBB + i)->blockNo, (pBB + i)->inB, (pBB + i)->outB );


		// Print out the result
		if( (pBB + i)->inB ) {

			for( rb = 0 ; rb < head.numOfVariables ; rb++ ) {

				if( (pBB + i)->inB & BITMASK( rb ) ) {

					if( rb )
						printf( ", " );

					printf( "B%dR%d", i, rb );
				}
			}
		}
		else
			printf( " " );
		printf( "\n" );

		if( (pBB + i)->outB ) {

            for( rb = 0 ; rb < head.numOfVariables ; rb++ ) {

                if( (pBB + i)->outB & BITMASK( rb ) ) {

					if( rb )
						printf( ", " );
                    printf( "B%dR%d", i, rb );
				}
            }
		}
		else
			printf( " " );
		printf( "\n" );


		// Downwards Exposed
		if( (pBB + i)->branchLeft ) {

			(pBB + i)->branchLeft->inB |= (pBB + i)->outB;
			pdbg( "B%d, in[B] = %d\n", (pBB + i)->branchLeft->blockNo, (pBB + i)->branchLeft->inB );
		}
		if( (pBB + i)->branchRight ) {

			(pBB + i)->branchRight->inB |= (pBB + i)->outB;
			pdbg( "B%d, in[B] = %d\n", (pBB + i)->branchRight->blockNo, (pBB + i)->branchRight->inB );
		}


		// If no changes, then break and end the program
		if( !change )
			break;
	}

	
	return 0;
}



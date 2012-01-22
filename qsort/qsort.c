#include <stdio.h>
#include <stdlib.h>

#define DATA_MAX        20


void swap( int *data1, int *data2 ) {

	int tmp;

	tmp = *data1;
	*data1 = *data2;
	*data2 = tmp;
}


void displayElements( int data[], int cell ) {

    int i;

    // Display
	for( i = 0 ; i < cell ; i++ )
		printf( "%2.2d ", i );
	printf( "\n" );

    for( i = 0 ; i < cell ; i++ )
        printf( "%2.2d ", data[ i ] );
    printf( "\n" );
}


void QuickSort( int data[], int left, int right ) {

	int pivot, lx, rx;
	int tmp;

	pivot = left;
	lx = left;
	rx = right;

	while( 1 ) {

		if( pivot < rx ) {

			if( data[ pivot ] > data[ rx ] ) {

				swap( &data[ pivot ], &data[ rx ] );
				printf( "SWAP %d %d\n", pivot, rx );
				pivot = rx;
				displayElements( data, DATA_MAX );
			}
			else
				rx--;

			continue;
		}

		if( pivot > lx ) {

			if( data[ pivot ] < data[ lx ] ) {

				swap( &data[ pivot ], &data[ lx ] );
				printf( "SWAP %d %d\n", pivot, lx );
				pivot = lx;
				displayElements( data, DATA_MAX );
			}
			else
				lx++;

			continue;
		}

		break;
	}

	// Left side
	if( left < pivot ) {

		printf( "Call(L) Quick Sort %d %d\n", left, pivot - 1 );
		QuickSort( data, left, pivot - 1 );
	}

	// Right side
	if( right > pivot ) {

		printf( "Call(R) Quick Sort %d %d\n", pivot + 1, right );
		QuickSort( data, pivot + 1, right );
	}
}


int main( int argc, char **argv ) {

	int i;
	int data[ DATA_MAX ];


	// Init
	for( i = 0 ; i < DATA_MAX ; i++ ) {

		srand( i * i );
		data[ i ] = (int)(rand() % 100);
	}


	// Display
	printf( "Inited Data =\n" );
	displayElements( data, DATA_MAX );


	// Perform Quick Sort
	QuickSort( data, 0, DATA_MAX - 1 );


	// Display
	printf( "Sorted Data =\n" );
	displayElements( data, DATA_MAX );

	return 0;
}



#include <stdio.h>

int main( int argc, char **argv ) {

	int a = 10;
	int b = 30;
	int c = 58;

	int i, j;
	int tst[ 50 ][ 50 ];

	for( i = 0 ; i < 50 ; i++ ) {
		for(  j = 0 ; j < 50 ; j++ ) {

			tst[ i ][ j ] = a * b * c * i * j;
			printf( "[%d][%d] = %d\n", i, j, tst[ i ][ j ] );
		}
	}

	return 0;
}


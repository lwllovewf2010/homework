#include <stdio.h>


int main( int argc, char **argv ) {

	unsigned int input = 8;
	unsigned int sum = 0;
	unsigned int i;

	printf( "Enter input:" );
	scanf( "%d", &input );

	for( i = 1 ; i <= input ; i++ )
		sum += i;

	printf( "%d! = %d\n", input, sum );

	return 0;
}



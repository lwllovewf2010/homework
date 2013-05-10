#include <stdio.h>
#include <stdlib.h>


__attribute__((fastcall)) static inline int test1( int *a, int *b, int c ) {

	return *a + *b + c;
}


__attribute__((fastcall)) static inline int test2( int *a, int *b, int c ) {

	return 123 + *a - *b + c;
}


typedef int (*func_test_t)( int *a, int *b, int c );
static func_test_t func_test_ops;


int main( int argc, char **argv ) {

	int en;
	int ret;
	int a = 10, b = 45;

	if( argc != 2 ) {

		fprintf( stderr, "Error\n" );
		return 0;
	}

	en = atoi( argv[ 1 ] );
	printf( "en = %d\n", en );

	func_test_ops = en ?
		test1 : test2;

	printf( "test1 = %p\n", test1 );
	printf( "test2 = %p\n", test2 );

	ret = func_test_ops( &a, &b, 67 );

	printf( "ret = %d\n", ret );

	return 0;
}


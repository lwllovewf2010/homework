#include <stdio.h>
#include <stdlib.h>

# define inline            inline          __attribute__((always_inline))


int test_wrap( int (*enter)(int *a, int *b, int c), int *x, int *y, int z ) {

	return enter( x, y, z );
}


__attribute__((fastcall)) static inline int test_enter( int *a, int *b, int c ) {

    return 123 + *a - *b + c;
}


__attribute__((fastcall)) static inline int test_tk( int *a, int *b, int c ) {

    return test_wrap( test_enter, a, b , c );
}


typedef __attribute__((fastcall)) int (*func_test_t)( int *a, int *b, int c );
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
		test_tk : test_enter;

	printf( "tk    = %p\n", test_tk );
	printf( "enter = %p\n", test_enter );

	ret = func_test_ops( &a, &b, 67 );

	printf( "ret = %d\n", ret );

	return 0;
}


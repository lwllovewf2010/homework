#include <stdio.h>



int CallByValue( int input ) {

    input = 0x1234;
    input *= 0x12;
    input /= 3;

    return input;
}


int *TestRoutine( int *input ) {

    *input = 0x1234;
    *input *= 0x12;
    *input /= 3;

    return input;
}



int main( void ) {

    int i = 98;
    int *j;


    j = TestRoutine( &i );
    *j = 45;


    printf( "Hello World, %d\n", *j );


    return 0;
}



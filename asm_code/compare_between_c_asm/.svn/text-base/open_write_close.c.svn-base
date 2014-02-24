#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( void ) {

    int i;
    char *buffer = "This is an Homework that is practicing Assembly.\n";

    i = open( "abc.txt" ,O_WRONLY );
    write( i , buffer ,sizeof(buffer) );
    close( i );

    return 0;
}

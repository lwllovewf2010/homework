#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( void ) {

    write( 1 ,"CHECK\n" ,7 );
    return 0;
}

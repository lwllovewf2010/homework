#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


char *buf, *tmpbuf;
char inst[ 512 ];


void help( void ) {

	fprintf( stderr, "krnoops [FILENAME]\n" );
}


int checkHexAscii( char c ) {

	if( (c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'F')
		|| (c >= 'a' && c <= 'f' ) )
		return 0;

	return 1;
}


int main( int argc, char **argv ) {

	int fd, fsz;
	int i, j, k;
	char tmp[ 3 ];
	char c;

	// Check input
	if( argc != 2 ) {

		help();
		return 0;
	}

	// Open the file
	fd = open( argv[ 1 ], O_RDWR);
	if( fd < 0 ) {

		fprintf( stderr, "Cannot open file: %s\n", argv[ 1 ] );
		return -1;
	}

	// Get file size
	fsz = lseek( fd, 0, SEEK_END );
	lseek( fd, 0, SEEK_SET );

	// Allocate memory
	buf = malloc( fsz );
	if( !buf ) {

		fprintf( stderr, "Out of memory (buf)\n" );
		return -1;
	}

	// Clear the memory
	memset( buf, 0, fsz );

	// Read in the file
	if( read( fd, buf, fsz ) != fsz ) {

		fprintf( stderr, "Cannot read full length of the file\n" );
		close( fd );
		return -1;
	}

	// Close
	close( fd );

	// Allocate memory
    tmpbuf = malloc( fsz );
    if( !tmpbuf ) {

        fprintf( stderr, "Out of memory (tmpbuf)\n" );
        return -1;
    }

	// Convert hex ascii to binary
	tmp[ 2 ] = '\0';
	for( i = 0, j = 0, k = 0 ; i < fsz ; i++ ) {

		// Read a character
		c = *(buf + i);

		// Ignore non-ASCII one
		if( checkHexAscii( c ) )
			continue;

		// Assign to the tmp
		tmp[ j++ ] = c;
		if( j >= 2 ) {

			j = 0;

			// Convert to binary
			tmpbuf[ k++ ] = (char)(strtol( tmp, NULL, 16 ) & 0xFF);
			//printf( "val = 0x%2.2X\n", tmpbuf[ k - 1 ] );
		}
	}

#if 0
	// Allocate the final buffer
	inst = malloc( k );
	if( !inst ) {

		fprintf( stderr, "Out of memory( inst )\n" );
		free( tmpbuf );
		free( buf );
		return -1;
	}
#endif
	memset( inst, 0, sizeof( inst ) );

	// Copy the free resources
	memcpy( inst, tmpbuf, k );
	
	free( tmpbuf );
	free( buf );

	// Print out the address of the buf
	printf( "Oops Code @ address: %p\n", inst );
	*(int *)0 = 0;

	// Return
	return 0;
}


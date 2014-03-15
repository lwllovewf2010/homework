//
// Android VM & Compiler Homework #2 Part-A
//
// Student ID: P01922001 (CSIE, NTU)
// Author    : Merck Hung
// Email     : merckhung@gmail.com
// Phone     : 0975-586938
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <openssl/sha.h>


#define MAGIC_LEN					8
#define CHKSM_LEN					4
#define SHA1_HASH_LEN			20

#define SHA1_SIGN_OFF			(MAGIC_LEN + CHKSM_LEN)
#define CHKSM_OFF					(MAGIC_LEN)

#define CHKSM_START				(MAGIC_LEN + CHKSM_LEN)
#define SHA1_START				(MAGIC_LEN + CHKSM_LEN + SHA1_HASH_LEN)


static void help( void ) {

    fprintf( stderr, "\nCopyright (c) 2014 Merck Hung all right reserved.\n\n" );
    fprintf( stderr, "MOVE73 converter, Rev1.00\n" );
    fprintf( stderr, "Author: merckhung@gmail.com\n\n" );
    fprintf( stderr, "USAGE: \n" );
    fprintf( stderr, "  move73 <DEX_FILE> <LIST_FILE>\n\n" );
}


const int MOD_ADLER = 65521;
unsigned int adler32( unsigned char *data, int len ) {

    unsigned int a = 1, b = 0;
    int index;
 
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
 
    return (b << 16) | a;
}


int doSHA1( unsigned char *input, unsigned long length, unsigned char *md ) {

    SHA_CTX context;

    if( !SHA1_Init( &context ) )
        return -1;

    if( !SHA1_Update( &context, (unsigned char *)input, length ) )
        return -1;

    if( !SHA1_Final( md, &context ) )
        return -1;

    return 0;
}


int main( int argc, char **argv ) {

	int fd, rb, fd1;
	unsigned int loc;
	char buf[ 24 ];
	char *p = NULL;
	unsigned short inst;
	unsigned int fs = 0, chksum = 0;
	unsigned char c;
	int ret = 0;
	unsigned char hash[ 20 ];

	if( argc != 3 ) {

		help();
		return 0;
	}

	// Open the list file
	fd = open( argv[ 2 ], O_RDWR );
	if( fd < 0 ) {

		fprintf( stderr, "Error to open %s file\n", argv[ 2 ] );
		return -1;
	}

	// Open the dex file
	fd1 = open( argv[ 1 ], O_RDWR );
	if( fd1 < 0 ) {

		ret = -1;
		fprintf( stderr, "Error to open %s file\n", argv[ 1 ] );
		goto ErrExit;
	}

	// Get the size of the dex file
	fs = lseek( fd1, 0, SEEK_END );
	lseek( fd1, 0, SEEK_SET );

	// Allocate a buffer
	p = malloc( fs );
	if( !p ) {

		fprintf( stderr, "Out of memory\n" );
		ret = -1;
		goto ErrExit2;
	}

	// Read the entire into buffer
	rb = read( fd1, p, fs );
	if( rb != fs ) {

		fprintf( stderr, "Error at reading the dex file\n" );
		ret = -1;
		goto ErrExit2;
	}

	// Iterate each location and patch the dex file
  // MOVE (0x01) => MOVE73 (0x73)
	while( 1 ) {

		// Read a line
		rb = read( fd, buf, 7 );
		if( rb < 6 )
			break;
		buf[ 6 ] = '\0';

		// Converter into binary
		loc = strtol( buf, NULL, 16 );
		printf( "Locating 0x%x\n", loc );

		// MOVE ==> MOVE73
		inst = *((unsigned short *)(p + loc));
		printf( "INST(B) : 0x%4.4X\n", inst );
		inst &= 0xFF00;	
		inst |= 0x0073;
		printf( "INST(A) : 0x%4.4X\n", inst );
		*((unsigned short *)(p + loc)) = inst;
	}

	// SHA-1 signature update
  if( doSHA1( (p + SHA1_START), (fs - SHA1_START), hash ) != 0 ) {

		ret = -1;
    fprintf( stderr, "Failed to do HASH on file: %s\n", argv[ 1 ] );
    goto ErrExit2;
  }
	printf( "Patching HASH: 0x%2.2X 0x%2.2X | 0x%2.2X 0x%2.2X\n", hash[ 0 ], hash[ 1 ], hash[ 18 ],hash[ 19 ] );
	memcpy( p + SHA1_SIGN_OFF, hash, SHA1_HASH_LEN );

	// Checksum update
	chksum = adler32( p + CHKSM_START, fs - CHKSM_START );
	printf( "New checksum: 0x%8.8X\n", chksum );
	memcpy( p + CHKSM_OFF, &chksum, 4 );

	// Write result to the file
	lseek( fd1, 0, SEEK_SET );
	rb = write( fd1, p, fs );
	if( rb != fs ) {

		fprintf( stderr, "Write size mismatch\n" );
		ret = -1;
		goto ErrExit2;
	}

ErrExit2:
	free( p );
	close( fd1 );

ErrExit:
	close( fd );

	return ret;
}



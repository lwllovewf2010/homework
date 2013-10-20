#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "mtypes.h"
#include "token.h"


#define MAXLEN_FILENAME		2048


static u8 *tokenStrStart = NULL;
static u8 *tokenStrEnd = NULL;


typedef struct _charToToken {

	u8			c;
	u8			tok;

} charToToken_t;


static charToToken_t charToTokenTbl[] = {

{ ' ',	TOK_SPACE },
{ '\t',	TOK_TAB },
{ '\n', TOK_NEWLINE },
{ ':',	TOK_COLON },
{ '=',	TOK_EQUAL },
{ '?',	TOK_QMARK },
{ '+',	TOK_PLUS },
{ '@',	TOK_AT },
{ '<',	TOK_LESSER },
{ '>',	TOK_GREATER },
};


static bool isAlphabet( const s8 c ) {

    if( c >= 'A' && c <= 'Z' )
        return true;

    if( c >= 'a' && c <= 'z' )
        return true;

	if( c == '.' || '-' )
		return true;

    return false;
}


static bool isAlphabetNumber( const s8 c ) {

    if( isAlphabet( c ) )
        return true;

    if( c >= '0' && c <= '9' )
        return true;

    return false;
}


u8 lexerNextToken( u8 **c, s32 len ) {

	s8 token = TOK_INVALID;
	s8 offset = 1;
	s32 i;

	// Length check
	if( len <= 0 )
		return TOK_EOF;

	// Comment
	if( **c == '#' ) {

		// It's definitely a COMMENT token
		token = TOK_COMMENT;
		offset = len; // Assume that it's reached the EOF
		for( i = 1 ; i < len ; i++ ) {

			if( *(*c + i) == '\r' && *(*c + i + 1) == '\n' ) {

				// Windows NL
				offset = i + 2;
				break;
			}
			else if( *(*c + i) == '\n' ) {

				// Unix NL
				offset = i + 1;
				break;
			}
		}

		goto ResRet;
	}

	// Windows Newline
	if( **c == '\r' && *(*c + 1) == '\n' ) {

		offset = 2;
		token = TOK_NEWLINE;
		goto ResRet;
	}

	// Variable type 1
	if( (**c == '$') && *(*c + 1) == '(' ) {

        token = TOK_VAR;
        tokenStrStart = *c + 2;

        // Assume it's reached the EOF
        tokenStrEnd = *c + (len - 1);
        offset = len;

        // Look for a delimiter
        for( i = 2 ; i < len ; i++ ) {

            // Seeing a ')', treat it as the delimiter
            if( *(*c + i) == ')' ) {

                offset = i + 1;
                tokenStrEnd = *c + (i - 1);
                break;
            }
        }

		// Sanity check
		if( (tokenStrStart > tokenStrEnd)
			|| (*(tokenStrEnd + 1) != ')') ) {

			return TOK_INVALID;
		}

        goto ResRet;
	}

	// Variable type 2
	if( **c == '$' ) {

		token = TOK_VAR;
        tokenStrStart = tokenStrEnd = *c + 1;
		offset = 2;

		goto ResRet;
	}

	// General character lookup
	for( i = 0 ; i < ARRAY_SIZE( charToTokenTbl ) ; i++ ) {

		if( **c == charToTokenTbl[ i ].c ) {

			token = charToTokenTbl[ i ].tok;
			goto ResRet;
		}
	}

	// String
	if( isAlphabetNumber( **c ) )  {

		token = TOK_STR;
		tokenStrStart = *c;

		// Assume it's reached the EOF
		tokenStrEnd = *c + (len - 1);
		offset = len;

		// Look for a delimiter
		for( i = 1 ; i < len ; i++ ) {

			// Seeing a newline, space, dallor, or tab, treat it as a delimiter
            if( (*(*c + i) == '\r' && *(*c + i + 1) == '\n')
				|| (*(*c + i) == '\n' || *(*c + i) == ' ' || *(*c + i) == '\t')
				|| (*(*c + i) == '$') || (*(*c + i) == ':') ) {

                offset = i;
				tokenStrEnd = *c + (i - 1);
                break;
            }
		}

		goto ResRet;
	}

	// Rest
	token = **c;

ResRet:
	*c += offset;
	return token;
}


s32 parse( u8* fp, s32 len ) {

	u8 token = TOK_INVALID;
	u8 *xfp = fp;
	u8 strBuf[ 512 ];

	while( (token = lexerNextToken( &xfp, (len - (xfp - fp)) )) != TOK_EOF ) {

		if( token == TOK_INVALID ) {

			printf( "Error Syntax: TOK_INVALID\n" );
			break;
		}

		if( token >= TOK_MAX ) {

			printf( "Char    : %c\n", token );
		}
		else if( token == TOK_STR ) {

            memcpy( strBuf, tokenStrStart, tokenStrEnd - tokenStrStart + 1 );
            strBuf[ tokenStrEnd - tokenStrStart + 1 ] = '\0';
            printf( "TOK_STR : %s\n", strBuf );
        }
		else if( token == TOK_VAR ) {

			memcpy( strBuf, tokenStrStart, tokenStrEnd - tokenStrStart + 1 );
			strBuf[ tokenStrEnd - tokenStrStart + 1 ] = '\0';
			printf( "TOK_VAR : %s\n", strBuf );
		}
		else
			printf( "TOKEN ID: %d\n", token );
	}

	return 0;
}


s32 main( s32 argc, s8 **argv ) {

	s8 ifname[ MAXLEN_FILENAME ];
	s32 ifd;
	u8 *fp;
	struct stat sb;

	if( argc != 2 ) {

		fprintf( stderr, "Invalid amount of the input arguments\n" );
		return -1;
	}

	// Filename
    strncpy( ifname, argv[ 1 ], sizeof( ifname ) );

	printf( "Handling file: %s\n", ifname );

    // Open file
    ifd = open( ifname, O_RDONLY );
    if( ifd < 0 ) {

        fprintf( stderr, "Cannot open file: %s\n", ifname );
        return -1;
    }

	// Get filesize
	if( fstat( ifd, &sb ) == -1 ) {

		fprintf( stderr, "Cannot get filesize: %s\n", ifname );
		goto ErrExit;
	}

	printf( "%s(%lld bytes)\n", ifname, sb.st_size );

	// Mmap the file
	fp = (u8 *)mmap( NULL, sb.st_size, PROT_READ, MAP_PRIVATE, ifd, 0 );
	if( fp == MAP_FAILED ) {

		fprintf( stderr, "Failed to mmap the file: %s\n", ifname );
		goto ErrExit;
	}

	
	// Parse the file
	parse( fp, sb.st_size );


ErrExit1:
	// Unmap the file
	munmap( fp, sb.st_size );

ErrExit:
	// Close file
    close( ifd );

	return 0;
}



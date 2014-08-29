#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mymake.h>
#include <lexer.h>

using namespace mymake;


typedef struct _tokStr {

  u32 tokId;
  u8 tokStr[ 50 ];

} TokStr_t;


TokStr_t tokStrTbl[] = {
  { Lexer::TOK_INVALID, AS_STR( Lexer::TOK_INVALID ) },
  { Lexer::TOK_NEWLINE, AS_STR( Lexer::TOK_NEWLINE ) },
  { Lexer::TOK_COMMENT, AS_STR( Lexer::TOK_COMMENT ) },
  { Lexer::TOK_SPACE, AS_STR( Lexer::TOK_SPACE ) },
  { Lexer::TOK_STR, AS_STR( Lexer::TOK_STR ) },
  { Lexer::TOK_TAB, AS_STR( Lexer::TOK_TAB ) },
  { Lexer::TOK_COLON, AS_STR( Lexer::TOK_COLON ) },
  { Lexer::TOK_QMARK, AS_STR( Lexer::TOK_QMARK ) },
  { Lexer::TOK_EQUAL, AS_STR( Lexer::TOK_EQUAL ) },
  { Lexer::TOK_DALLOR, AS_STR( Lexer::TOK_DALLOR ) },
  { Lexer::TOK_PLUS, AS_STR( Lexer::TOK_PLUS ) },
  { Lexer::TOK_AT, AS_STR( Lexer::TOK_AT ) },
  { Lexer::TOK_LESSER, AS_STR( Lexer::TOK_LESSER ) },
  { Lexer::TOK_GREATER, AS_STR( Lexer::TOK_GREATER ) },
  { Lexer::TOK_VAR, AS_STR( Lexer::TOK_VAR ) },
  { Lexer::TOK_PRECENT, AS_STR( Lexer::TOK_PRECENT ) },
  { Lexer::TOK_CARET, AS_STR( Lexer::TOK_CARET ) },
  { Lexer::TOK_STAR, AS_STR( Lexer::TOK_STAR ) },
  { Lexer::TOK_BOF, AS_STR( Lexer::BOF ) },
  { Lexer::TOK_EOF, AS_STR( Lexer::TOK_EOF ) },

  { Lexer::TOK_VPATH, AS_STR( Lexer::TOK_VPATH ) },
  { Lexer::TOK_VPATH_CAPS, AS_STR( Lexer::TOK_VPATH_CAPS ) },
  { Lexer::TOK_INCLUDE, AS_STR( Lexer::TOK_INCLUDE ) },
  { Lexer::TOK_IF, AS_STR( Lexer::TOK_IF ) },
  { Lexer::TOK_IFDEF, AS_STR( Lexer::TOK_IFDEF ) },
  { Lexer::TOK_IFNDEF, AS_STR( Lexer::TOK_IFNDEF ) },
  { Lexer::TOK_ENDEF, AS_STR( Lexer::TOK_ENDEF ) },
  { Lexer::TOK_IFEQ, AS_STR( Lexer::TOK_IFEQ ) },
  { Lexer::TOK_IFNEQ, AS_STR( Lexer::TOK_IFNEQ ) },
  { Lexer::TOK_ELSE, AS_STR( Lexer::TOK_ELSE ) },
  { Lexer::TOK_ENDIF, AS_STR( Lexer::TOK_ENDIF ) },
  { Lexer::TOK_LOAD, AS_STR( Lexer::TOK_LOAD ) },
  { Lexer::TOK_DEF_GOAL, AS_STR( Lexer::TOK_DEF_GOAL ) },
  { Lexer::TOK_DEF, AS_STR( Lexer::TOK_DEF ) },
  { Lexer::TOK_DEL_ON_ERR, AS_STR( Lexer::TOK_DEL_ON_ERR ) },
  { Lexer::TOK_EXP_ALL_VAR, AS_STR( Lexer::TOK_EXP_ALL_VAR ) },
  { Lexer::TOK_FEATURES, AS_STR( Lexer::TOK_FEATURES ) },
  { Lexer::TOK_IGNORE, AS_STR( Lexer::TOK_IGNORE ) },
  { Lexer::TOK_INC_DIRS, AS_STR( Lexer::TOK_INC_DIRS ) },
  { Lexer::TOK_INTERMED, AS_STR( Lexer::TOK_INTERMED ) },
  { Lexer::TOK_LIBPATTERNS, AS_STR( Lexer::TOK_LIBPATTERNS ) },
  { Lexer::TOK_LOADED, AS_STR( Lexer::TOK_LOADED ) },
  { Lexer::TOK_LOW_RES_TIME, AS_STR( Lexer::TOK_LOW_RES_TIME ) },
  { Lexer::TOK_NOTPARALLEL, AS_STR( Lexer::TOK_NOTPARALLEL ) },
  { Lexer::TOK_ONESHELL, AS_STR( Lexer::TOK_ONESHELL ) },
  { Lexer::TOK_PHONY, AS_STR( Lexer::TOK_PHONY ) },
  { Lexer::TOK_POSIX, AS_STR( Lexer::TOK_POSIX ) },
  { Lexer::TOK_PRECIOUS, AS_STR( Lexer::TOK_PRECIOUS ) },
  { Lexer::TOK_RECIPE_PREFIX, AS_STR( Lexer::TOK_RECIPE_PREFIX ) },
  { Lexer::TOK_SECONDARY, AS_STR( Lexer::TOK_SECONDARY ) },
  { Lexer::TOK_SECONDEXPANSION, AS_STR( Lexer::TOK_SECONDEXPANSION ) },
  { Lexer::TOK_SHELLFLAGS, AS_STR( Lexer::TOK_SHELLFLAGS ) },
  { Lexer::TOK_SLIENT, AS_STR( Lexer::TOK_SLIENT ) },
  { Lexer::TOK_SUFFIXES, AS_STR( Lexer::TOK_SUFFIXES ) },
  { Lexer::TOK_VARIABLES, AS_STR( Lexer::TOK_VARIABLES ) },
};


TokStr_t lineTypeTbl[] = {
  { Lexer::LP_NORMAL, AS_STR( Lexer::LP_NORMAL ) },
  { Lexer::LP_TARGET, AS_STR( Lexer::LP_TARGET ) },
  { Lexer::LP_RECEIPT, AS_STR( Lexer::LP_RECEIPT ) },
  { Lexer::LP_VARIABLE, AS_STR( Lexer::LP_VARIABLE ) },
  { Lexer::LP_AMB_REC_VAR, AS_STR( Lexer::LP_AMB_REC_VAR ) },
};


u8 *GetTokenNameString( Lexer::Token tok ) {

	s32 i;

	for( i = 0 ; i < ARRAY_SIZE( tokStrTbl ) ; i++ )
    if( tokStrTbl[ i ].tokId == (Lexer::Token)tok )
      return tokStrTbl[ i ].tokStr;
  return NULL;
}


u8 *GetLineTypeString( Lexer::LineType type ) {

  s32 i;

  for( i = 0 ; i < ARRAY_SIZE( lineTypeTbl ) ; i++ )
    if( lineTypeTbl[ i ].tokId == (Lexer::LineType)type )
      return lineTypeTbl[ i ].tokStr;
  return NULL;
}


s32 main( s32 argc, s8 **argv ) {

  Lexer *lexer = new Lexer();
  Lexer::Token tok, latok, chktok;
  Lexer::LineType typ;
  s32 len, ret = 0, rb;
	s32 ans, i;
  s8 ver = 1;
  s8 restart = 0;
  s8 buf[ 2048 ], chk[ 2048 ];
  u32 a, b, c;


  // Check input arguments
  if( argc < 3 ) {

    fprintf( stderr, "Invalid input arguments\n" );
    fprintf( stderr, "tstLexer [MAKEFILE] [TOKEN_LIST] [OFFSET_LIST] <STOP>\n\n" );
    return -1;
  }

  if( argc == 4 )
    ver = 0;

  // Open the Makefile
  if( lexer->Open( argv[ 1 ] ) == false ) {

    fprintf( stderr, "Cannot open Makefile: %s\n", argv[ 1 ] );
    return -1;
  }

  // Open answers
  ans = open( argv[ 2 ], O_RDONLY );
  if( ans < 0 ) {

    fprintf( stderr, "Cannot open Answer files: %s\n", argv[ 2 ] );
    ret = -1;
    goto ErrExit;
  }


  //
  //
  // GetNextToken() test
  //
  //
RestartPoint:
  if( ver && !restart )
    printf( "%s:Test API: GetNextToken() starts\n", argv[ 1 ] );

  while( (tok = lexer->GetNextToken())
         != Lexer::TOK_EOF ) {

    // Lexer error
    if( tok == Lexer::TOK_INVALID ) {

      fprintf( stderr, "Lexer error: invalid character: %s\n", argv[ 1 ] );
      ret = -1;
      goto ErrExit;
    }

    // Get the token
    lexer->ExtractTokenStr();
    snprintf( buf, sizeof( buf ), "%s(%d),len:%d,line:%d,offs:%d,offe:%d\n\"%s\"\n",
            GetTokenNameString( tok ),
            tok,
            lexer->GetTokenStrLen(),
            lexer->GetLineNum(),
            lexer->GetLineOffStart(),
            lexer->GetLineOffEnd(),
            lexer->GetTokenStr() );
		len = strlen( buf );

    // Read answer from the file
    rb = read( ans, chk, len );

    // Check the read bytes
    if( (rb != len) && ver ) {

      fprintf( stderr, "[ERROR]Token check (length) has failed at:\n" );
      fprintf( stderr, "[Check Start]%s[Check End]\n", chk );
      fprintf( stderr, "[Read  Start]%s[Read  End]\n", buf );
      ret = -1;
      goto ErrExit;
    }

    // Compare the strings
    if( (strncmp( buf, chk, len )) && ver ) {

      fprintf( stderr, "[ERROR]Token check (content) has failed at:\n" );
      fprintf( stderr, "[Check Start]%s[Check End]\n", chk );
      fprintf( stderr, "[Read  Start]%s[Read  End]\n", buf );
      ret = -1;
      goto ErrExit;
    }

    // Print the read
    if( !ver )
      printf( "%s", buf );
  }
  if( ver && !restart )
    printf( "%s:Test API: GetNextToken() passed\n", argv[ 1 ] );


  //
  //
  // Restart() test
  //
  //
  if( !restart ) {

    if( ver )
      printf( "%s:Test API: Restart() starts\n", argv[ 1 ] );

    lexer->Restart();
    restart = 1;
    goto RestartPoint;
  }
  if( ver )
    printf( "%s:Test API: Restart() passed\n", argv[ 1 ] );


  //
  //
  // LookAheadToken() test
  //
  //
  if( ver )
    printf( "%s:Test API: LookAheadToken() starts\n", argv[ 1 ] );

  // Restart
  lexer->Restart();

  // Start the test
  latok = Lexer::TOK_BOF;
  while( 1 ) {

    // Get next token
    tok = lexer->GetNextToken();

    // Lexer error
    if( tok == Lexer::TOK_INVALID ) {

      fprintf( stderr, "Lexer error: invalid character: %s\n", argv[ 1 ] );
      ret = -1;
      goto ErrExit;
    }

    // EOF
    if( tok == Lexer::TOK_EOF )
      break;

    // Validate
    if( latok != Lexer::TOK_BOF ) {

      if( tok != latok ) {

        fprintf( stderr, "Lexer error: LookAheadToken() is broken\n" );
        fprintf( stderr, "%s(%d) != %s(%d)\n",
          GetTokenNameString( tok ),
          tok,
          GetTokenNameString( latok ),
          latok );
        ret = -1;
        goto ErrExit;
      }
    }

    // Look ahead the next token
    latok = lexer->LookAheadToken();
  }
  if( ver )
    printf( "%s:Test API: LookAheadToken() passed\n", argv[ 1 ] );


  //
  //
  // ToNextLine() test
  //
  //
  if( ver )
    printf( "%s:Test API: ToNextLine() starts\n", argv[ 1 ] );

  // TEST1: Jump to next line from BOF
  lexer->Restart();
  latok = Lexer::TOK_BOF;
  while( 1 ) {

    // Get the first token from the next line
    tok = latok;
    latok = lexer->GetNextToken();
    if( tok == Lexer::TOK_NEWLINE )
      break;
  }

  // Restart() has been verified
  lexer->Restart();

  // Jump over to next line
  lexer->ToNextLine();

  // Get the first token from the next line
	tok = lexer->GetNextToken();

  // Validate
  if( tok != latok ) {

    fprintf( stderr, "Lexer error: ToNextLine() is broken\n" );
    fprintf( stderr, "%s(%d) != %s(%d)\n",
      GetTokenNameString( tok ),
      tok,
      GetTokenNameString( latok ),
      latok );
    ret = -1;
    goto ErrExit;
  }


	// TEST2: Jump to next line from middle
  lexer->Restart();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();

  latok = Lexer::TOK_BOF;
  while( 1 ) {

    // Get the first token from the next line
    tok = latok;
    latok = lexer->GetNextToken();
    if( tok == Lexer::TOK_NEWLINE )
      break;
  }

  // Restart() has been verified
  lexer->Restart();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();
  lexer->GetNextToken();

  // Jump over to next line from the middle
  lexer->ToNextLine();

  // Get the first token from the next line
  tok = lexer->GetNextToken();

  // Validate
  if( tok != latok ) {

    fprintf( stderr, "Lexer error: ToNextLine() is broken\n" );
    fprintf( stderr, "%s(%d) != %s(%d)\n",
      GetTokenNameString( tok ),
      tok,
      GetTokenNameString( latok ),
      latok );
    ret = -1;
    goto ErrExit;
  }

  // TEST3: Jump to next line from a NL token
  lexer->Restart();
  latok = Lexer::TOK_BOF;
  while( 1 ) {

    // Get the first token from the next line
    tok = latok;
    latok = lexer->GetNextToken();
    if( tok == Lexer::TOK_NEWLINE )
      break;
  }

  // Restart() has been verified
  lexer->Restart();

  // Move to a NL token
  while( 1 ) {

    lexer->GetNextToken();
    tok = lexer->LookAheadToken();
    if( tok == Lexer::TOK_NEWLINE )
      break;
  }

  // Jump over to next line
  lexer->ToNextLine();

  // Get the first token from the next line
  tok = lexer->GetNextToken();

  // Validate
  if( tok != latok ) {

    fprintf( stderr, "Lexer error: ToNextLine() is broken\n" );
    fprintf( stderr, "%s(%d) != %s(%d)\n",
      GetTokenNameString( tok ),
      tok,
      GetTokenNameString( latok ),
      latok );
    ret = -1;
    goto ErrExit;
  }

  // Validate
  if( tok != latok ) {

    fprintf( stderr, "Lexer error: ToNextLine() is broken\n" );
    fprintf( stderr, "%s(%d) != %s(%d)\n",
      GetTokenNameString( tok ),
      tok,
      GetTokenNameString( latok ),
      latok );
    ret = -1;
    goto ErrExit;
  }


  // TEST4: Continue to jump until the EOF
  lexer->Restart();
  for( i = 0 ; ; i++ ) {

    if( (i + 1) != lexer->GetLineNum() ) {

      fprintf( stderr, "Lexer error: ToNextLine() failed at line %d<=>%d\n",
               i + 1,
               lexer->GetLineNum() );
      ret = -1;
      goto ErrExit;
    }

    if( lexer->ToNextLine() == false )
      break;
  }

  // Check EOF
  if( lexer->IsEOF() == false ) {

    fprintf( stderr, "Lexer error: ToNextLine() failed to reach EOF\n" );
    ret = -1;
    goto ErrExit;
  }
  if( ver )
    printf( "%s:Test API: ToNextLine() passed\n", argv[ 1 ] );


  //
  //
  // ToPrevLine() test
  //
  //
  if( ver )
    printf( "%s:Test API: ToPrevLine() starts\n", argv[ 1 ] );


  // TEST1: Go back from EOL until reaches BOF
  lexer->Restart();

  // Go the EOL first
  for( i = 0 ; lexer->ToNextLine() ; i++ );

  // Check line by line
  for( ; ; --i ) {

    if( (i + 1) != lexer->GetLineNum() ) {

      fprintf( stderr, "Lexer error: ToPrevLine() failed at line %d<=>%d\n",
               i,
               lexer->GetLineNum() );
      ret = -1;
      goto ErrExit;
    }

    if( lexer->ToPrevLine() == false )
      break;
  }
  // Check for BOF
  if( lexer->IsBOF() == false ) {

    fprintf( stderr, "Lexer error: ToPrevLine() failed to reach BOF\n" );
    ret = -1;
    goto ErrExit;
  }


  // TEST2: Go back from the middle until reaches BOF
  lexer->Restart();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->GetNextToken();
  lexer->GetNextToken();
  i = lexer->GetLineNum() - 1;

  // Check line by line
  for( ; ; --i ) {

    if( (i + 1) != lexer->GetLineNum() ) {

      fprintf( stderr, "Lexer error: ToPrevLine() failed at line %d<=>%d\n",
               i,
               lexer->GetLineNum() );
      ret = -1;
      goto ErrExit;
    }

    if( lexer->ToPrevLine() == false )
      break;
  }

  // Check for BOF
  if( lexer->IsBOF() == false ) {

    fprintf( stderr, "Lexer error: ToPrevLine() failed to reach BOF\n" );
    ret = -1;
    goto ErrExit;
  }


  // TEST3: Already at BOF and stress for times
  lexer->Restart();
  lexer->ToPrevLine();
  lexer->ToPrevLine();
  lexer->ToPrevLine();
  lexer->ToPrevLine();
  lexer->ToPrevLine();
  lexer->ToPrevLine();

  // Check for BOF
  if( lexer->IsBOF() == false ) {

    fprintf( stderr, "Lexer error: ToPrevLine() failed to reach BOF\n" );
    ret = -1;
    goto ErrExit;
  }
  if( ver )
    printf( "%s:Test API: ToPrevLine() passed\n", argv[ 1 ] );



  //
  //
  // ToBeginOfLine() test
  //
  //
  if( ver )
    printf( "%s:Test API: ToBeginOfLine() starts\n", argv[ 1 ] );


  // TEST1: Already at BOF, stress for few times
  lexer->Restart();
  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();

  // Check for BOF
  if( lexer->IsBOF() == false ) {

    fprintf( stderr, "Lexer error: ToBeginOfLine() failed to reach BOF\n" );
    ret = -1;
    goto ErrExit;
  }


  // TEST2: Return from the middle of a line
  // Make sure the line number doesn't change
  lexer->Restart();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->GetNextToken();
  lexer->GetNextToken();

  i = lexer->GetLineNum();

  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();

  if( i != lexer->GetLineNum() ) {

    fprintf( stderr, "Lexer error: ToBeginOfLine() failed at line %d<=>%d\n",
             i,
             lexer->GetLineNum() );
    ret = -1;
    goto ErrExit;
  }


  // TEST3: Return from the middle of a line
  // Make sure it's the same token
  lexer->Restart();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  
  // Find 3 consective tokens in the same line
  while( 1 ) {

    a = lexer->GetLineNum();
    chktok = lexer->GetNextToken();

    b = lexer->GetLineNum();
    tok = lexer->GetNextToken();

    c = lexer->GetLineNum();
    latok = lexer->GetNextToken();

    if( (chktok == Lexer::TOK_EOF) 
        || (tok == Lexer::TOK_EOF)
        || (latok == Lexer::TOK_EOF) ) {

      fprintf( stderr, "%s: Cannot find 3 consective tokens for ToBeginLine() test\n", argv[ 1 ] );
      fprintf( stderr, "%s: Please change the content of the test file\n", argv[ 1 ] );
      ret = -1;
      goto ErrExit;
    }

    // All tokens are not NL, as well as all on the same line
    if( (chktok != Lexer::TOK_NEWLINE)
        && (tok != Lexer::TOK_NEWLINE)
        && (latok != Lexer::TOK_NEWLINE)
        && (a == b)
        && (b == c) )
      break;

    // Move to the beginning of next line
    lexer->ToNextLine();
  }

  // Call for several times
  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();
  lexer->ToBeginOfLine();

  // Validate
  tok = lexer->GetNextToken();
  if( chktok != tok ) {

    fprintf( stderr, "Lexer error: ToBeginOfLine() failed at line %d\n",
             lexer->GetLineNum() );
    fprintf( stderr, "Lexer error: Token %s(%d)<=>%s(%d) mismatch\n",
             GetTokenNameString( chktok ), chktok,
             GetTokenNameString( tok ), tok );
    ret = -1;
    goto ErrExit;
  }
  if( ver )
    printf( "%s:Test API: ToBeginOfLIne() passed\n", argv[ 1 ] );


  //
  //
  // IsBOL() test
  //
  //
  if( ver )
    printf( "%s:Test API: IsBOL() starts\n", argv[ 1 ] );

  // TEST1: Test from BOL
  lexer->Restart();
  if( lexer->IsBOL() != true ) {

    fprintf( stderr, "Lexer error: IsBOL() is broken(1)\n" );
    ret = -1;
    goto ErrExit;
  }

  // TEST2: Test from EOF
  while( lexer->ToNextLine() );
  if( lexer->IsBOL() != true ) {

    fprintf( stderr, "Lexer error: IsBOL() is broken(2)\n" );
    ret = -1;
    goto ErrExit;
  }

  // TEST3: Test from some line start
  lexer->Restart();
  lexer->ToNextLine();
  if( lexer->IsBOL() != true ) {

    fprintf( stderr, "Lexer error: IsBOL() is broken(3)\n" );
    ret = -1;
    goto ErrExit;
  }

  // TEST4: Test from the middle of some line
  lexer->Restart();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  lexer->ToNextLine();
  tok = lexer->GetNextToken();
  if( tok == Lexer::TOK_NEWLINE ) {

    fprintf( stderr, "%s: Cannot move to the middle of a line for ToBOL() test\n", argv[ 1 ] );
    fprintf( stderr, "%s: Please change the line(%d) of the test file\n", argv[ 1 ], lexer->GetLineNum() );
    ret = -1;
    goto ErrExit;
  }
  if( lexer->IsBOL() == true ) {

    fprintf( stderr, "Lexer error: IsBOL() is broken(4)\n" );
    ret = -1;
    goto ErrExit;
  }
  if( ver )
    printf( "%s:Test API: IsBOL() passed\n", argv[ 1 ] );



  //
  //
  // PredictLineType() test
  //
  //
  if( ver )
    printf( "%s:Test API: PredictLineType() starts\n", argv[ 1 ] );


  // TEST1: Do it at the beginning of each line
  lexer->Restart();

  // Iterate each line
  do {

    // Get the line predict and the number
    typ = lexer->PredictLineType();
    i = lexer->GetLineNum();

    // Prepare the buffer
    snprintf( buf, sizeof( buf ), "LINE %d is predicted as type: %s(%d)\n",
              i,
              GetLineTypeString( typ ),
              typ );
    len = strlen( buf );

    // Read answer from the file
    rb = read( ans, chk, len );

    // Check the read bytes
    if( (rb != len) && ver ) {

      fprintf( stderr, "[ERROR]Line type check (length) has failed at:\n" );
      fprintf( stderr, "[Check Start]%s[Check End]\n", chk );
      fprintf( stderr, "[Read  Start]%s[Read  End]\n", buf );
      ret = -1;
      goto ErrExit;
    }

    // Compare the strings
    if( (strncmp( buf, chk, len )) && ver ) {

      fprintf( stderr, "[ERROR]Line type check (content) has failed at:\n" );
      fprintf( stderr, "[Check Start]%s[Check End]\n", chk );
      fprintf( stderr, "[Read  Start]%s[Read  End]\n", buf );
      ret = -1;
      goto ErrExit;
    }

    // Print the read
    if( !ver )
      printf( "%s", buf );

  } while( lexer->ToNextLine() );

  if( ver )
    printf( "%s:Test API: PredictLineType() passed\n", argv[ 1 ] );



  // Release
ErrExit:
  if( ans >= 0 )
    close( ans );
  lexer->Close();

  return ret;
}



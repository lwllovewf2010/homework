#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/PassManager.h"

#include <stdio.h>
#include <mymake.h>
#include <lexer.h>
#include <parser.h>
#include <ast.h>

using namespace mymake;


static void ErrorMessageHandle( Lexer *lexer, bool fatal ) {
  
  s32 i;

  //
  // Syntax Error Handling
  //
  if( fatal )
    fprintf( stderr, "\x1B[1mtest2.mak:%u:%u: \x1B[31mfatal error\x1B[0m\n",
          lexer->GetLineNum(),
          lexer->GetLineOffStart() );
  else
    fprintf( stderr, "\x1B[1mtest2.mak:%u:%u: \x1B[35mwarning\x1B[0m\n",
          lexer->GetLineNum(),
          lexer->GetLineOffStart() );
  
  fprintf( stderr, "  %s\n",
          lexer->ExtractLineStr() );
  
  for( i = 0 ; i < (lexer->GetLineOffStart() + 2) ; i++ )
  fprintf( stderr, " " );
  
  fprintf( stderr, "\x1B[1;32m" );
  for( i = 0 ; i < (lexer->GetLineOffEnd() - lexer->GetLineOffStart() + 1) ; i++ )
  fprintf( stderr, "^" );
  
  fprintf( stderr, "\x1B[0;37m\n" );
}


bool Parser::Driver( Lexer *lexer ) {

  Lexer::Token tok;
  Lexer::LineType type;
  s32 ln, i, wb;
  s8 buf[ LEXER_TMPBUF_SIZE ], name[ 128 ];
  bool sep, fstr;
  
  // Predict the type of the first line
  type = lexer->PredictLineType();
  
  // Iterate all tokens
  for( ln = 1 ;
       (tok = lexer->GetNextToken())
        != Lexer::TOK_EOF ; ) {

		// Invalid token handling
    if( tok == Lexer::TOK_INVALID ) {
      
      // Fatal error
      ErrorMessageHandle( lexer, true );
      return false;
    }
    
    // Construct the AST
    switch( type ) {
        
    case Lexer::LP_TARGET:
        
      printf( "Target AST\n" );

      for( i = 0, sep = false, fstr = false, wb = 0 ; ; i++ ) {
        
        // Syntax checks
        if( !i
            && tok != Lexer::TOK_STR
            && tok != Lexer::TOK_VAR ) {
          
          // Fatal error
          ErrorMessageHandle( lexer, true );
          return false;
        }
        
        // Get the variable name
        if( !i )
          snprintf( name, sizeof( name ), "%s",
                    lexer->ExtractTokenStr() );
        
        // See the first string token
        if( sep && fstr == false && tok == Lexer::TOK_STR )
          fstr = true;
        
        // See the sepatator
        if( sep == false && tok == Lexer::TOK_COLON ) {
          
          memset( buf, sizeof( buf ), 0 );
          sep = true;
        }
        
        // Copy the string to the buffer
        if( fstr ) {
          wb += snprintf( buf, sizeof( buf ) - wb, "%s",
                         lexer->ExtractTokenStr() );
        }
        
        // Get next token
        tok = lexer->GetNextToken();
        if( tok == Lexer::TOK_NEWLINE )
          break;
      }
        
      printf( "<TGT>\"%s\" : \"%s\"\n", name, buf );
      break;
        
    case Lexer::LP_RECEIPT:
      printf( "Receipt AST\n" );
      break;
        
    case Lexer::LP_VARIABLE:
        
      printf( "Variable AST\n" );

      for( i = 0, sep = false, fstr = false, wb = 0 ; ; i++ ) {
        
        // Syntax checks
        if( !i && tok != Lexer::TOK_STR ) {
          
          // Warning
          ErrorMessageHandle( lexer, false );
        }
        
        // Get the variable name
        if( !i )
          snprintf( name, sizeof( name ), "%s",
                    lexer->ExtractTokenStr() );
        
        // See the first string token
        if( sep && fstr == false && tok == Lexer::TOK_STR )
          fstr = true;
        
        // See the sepatator
        if( sep == false && tok == Lexer::TOK_EQUAL )
          sep = true;
        
        // Copy the string to the buffer
        if( fstr ) {
          wb += snprintf( buf, sizeof( buf ) - wb, "%s",
                        lexer->ExtractTokenStr() );
        }

        // Get next token
        tok = lexer->GetNextToken();
        if( tok == Lexer::TOK_NEWLINE )
          break;
      }
        
      printf( "<VAR>\"%s\" = \"%s\"\n", name, buf );
      break;
        
    case Lexer::LP_AMB_REC_VAR:
      printf( "Ambiguous Receipt Variable further distinguishment\n" );
      break;
    
    default:
    case Lexer::LP_NORMAL:
      printf( "Normal AST\n" );
      break;
    }
    
    
    // Update the line type prediction per each new line
    if( tok == Lexer::TOK_NEWLINE ) {
      
      type = lexer->PredictLineType();
      ln++; // Next line
    }
    
  } // while( tok = ... )
  
  
  // Success
  return true;
}


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <mymake.h>
#include <lexer.h>

using namespace mymake;


static Lexer::charToToken_t charToTokenTbl[] = {
  { ' ',  Lexer::TOK_SPACE },
  { '\t', Lexer::TOK_TAB },
  { ':',  Lexer::TOK_COLON },
  { '=',  Lexer::TOK_EQUAL },
  { '?',  Lexer::TOK_QMARK },
  { '+',  Lexer::TOK_PLUS },
  { '@',  Lexer::TOK_AT },
  { '<',  Lexer::TOK_LESSER },
  { '>',  Lexer::TOK_GREATER },
  { '%',  Lexer::TOK_PRECENT },
  { '^',  Lexer::TOK_CARET },
  { '*',  Lexer::TOK_STAR },
};


#define X(STR,TOK) { STR, strlen( STR ), TOK }
static Lexer::keywordToToken_t keywordToTokenTbl[] = {
  X( "vpath",                Lexer::TOK_VPATH ),
  X( "VPATH",                Lexer::TOK_VPATH_CAPS ),
  X( "include",              Lexer::TOK_INCLUDE ),
  X( "if",                   Lexer::TOK_IF ),
  X( "ifdef",                Lexer::TOK_IFDEF ),
  X( "ifndef",               Lexer::TOK_IFNDEF ),
  X( "endef",                Lexer::TOK_ENDEF ),
  X( "ifeq",                 Lexer::TOK_IFEQ ),
  X( "ifneq",                Lexer::TOK_IFNEQ ),
  X( "else",                 Lexer::TOK_ELSE ),
  X( "endif",                Lexer::TOK_ENDIF ),
  X( "load",                 Lexer::TOK_LOAD ),
  X( ".DEFAULT_GOAL",        Lexer::TOK_DEF_GOAL ),
  X( ".DEFAULT",             Lexer::TOK_DEF ),
  X( ".DELETE_ON_ERROR",     Lexer::TOK_DEL_ON_ERR ),
  X( ".EXPORT_ALL_VARIABLE", Lexer::TOK_EXP_ALL_VAR ),
  X( ".FEATURES",            Lexer::TOK_FEATURES ),
  X( ".IGNORE",              Lexer::TOK_IGNORE ),
  X( ".INCLUDE_DIRS",        Lexer::TOK_INC_DIRS ),
  X( ".INTERMEDIATE",        Lexer::TOK_INTERMED ),
  X( ".LIBPATTERNS",         Lexer::TOK_LIBPATTERNS ),
  X( ".LOADED",              Lexer::TOK_LOADED ),
  X( ".LOW_RESOLUTION_TIME", Lexer::TOK_LOW_RES_TIME ),
  X( ".NOTPARALLEL",         Lexer::TOK_NOTPARALLEL ),
  X( ".ONESHELL",            Lexer::TOK_ONESHELL ),
  X( ".PHONY",               Lexer::TOK_PHONY ),
  X( ".POSIX",               Lexer::TOK_POSIX ),
  X( ".PRECIOUS",            Lexer::TOK_PRECIOUS ),
  X( ".RECIPEPREFIX",        Lexer::TOK_RECIPE_PREFIX ),
  X( ".SECONDARY",           Lexer::TOK_SECONDARY ),
  X( ".SECONDEXPANSION",     Lexer::TOK_SECONDEXPANSION ),
  X( ".SHELLFLAGS",          Lexer::TOK_SHELLFLAGS ),
  X( ".SILENT",              Lexer::TOK_SLIENT ),
  X( ".SUFFIXES",            Lexer::TOK_SUFFIXES ),
  X( ".VARIABLES",           Lexer::TOK_VARIABLES ),
};


void Lexer::Restart() {

  restLen = fsize;
  tmpBufLen = 0;
  lineStartP = c = fp;
  tokenStrStart = tokenStrEnd = fp;
  currLine = 1;
  currTok = nextTok = TOK_INVALID;
}


bool Lexer::Open( const s8 *path ) {

  struct stat sb;

  // Open file
  ifd = open( path, O_RDONLY );
  if( ifd < 0 )
    return false;

  // Get the file size
  if( fstat( ifd, &sb ) == -1 )
    goto ErrExit;
  restLen = fsize = sb.st_size;

  // Sanity check
  if( fsize <= 0 )
    goto ErrExit;

  // Using mmap to map the file into memory
  fp = (u8 *)mmap( NULL, fsize, PROT_READ, MAP_PRIVATE, ifd, 0 );
  if( fp == MAP_FAILED )
    goto ErrExit;

  // Assignment
  lineStartP = c = fp;
  nextTok = currTok = TOK_BOF;

  // Successful
  initialized = true;
  return true;

ErrExit:
  close( ifd );
  return false;
}


bool Lexer::Open( const std::string &path ) {

  return Open( path.c_str() );
}


void Lexer::Close() {

  initialized = false;

  // Unmap the file
  if( fp != MAP_FAILED )
    munmap( fp, fsize );

  // Close the file
  if( ifd >= 0 )
    close( ifd );

  // Clear all states
  ClearStates();
}


bool Lexer::IsBOL() {

  if( IsBOF() )
    return true;

  if( *(c - 1) == '\n' )
    return true;

  return false;
}


static inline bool isAlphabet( const s8 c ) {

  if( c >= 'A' && c <= 'Z' )
    return true;

  if( c >= 'a' && c <= 'z' )
    return true;

  return false;
}


static inline bool isAlphabetNumber( const s8 c ) {

  if( isAlphabet( c ) )
    return true;

  if( c >= '0' && c <= '9' )
    return true;

  return false;
}


static bool isValidString( const s8 c ) {

  if( c == '$' || c == ':' )
    return false;

  if( c >= 0x21 && c <= 0x7E )
    return true;

  return false;
}


Lexer::Token Lexer::GetNextToken() {

  s32 i, offset = 1;

  // Invalidate the next & current tokens
	nextTok = currTok = TOK_INVALID;

  // Length check
  if( restLen <= 0 ) {

    currTok = TOK_EOF;
    return TOK_EOF;
  }

  // Comment
  if( *c == '#' ) {

    // It's definitely a COMMENT token
    currTok = TOK_COMMENT;
    tokenStrStart = c;
    offset = restLen; // Assume that it's reached the EOF
    for( i = 1 ; i < restLen ; i++ ) {

      if( *(c + i) == '\r' && *(c + i + 1) == '\n' ) {

        // Windows NL
        offset = i + 1;
        break;
      }
      else if( *(c + i) == '\n' ) {

        // Unix NL
        offset = i;
        break;
      }
    }

    // Determine the amount of the movement
    tokenStrEnd = c + offset - 1;
    goto ResRet;
  }

  // Newline
  if( *c == '\r' && *(c + 1) == '\n' ) {

    // Windows
    currTok = TOK_NEWLINE;
    tokenStrStart = c;
    tokenStrEnd = c + 1;

    lineStartP = tokenStrEnd + 1;
		currLine++;
    goto ResRet;
  }
  else if( *c == '\n' ) {

    // UNIX
    currTok = TOK_NEWLINE;
    tokenStrStart = tokenStrEnd = c;

    lineStartP = tokenStrEnd + 1;
    currLine++;
    goto ResRet;
  }

  // Variable type 1
  if( (*c == '$') && *(c + 1) == '(' ) {

    currTok = TOK_VAR;
    tokenStrStart = c + 2;

    // Assume it's reached the EOF
    tokenStrEnd = c + (restLen - 1);
    offset = restLen;

    // Look for a delimiter
    for( i = 2 ; i < restLen ; i++ ) {

      // Seeing a ')', treat it as the delimiter
      if( *(c + i) == ')' ) {

        offset = i + 1;
        tokenStrEnd = c + (i - 1);
        break;
      }
    }

    // Sanity check
    if( (tokenStrStart > tokenStrEnd)
      || (*(tokenStrEnd + 1) != ')') ) {

      currTok = TOK_INVALID;
      return TOK_INVALID;
    }

    goto ResRet;
  }

  // Variable type 2
  if( *c == '$' ) {

    currTok = TOK_VAR;
    tokenStrStart = tokenStrEnd = c + 1;
    offset = 2;

    goto ResRet;
  }

  // General character lookup
  for( i = 0 ; i < ARRAY_SIZE( charToTokenTbl ) ; i++ ) {

    if( *c == charToTokenTbl[ i ].c ) {

      currTok = charToTokenTbl[ i ].tok;
      tokenStrStart = c;
      tokenStrEnd = c;
      goto ResRet;
    }
  }

  // String
  if( isValidString( *c ) )  {

    s32 strLen;
    currTok = TOK_STR;
    tokenStrStart = c;

    // Assume it's reached the EOF
    tokenStrEnd = c + (restLen - 1);
    offset = restLen;

    // Look for a delimiter
    for( i = 1 ; i < restLen ; i++ ) {

      // Seeing a newline, space, dallor, or tab, treat it as a delimiter
      if( (*(c + i) == '\r' && *(c + i + 1) == '\n')
        || (*(c + i) == '\n' || *(c + i) == ' ' || *(c + i) == '\t')
        || (isValidString( *(c + i) ) == false) ) {

        offset = i;
        tokenStrEnd = c + (i - 1);
        break;
      }
    }

    // Determine that whether it's a keyword or not
    // Keyword lookup
    strLen = tokenStrEnd - tokenStrStart + 1;
    for( i = 0 ; i < ARRAY_SIZE( keywordToTokenTbl ) ; i++ ) {
      
      // Match the length first
      if( strLen != keywordToTokenTbl[ i ].keyStrLen )
        continue;
      
      // If it maches, then compare the content
      if( !strncmp( (const s8 *)tokenStrStart, keywordToTokenTbl[ i ].keyStr, strLen ) ) {
      
        // Found it, replace the TOK_STR with specified TOK_*
        currTok = keywordToTokenTbl[ i ].tok;
        break;
      }
    }

    goto ResRet;
  }

  // Rest of characters is treated as a lexer error
  return TOK_INVALID;

ResRet:
  c += offset;
  if( offset <= restLen )
    restLen -= offset;
  else
    restLen = 0;

  return currTok;
}


Lexer::Token Lexer::LookAheadToken() {

  // At the EOF
  if( currTok == TOK_EOF )
    return TOK_EOF;

  // Save states
  SaveStates();

  // Get next token
  GetNextToken();

  // Save the info of next token
  nextTok = currTok;
  nextTokenStrStart = tokenStrStart;
  nextTokenStrEnd = tokenStrEnd;

  // Restore states
  RestoreStates();

  return nextTok;
}


bool Lexer::ToNextLine() {

  s32 i, offset = restLen;
	bool nr = false;

  if( restLen <= 0 )
    return false;

  for( i = 0 ; i < restLen ; i++ ) {

    if( *(c + i) == '\r' && *(c + i + 1) == '\n' ) {

      // Windows NL
      offset = i + 2;
			nr = true;
      break;
    }
    else if( *(c + i) == '\n' ) {

      // Unix NL
      offset = i + 1;
			nr = true;
      break;
    }
  }

  // Move the pointer & the rest of length
  c += offset;
  restLen -= offset;
	if( nr == true ) {

		currLine++;
    lineStartP = c;
  }
  return true;
}


bool Lexer::ToPrevLine() {

  s32 i, rest = c - fp;
	s8 nr = 0;

  if( rest <= 0 )
    return false;

  for( i = 1 ; i < rest ; i++ ) {

    if( *(c - i) == '\n' ) {

      // Reach the BOL
      if( nr == 1 ) {

        // Fallback a character
        i--;
        break;
			}

      // Increase the line counter
      nr++;
    }
  }

  // Move the cursor
  c -= i;
  restLen += i;
  currLine -= nr;
  if( nr )
    lineStartP = c;

  return true;
}


bool Lexer::ToBeginOfLine() {

  s32 i, rest = c - fp;

  if( rest <= 0 )
    return false;

  for( i = 1 ; i < rest ; i++ ) {

    if( *(c - i) == '\n' )
      break;
  }

  // Move the cursor
  c -= (i - 1);
  restLen += (i - 1);
  lineStartP = c;

  return true;
}


Lexer::LineType Lexer::PredictLineType() {
  
  s32 i;
  Lexer::LineType type = LP_NORMAL;
  u8 tmp;
  bool signEq = false, signTab = false;
  bool signCol = false, signColEq = false;
  
  // Scan the line to determine the property
  for( i = 0 ; i < restLen ; i++ ) {
    
    // Read a char
    tmp = *(lineStartP + i);
    
    // It's reaches the EOL
    if( tmp == '\r' && *(lineStartP + i) == '\n' )
    break;
    else if( tmp == '\n' )
    break;
    
    // Character inspection
    if( i ) {
      
      // Not happen at the BOL
      // If there's a equal sign
      if( signEq == false && tmp == '=' )
      signEq = true;
      
      // If there's a ":=" sign
      // If there's a single ':' sign
      if( signColEq == false
         && tmp == ':'
         && *(lineStartP + i) == '=' )
      signColEq = true;
      else if( signCol == false && tmp == ':' )
      signCol = true;
    }
    else {
      
      // Happen at the BOL
      // If it's a TAB
      if( tmp == '\t' )
      signTab = true;
    }
  }
  
  // Determine the line type
  if( signTab && signEq )
  type = LP_AMB_REC_VAR;
  else if( signTab )
  type = LP_RECEIPT;
  else if( signEq )
  type = LP_VARIABLE;
  else if( signColEq )
  type = LP_VARIABLE;
  else if( signCol )
  type = LP_TARGET;
  
  // Return
  return type;
}


const u8 *Lexer::ExtractTokenStr() {

  tmpBufLen = tokenStrEnd - tokenStrStart + 1;
  if( (tmpBufLen < 0) || (tmpBufLen >= LEXER_TMPBUF_SIZE ) )
    return NULL;

  memcpy( tmpBuf, tokenStrStart, tmpBufLen );
  tmpBuf[ tmpBufLen ] = '\0';

  return tmpBuf;
}


const u8 *Lexer::ExtractLineStr() {

  s32 i;

  // Copy a line into buffer
  for( i = 0 ; i < restLen ; i++ ) {

    if( *(lineStartP + i) == '\r' && *(lineStartP + i) == '\n' )
      break;
    else if( *(lineStartP + i) == '\n' )
      break;

    tmpBuf[ i ] = *(lineStartP + i);
  }
  tmpBuf[ i ] = '\0';

  // Return
  return tmpBuf;
}


void Lexer::ClearStates() {
  
  initialized = false;
  ifd = -1;
  fsize = restLen = tmpBufLen = 0;
  lineStartP = fp = c = (u8 *)MAP_FAILED;
	currLine = 1;
  currTok = nextTok = TOK_INVALID;
}


void Lexer::SaveStates() {
  
  // Save states
  bakLen = restLen;
  bakLineStartP = lineStartP;
  bakC = c;
  bakTok = currTok;
  bakTokenStrStart = tokenStrStart;
  bakTokenStrEnd = tokenStrEnd;
  bakLine = currLine;
}


void Lexer::RestoreStates() {
  
  // Restore states
  restLen = bakLen;
  lineStartP = bakLineStartP;
  c = bakC;
  currTok = bakTok;
  tokenStrStart = bakTokenStrStart;
  tokenStrEnd = bakTokenStrEnd;
  currLine = bakLine;
}


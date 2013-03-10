#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "token.h"
#include "ast.h"

using namespace llvm;


std::string IdentifierStr;
double NumVal;


//
// Lexer
//
int gettok() {

	static int LastChar = ' ';

	// Skip any whitespace
	while( isspace( LastChar ) )
		LastChar = getchar();

	// Identifier: [a-zA-Z][a-zA-Z0-9]*
	if( isalpha( LastChar )) {

		IdentifierStr = LastChar;
		while( isalnum( (LastChar = getchar()) ) )
			IdentifierStr += LastChar;

		if( IdentifierStr == "def" )
			return tok_def;

		if( IdentifierStr == "extern" )
			return tok_extern;

		return tok_identifier;
	}

	// Number: [0-9.]+
	if( isdigit( LastChar ) || LastChar == '.' ) {

		std::string NumStr;
		do {

			NumStr += LastChar;
			LastChar = getchar();

		} while( isdigit( LastChar ) || LastChar == '.' );

		NumVal = strtod( NumStr.c_str(), 0 );
		return tok_number;
	}

	// Comment until end of line.
	if( LastChar == '#' ) {

		do LastChar = getchar();
		while( LastChar != EOF && LastChar != '\n' && LastChar != '\r' );

		if( LastChar != EOF )
			return gettok();
	}

	// Check for end of file. Don't eat the EOF
	if( LastChar == EOF )
		return tok_eof;

	// Otherwise, just return the character as its ascii value.
	int ThisChar = LastChar;
	LastChar = getchar();
	return ThisChar;

}



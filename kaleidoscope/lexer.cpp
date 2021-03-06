#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Verifier.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "token.h"


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

		if( IdentifierStr == "if" )
			return tok_if;

        if( IdentifierStr == "then" )
            return tok_then;

        if( IdentifierStr == "else" )
            return tok_else;

        if( IdentifierStr == "for" )
            return tok_for;

        if( IdentifierStr == "in" )
            return tok_in;

		if( IdentifierStr == "binary" )
			return tok_binary;

		if( IdentifierStr == "unary" )
			return tok_unary;

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



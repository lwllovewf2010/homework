#include "llvm/Support/raw_ostream.h"

using namespace llvm;


// gettok - Return the next token from standard input.
static int gettok() {

	static int LastChar = ' ';

	// Skip any
	while( isspace( LastChar ) )
		LastChar = getChar();

	if( isalpha( LastChar ) ) {

		IdentifierStr = LastChar;

		while( isalnum( (LastChar = getchar()) ) )
			IdentifierStr += LastChar;

		if( IdentifierStr == "def" ) return tok_def;
		if( IdentifierStr == "extern" ) return tok_extern;

		return tok_identifier;
	}
}


int main( int argc, char **argv ) {

	return 0;
}



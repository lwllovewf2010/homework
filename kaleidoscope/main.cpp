#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "token.h"
#include "ast.h"

using namespace llvm;


int main( int argc, char **argv ) {


	// Install standard binary operators.
	// 1 is lowest precedence.
	BinopPrecedence[ '<' ] = 10;
	BinopPrecedence[ '+' ] = 20;
	BinopPrecedence[ '-' ] = 20;
	BinopPrecedence[ '*' ] = 40;	// highest.

	// Prime the first token.
	fprintf( stderr, "ready> " );
	getNextToken();

	// Run the main "interpreter loop" now
	MainLoop();

    return 0;
}



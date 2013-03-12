#include "llvm/Support/raw_ostream.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

using namespace llvm;

#include "token.h"
#include "ast.h"


extern std::map<char, int> BinopPrecedence;
extern int CurTok;
extern Module *TheModule;


extern "C"
double putchard( double x ) {

	putchar( (char)x );
	return 0;
}


// top ::= definition | external | expression | ';'
static void MainLoop() {

    while( 1 ) {

        fprintf( stderr, "ready> " );
        switch( CurTok ) {

        case tok_eof:
            return;

        case ';':
            getNextToken();
            break;

        case tok_def:
            HandleDefinition();
            break;

        case tok_extern:
            HandleExtern();
            break;

        default:
            HandleTopLevelExpression();
            break;
        }
    }
}


int main( int argc, char **argv ) {

	LLVMContext &Context = getGlobalContext();

	// Install standard binary operators.
	// 1 is lowest precedence.
	BinopPrecedence[ '<' ] = 10;
	BinopPrecedence[ '+' ] = 20;
	BinopPrecedence[ '-' ] = 20;
	BinopPrecedence[ '*' ] = 40;	// highest.

	// Prime the first token.
	fprintf( stderr, "ready> " );
	getNextToken();

	// Make the module, which holds all the code.
	TheModule = new Module( "my cool jit", Context );

	// Run the main "interpreter loop" now
	MainLoop();

	// Print out all of the generated code.
	TheModule->dump();

    return 0;
}



#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/DataLayout.h"

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
extern FunctionPassManager *TheFPM;
extern ExecutionEngine *TheExecutionEngine;


extern "C"
double putchard( double x ) {

	putchar( (char)x );
	return 0;
}


extern "C"
double printd( double X ) {

	printf( "%f\n", X );
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

	InitializeNativeTarget();
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

	// Create he JIT. This takes ownership of the module.
	std::string ErrStr;
	TheExecutionEngine = EngineBuilder( TheModule ).setErrorStr( &ErrStr ).create();
	if( !TheExecutionEngine ) {

		fprintf( stderr, "Could not create ExecutionEngine: %s\n", ErrStr.c_str() );
		exit( 1 );
	}

	FunctionPassManager OurFPM( TheModule );

	// Set up the optimizer pipeline. Start with registering info about how the
	// target Lays out data structures.
	OurFPM.add( new DataLayout( *TheExecutionEngine->getDataLayout() ) );

	// Provide basic AliasAnalysis support for GVN.
	OurFPM.add( createBasicAliasAnalysisPass() );

	// Do simple "peephole" optimizations and bit-twiddling optzns.
	OurFPM.add( createInstructionCombiningPass() );

	// Reassociate expressions.
	OurFPM.add( createReassociatePass() );

	// Eliminate common SubExpressions.
	OurFPM.add( createGVNPass() );

	// Simplify the control flow graph (deleting unreachable blocks, etc).
	OurFPM.add( createCFGSimplificationPass() );

	OurFPM.doInitialization();

	TheFPM = &OurFPM;

	// Run the main "interpreter loop" now
	MainLoop();

	TheFPM = 0;

	// Print out all of the generated code.
	TheModule->dump();

    return 0;
}



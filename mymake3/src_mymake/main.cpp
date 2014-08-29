#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/DataLayout.h"

#include <mymake.h>
#include <lexer.h>
#include <parser.h>

using namespace mymake;
using namespace llvm;


Module *TheModule;
extern IRBuilder<> Builder;


static void usage( void ) {

  std::cerr << GM_COPYRIGHT_TEXT << "\n";
  std::cerr << GM_PROGRAM_NAME << ", Version " << GM_REVISION << "\n";
  std::cerr << "Author: " << GM_AUTHOR_NAME << "\n\n";

  std::cerr << "Usage: mymake [options] [target] ...\n";
	std::cerr << "Options:\n";
	std::cerr << "  -b, -m                         Ignored for compatibility.\n";
	std::cerr << "  -B, --always-make              Unconditionally make all targe\
    ts.\n";
	std::cerr << "-C DIRECTORY, --directory=DIRECTORY\n";
	std::cerr << "                                 Change to DIRECTORY before doi\
    ng anything.\n";
}


s32 main( s32 argc, s8 **argv ) {

	InitializeNativeTarget();
	LLVMContext &Context = getGlobalContext();
	Lexer *testf1 = new Lexer();
  Lexer *testf2 = new Lexer();
	Parser *makeParser = new Parser();

	// Handle arguments
	if( argc != 1 ) {

		usage();
		return 0;
	}

	// Create a module
  TheModule = new Module( "GorgeousMake", Context );

	// Initialize
	testf1->Open( "unittests/files/test1.mak" );
  testf2->Open( "unittests/files/test2.mak" );
	if( testf1->IsInit() == false || testf2->IsInit() == false ) {

		std::cerr << "Unable to open file\n";
		return 0;
	}

	// Create main function
	FunctionType *funcType = FunctionType::get(
    Type::getInt32Ty( getGlobalContext() ), false );

	Function *mainFunc = Function::Create(
    funcType, Function::ExternalLinkage, "main", TheModule );

	BasicBlock *entry = BasicBlock::Create( Context, "entrypoint", mainFunc );
	Builder.SetInsertPoint( entry );

	// Do parsing
	makeParser->Driver( testf1 );
  makeParser->Driver( testf2 );

	Builder.CreateRetVoid();

	// Print put all generated code
	TheModule->dump();

//ErrExit:
	testf1->Close();
  testf2->Close();
	delete testf1;
  delete testf2;
  delete makeParser;

	// Return
	return 0;
}



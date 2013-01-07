//
// Advanced Compiler Design Course (Fall 2012)
//   Homework #6: Hello Pass
//
//   Student ID : P01922001
//   Author     : Merck Hung
//   Email      : merckhung@gmail.com
//   Phone      : +886-975-586938
//

#define DEBUG_TYPE "HelloPass"

#include "llvm/Support/raw_ostream.h"

#include "llvm/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/Dominators.h"

using namespace llvm;

namespace {

	class HelloPass : public FunctionPass {
	public:
		static char ID;
		HelloPass() : FunctionPass( ID ) {}

		virtual bool runOnFunction( Function &F );
    };

	char HelloPass::ID = 0;
	RegisterPass<HelloPass> X( "hello", "Hello_Pass", false, false );
}


bool HelloPass::runOnFunction( Function &F ) {

	errs() << F.getName() << "\n";
	return false;
}



//
// Advanced Compiler Design Course (Fall 2012)
//   Homework #6: Local Redundancy Elmination
//
//   Student ID : P01922001
//   Author     : Merck Hung
//   Email      : merckhung@gmail.com
//   Phone      : +886-975-586938
//

#define DEBUG
#ifdef DEBUG
#define pdbg( MSG, ARGS... )    printf( "%s: " MSG, __func__, ##ARGS )
#else
#define pdbg( MSG, ARGS... )
#endif

#define DEBUG_TYPE "LocalRE"
#include "llvm/ADT/Statistic.h"
#include "llvm/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

	struct LocalRE : public FunctionPass {

		static char ID;
		LocalRE() : FunctionPass( ID ) {}

		virtual bool runOnFunction( Function &F ) {

			return false;
		}
	};
}

char LocalRE::ID = 0;
static RegisterPass<LocalRE> X( "LocalRE", "P01922001 Local Redundancy Elimination" );


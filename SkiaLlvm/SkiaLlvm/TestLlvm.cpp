#define DEBUG_TYPE "hello"

#include "llvm/Support/raw_ostream.h"

#include "llvm/ADT/Statistic.h"
#include "llvm/Function.h"
#include "llvm/Pass.h"

using namespace llvm;

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
    
    // Hello - The first implementation, without getAnalysisUsage.
    struct Hello : public FunctionPass {
        
        static char ID; // Pass identification, replacement for typeid
        Hello() : FunctionPass(ID) {}
        
        virtual bool runOnFunction(Function &F) {
            
            ++HelloCounter;
            errs() << "Hello: ";
            errs().write_escaped(F.getName()) << '\n';
            return false;
        }
    };
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");


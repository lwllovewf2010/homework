#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
	struct MerckPass : public FunctionPass {

		static char ID;
		MerckPass() : FunctionPass( ID ) {}

		virtual bool runOnFunction( Function &F ) {

			errs() << "MerckPass: " << F.getName() << "\n";
			return false;
		}
	};

	char MerckPass::ID = 0; 
	static RegisterPass<MerckPass> X( "MerckPass", "Merck's First Pass", false, false );

	struct MerckPass2 : public BasicBlockPass {

		static char ID;
		MerckPass2() : BasicBlockPass( ID ) {
		}

        virtual bool runOnBasicBlock( BasicBlock &BB ) {

            errs() << "MerckPass2: " << BB.getName() << "\n";
            return false;
        }
	};

	char MerckPass2::ID = 0;
	static RegisterPass<MerckPass2> Y( "MerckPass2", "Merck's Second Pass", false, false );
}


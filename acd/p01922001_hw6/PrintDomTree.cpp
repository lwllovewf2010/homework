//
// Advanced Compiler Design Course (Fall 2012)
//   Homework #6: Print Dominator Tree
//
//   Student ID : P01922001
//   Author     : Merck Hung
//   Email      : merckhung@gmail.com
//   Phone      : +886-975-586938
//

#define DEBUG_TYPE "PrintDomTree"

#include "llvm/Support/raw_ostream.h"

#include "llvm/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/Dominators.h"

using namespace llvm;

namespace {

	class PrintDomTree : public DominatorTree {
	public:
		static char ID;
		PrintDomTree() : DominatorTree() {}

		virtual void getAnalysisUsage( AnalysisUsage &AU ) const;
		virtual bool runOnFunction( Function &F );
    };

	char PrintDomTree::ID = 0;
	RegisterPass<PrintDomTree> X( "pdomtree", "Print_Dom_Tree", false, false );
}


void PrintDomTree::getAnalysisUsage( AnalysisUsage &AU ) const {

	AU.setPreservesCFG();
	AU.addRequired<DominatorTree >();
}


static void ProcessDomTree( const DomTreeNode *Node, size_t Level ) {

	if( !Node )
		return;

	// Print current basic block
	errs().indent( Level * 2 + 2 );

	BasicBlock *BB = Node->getBlock();

	if( BB->hasName() )
		errs() << BB->getName() << "\n";
	else
	 	errs() << "<unnamed>\n";

	// Traverse child nodes
	for( DomTreeNode::const_iterator I = Node->begin(),
		E = Node->end() ; I != E ; ++I ) {

		ProcessDomTree( *I, Level + 1 );
	}


}


bool PrintDomTree::runOnFunction( Function &F ) {

	errs() << F.getName() << "\n";

	DominatorTree &DT = getAnalysis<DominatorTree >();
	ProcessDomTree( DT.getRootNode(), 0 );

    return false;
}



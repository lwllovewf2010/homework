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

#include "llvm/Support/raw_ostream.h"

#include "llvm/Instructions.h"
#include "llvm/Operator.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/Dominators.h"

using namespace llvm;

namespace {

	struct LocalRE : public FunctionPass {

		static char ID;
		LocalRE() : FunctionPass( ID ) {}
		virtual bool runOnFunction( Function &F );
	};

	char LocalRE::ID = 0;
	static RegisterPass<LocalRE> X( "LocalRE", "P01922001 Local Redundancy Elimination" );	
}


inline static bool IsSupportedBinaryOperator( const BinaryOperator &Op ) {

	Instruction::BinaryOps Opcode = Op.getOpcode();

	return ( Opcode == Instruction::Add || Opcode == Instruction::FAdd
			|| Opcode == Instruction::Sub || Opcode == Instruction::FSub
			|| Opcode == Instruction::Mul || Opcode == Instruction::FMul
			|| Opcode == Instruction::Shl || Opcode == Instruction::LShr
			|| Opcode == Instruction::AShr || Opcode == Instruction::And
			|| Opcode == Instruction::Or || Opcode == Instruction::Xor );
}


inline static bool IsEqual( const BinaryOperator &A,
							const BinaryOperator &B ) {

	return ((A.getOpcode() == B.getOpcode())
			&& (!isa<OverflowingBinaryOperator >( A )
				|| A.hasNoUnsignedWrap() == B.hasNoUnsignedWrap())
			&& (!isa<OverflowingBinaryOperator >( A )
				|| A.hasNoSignedWrap() == B.hasNoSignedWrap())
			&& (!isa<PossiblyExactOperator >( A )
				|| A.isExact() == B.isExact())
			&& (A.getOperand( 0 ) == B.getOperand( 0 ))
			&& (A.getOperand( 1 ) == B.getOperand( 1 )) );
}


bool LocalRE::runOnFunction( Function &F ) {

	bool Changed = false;

	for( Function::iterator BI = F.begin(),
			BE = F.end() ; BI != BE ; ++BI ) {

		::std::vector<BinaryOperator *> Redundancies;
		::std::vector<BinaryOperator *> History;

		for( BasicBlock::iterator I = BI->begin(),
				E = BI->end() ; I != E ; ++I ) {

			if( BinaryOperator *BOp = dyn_cast<BinaryOperator >( I ) ) {

				// Only support a limited subset of binary operator
				if( !IsSupportedBinaryOperator( *BOp ) )
					continue;

				// Search for equivalent instruction in the history
				BinaryOperator *EqualOp = 0;
				for( size_t i = 0 ; i < History.size() ; ++i ) {

					if( IsEqual( *BOp, *History[ i ] ) ) {

						EqualOp = History[ i ];
						break;
					}
				}

				if( !EqualOp ) {

					// If no equivalent instruction in the history, then add this
					// instruction to the history
					History.push_back( BOp );
				}
				else {

					// BOp is a redundancy instruction. Let's replace all of the
					// usage of BOp, and add them to redundant set, so that we can
					// erase them later. We can't erase them currently, because we
					// are still walking through the linking list.
					BOp->replaceAllUsesWith( EqualOp );
					Redundancies.push_back( BOp );
					Changed = true;
				}
			}
		}

		// Erase the redundant instructions
		for( size_t i = 0 ; i < Redundancies.size() ; ++i ) {

			Redundancies[ i ]->eraseFromParent();
		}
	}

	return Changed;
}



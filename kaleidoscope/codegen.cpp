#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/PassManager.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

using namespace llvm;

#include "token.h"
#include "ast.h"


//
// Code Generation
//
Module *TheModule;
IRBuilder<> Builder( getGlobalContext() );
std::map<std::string, Value *> NamedValues;
FunctionPassManager *TheFPM;
extern std::map<char, int> BinopPrecedence;


Value *ErrorV( const char *Str ) {

	Error( Str );
	return 0;
}


Value *NumberExprAST::Codegen() {

	return ConstantFP::get( getGlobalContext(), APFloat( Val ) );
}


Value *VariableExprAST::Codegen() {

	// Look this variable up in the function.
	Value *V = NamedValues[ Name ];
	return V ? V : ErrorV( "Unknown variable name" );
}


Value *BinaryExprAST::Codegen() {

	Value *L = LHS->Codegen();
	Value *R = RHS->Codegen();

	if( L == 0 || R == 0 )
		return 0;

	switch( Op ) {

	case '+':
		return Builder.CreateFAdd( L, R, "addtmp" );

	case '-':
		return Builder.CreateFSub( L, R, "subtmp" );

	case '*':
		return Builder.CreateFMul( L, R, "multmp" );

	case '<':
		L = Builder.CreateFCmpULT( L, R, "cmptmp" );
		// Convert bool 0/1 to double 0.0 or 1.0
		return Builder.CreateUIToFP( L, Type::getDoubleTy( getGlobalContext() ), "booltmp" );

	default:
		break;
	}

	// If it wasn't a builtin binary operator, it must be a user defined one.
	// Emit a call to it.
	Function *F = TheModule->getFunction( std::string( "binary" ) + Op );
	assert( F && "binary operator not found!" );

	Value *Ops[ 2 ] = { L, R };
	return Builder.CreateCall( F, Ops, "binop" );
}


Value *CallExprAST::Codegen() {

	// Look up the name in the global module table.
	Function *CalleeF = TheModule->getFunction( Callee );
	if( CalleeF == 0 )
		return ErrorV( "Unknown function referenced" );

	// If argument mismatch error.
	if( CalleeF->arg_size() != Args.size() )
		return ErrorV( "Incorrect # arguments passed" );

	std::vector<Value *> ArgsV;
	for( unsigned i = 0, e = Args.size() ;
		i != e ; ++i ) {

		ArgsV.push_back( Args[ i ]->Codegen() );
		if( ArgsV.back() == 0 )
			return 0;
	}

	return Builder.CreateCall( CalleeF, ArgsV, "calltmp" );
}


Function *PrototypeAST::Codegen() {

	// Make the function type: double(double, double) etc.
	std::vector<Type *> Doubles( Args.size(),
								Type::getDoubleTy( getGlobalContext() ) );

	FunctionType *FT = FunctionType::get( Type::getDoubleTy( getGlobalContext() ),
										Doubles, false );

	Function *F = Function::Create( FT, Function::ExternalLinkage, Name, TheModule );

	// If F conflicted, there was already something named 'Name'. If it has a
	// body, don't allow redefinition or reextern.
	if( F->getName() != Name ) {

		// Delete the one we just made and get the existing one.
		F->eraseFromParent();
		F = TheModule->getFunction( Name );

		// If F already has a body, reject this.
		if( !F->empty() ) {

			ErrorF( "redefinition of function" );
			return 0;
		}

		// If F took a different number of args, reject.
		if( F->arg_size() != Args.size() ) {

			ErrorF( "redefintion of function with different # args" );
			return 0;
		}
	}

	// Set names for all arguments.
	unsigned Idx = 0;
	for( Function::arg_iterator AI = F->arg_begin() ;
		Idx != Args.size() ; ++AI, ++Idx ) {

		AI->setName( Args[ Idx ] );

		// Add arguments to variable symbol table.
		NamedValues[ Args[ Idx ] ] = AI;
	}

	return F;
}


Function *FunctionAST::Codegen() {

	NamedValues.clear();

	Function *TheFunction = Proto->Codegen();
	if( TheFunction == 0 )
		return 0;

	// If this is an operator, install it
	if( Proto->isBinaryOp() )
		BinopPrecedence[ Proto->getOperatorName() ] = Proto->getBinaryPrecedence();

	// Create a new basic block to start insertion into.
	BasicBlock *BB = BasicBlock::Create( getGlobalContext(), "entry", TheFunction );
	Builder.SetInsertPoint( BB );

	if( Value *RetVal = Body->Codegen() ) {

		// Finish off the function.
		Builder.CreateRet( RetVal );

		// Validate the generated code, checking for consistency
		verifyFunction( *TheFunction );

		// Optimize the function.
		TheFPM->run( *TheFunction );

		return TheFunction;
	}

	// Error reading body, remove function.
	TheFunction->eraseFromParent();
	return 0;
}


Value *IfExprAST::Codegen() {

	Value *CondV = Cond->Codegen();
	if( CondV == 0 )
		return 0;

	// Convert condition to a bool by comparing equal to 0.0
	CondV = Builder.CreateFCmpONE( CondV,
		ConstantFP::get( getGlobalContext(),
		APFloat( 0.0 )),
		"ifcond" );

	Function *TheFunction = Builder.GetInsertBlock()->getParent();

	// Create blocks for the then and else cases.
	// Insert the 'then' block at the end of the function.
	BasicBlock *ThenBB = BasicBlock::Create( 
							getGlobalContext(), 
							"then", 
							TheFunction );
	BasicBlock *ElseBB = BasicBlock::Create(
							getGlobalContext(),
							"else" );
	BasicBlock *MergeBB = BasicBlock::Create(
							getGlobalContext(),
							"ifcont" );

	Builder.CreateCondBr( CondV, ThenBB, ElseBB );

	// Emit then value
	Builder.SetInsertPoint( ThenBB );

	Value *ThenV = Then->Codegen();
	if( ThenV == 0 )
		return 0;

	Builder.CreateBr( MergeBB );
	// Codegen of 'Then' can change the current block, update ThenBB for the PHI
	ThenBB = Builder.GetInsertBlock();

	// Emit else block.
	TheFunction->getBasicBlockList().push_back( ElseBB );
	Builder.SetInsertPoint( ElseBB );

	Value *ElseV = Else->Codegen();
	if( ElseV == 0 )
		return 0;

	Builder.CreateBr( MergeBB );
	// Codegen of 'Else' can change the current block, update ElseBB for the PHI
	ElseBB = Builder.GetInsertBlock();

	// Emit merge block
	TheFunction->getBasicBlockList().push_back( MergeBB );
	Builder.SetInsertPoint( MergeBB );
	PHINode *PN = Builder.CreatePHI( Type::getDoubleTy( getGlobalContext() ),
									2,
									"iftmp" );

	PN->addIncoming( ThenV, ThenBB );
	PN->addIncoming( ElseV, ElseBB );

	return PN;
}


Value *ForExprAST::Codegen() {

	// Output this as:
	//   ...
	//   start = startexpr
	//   goto Loop
	// Loop:
	//   variable = phi [start, Loopheader], [nextvariable, Loopend]
	//   ...
	//   bodyexpr
	//   ...
	// Loopend:
	//   step = stepexpr
	//   nextvariable = variable + step
	//   endcond = enxexpr
	//   br endcond, Loop, endLoop
	// outLoop:

	// Emit the start code first, without 'variable' in scope
	Value *StartVal = Start->Codegen();
	if( StartVal == 0 )
		return 0;

	// Make the new basic block for the Loop header, inserting after current
	// block.
	Function *TheFunction = Builder.GetInsertBlock()->getParent();
	BasicBlock *PreheaderBB = Builder.GetInsertBlock();
	BasicBlock *LoopBB = BasicBlock::Create( getGlobalContext(), "loop", TheFunction );

	// Insert an explicit fall through from the current block to the LoopBB
	Builder.CreateBr( LoopBB );

	// Start insertion in LoopBB.
	Builder.SetInsertPoint( LoopBB );

	// Start the PHI node with an entry for Start.
	PHINode *Variable = Builder.CreatePHI( Type::getDoubleTy( getGlobalContext() ), 2, VarName.c_str() );
	Variable->addIncoming( StartVal, PreheaderBB );

	// Within the Loop, the variable is defined equal to the PHI node.
	// If it shadows an existing variable, we have to restore it,
	// sp save it now.
	Value *OldVal = NamedValues[ VarName ];
	NamedValues[ VarName ] = Variable;

	// Emit the body of the Loop.
	// This, Like any other expr, can change the current BB.
	// Note that we ignore the value computed by the body,
	// but don't allow an error.
	if( Body->Codegen() == 0 )
		return 0;

	// Emit the step value.
	Value *StepVal;
	if( Step ) {

		StepVal = Step->Codegen();
		if( StepVal == 0 )
			return 0;
	}
	else {

		// If not specified, use 1.0.
		StepVal = ConstantFP::get( getGlobalContext(), APFloat( 1.0 ) );
	}

	Value *NextVar = Builder.CreateFAdd( Variable, StepVal, "nextvar" );

	// Compute the end condition
	Value *EndCond = End->Codegen();
	if( EndCond == 0 )
		return EndCond;

	// Convert condition to a bool by comparing equal to 0.0.
	EndCond = Builder.CreateFCmpONE( EndCond,
				ConstantFP::get( getGlobalContext(), APFloat( 0.0 )),
				"loopcond" );

	// Create the "after Loop" block and insert it.
	BasicBlock *LoopEndBB = Builder.GetInsertBlock();
	BasicBlock *AfterBB = BasicBlock::Create( getGlobalContext(), "afterloop", TheFunction );

	// Insert the conditional branch into the end of LoopEndBB
	Builder.CreateCondBr( EndCond, LoopBB, AfterBB );

	// Any new code will be inserted in AfterBB.
	Builder.SetInsertPoint( AfterBB );

	// Add a new entry to the PHI node for the backedge.
	Variable->addIncoming( NextVar, LoopEndBB );

	// Restore the unshadowed variable.
	if( OldVal )
		NamedValues[ VarName ] = OldVal;
	else
		NamedValues.erase( VarName );

	// for expr always returns 0.0.
	return Constant::getNullValue( Type::getDoubleTy( getGlobalContext() ) );
}


Value *UnaryExprAST::Codegen() {

	Value *OperandV = Operand->Codegen();

	if( OperandV == 0 )
		return 0;

	Function *F = TheModule->getFunction( std::string( "unary" ) + Opcode );
	if( F == 0 )
		return ErrorV( "Unknown unary operator" );

	return Builder.CreateCall( F, OperandV, "unop" );
}



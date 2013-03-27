#include "llvm/Support/raw_ostream.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/JIT.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

using namespace llvm;

#include "token.h"
#include "ast.h"


//
// Parser
//


// BinopPrecedence - This holds the precedence for each binary operator that is
// defined.
std::map<char, int> BinopPrecedence;


extern std::string IdentifierStr;
extern double NumVal;
ExecutionEngine *TheExecutionEngine;


// CurTok/getNextToken - Provide a simple token buffer. CurTok is the current
// token the parser is looking at. getNextToken reads another token from the
// Lexer and updates CurTok with its results.
int CurTok;


int getNextToken() {

	return CurTok = gettok();
}


// GetTokPrecedence - Get the precedence of the pending binary operator token.
int GetTokPrecedence() {

	// Make sure it's a ASCII character
	if( !isascii( CurTok ) )
		return -1;

	// Make sure it's a declared binop.
	int TokPrec = BinopPrecedence[ CurTok ];
	if( TokPrec <= 0 )
		return -1;

	return TokPrec;
}


// Error* - These are little helper functions for error handling.
ExprAST *Error( const char *str ) {

	fprintf( stderr, "Error: %s\n", str );
	return 0;
}


PrototypeAST *ErrorP( const char *str ) {

	Error( str );
	return 0;
}


FunctionAST *ErrorF( const char *str ) {

	Error( str );
	return 0;
}


// identifierexpr
//   ::= identifier
//   ::= identifier '(' expression* ')'
static ExprAST *ParseIdentifierExpr() {

	std::string IdName = IdentifierStr;

	// Eat identifier
	getNextToken();

	// Simple variable ref.
	if( CurTok != '(' )
		return new VariableExprAST( IdName );

	// Call.
	getNextToken();		// Eat (
	std::vector<ExprAST *> Args;

	if( CurTok != ')' ) {

		while( 1 ) {

			ExprAST *Arg = ParseExpression();
			if( !Arg )
				return 0;

			Args.push_back( Arg );

			if( CurTok == ')' )
				break;

			if( CurTok != ',' )
				return Error( "Expected ')' or ',' in argument list" );

			getNextToken();
		}
	}

	// Eat )
	getNextToken();

	return new CallExprAST( IdName, Args );
}


// numberexpr ::= number
static ExprAST *ParseNumberExpr() {

	ExprAST *Result = new NumberExprAST( NumVal );

	getNextToken();		// consume the number
	return Result;
}


// parenexpr ::= '(' expression ')'
static ExprAST *ParseParenExpr() {

	getNextToken();		// eat (.

	ExprAST *V = ParseExpression();
	if( !V )
		return 0;

	if( CurTok != ')' )
		return Error( "expected ')'" );

	getNextToken();		// eat ).

	return V;
}


// Ifexpr ::= 'if' expression 'then' expression 'else' expression
ExprAST *ParseIfExpr() {

	getNextToken(); // eat the if

	// condition.
	ExprAST *Cond = ParseExpression();
	if( !Cond )
		return 0;

	if( CurTok != tok_then )
		return Error( "expected then" );

	getNextToken(); // eat the then

	ExprAST *Then = ParseExpression();
	if( Then == 0 )
		return 0;

	if( CurTok != tok_else )
		return Error( "expected else" );

	getNextToken();

	ExprAST *Else = ParseExpression();
	if( !Else )
		return 0;

	return new IfExprAST( Cond, Then, Else );
}


// forexpr ::= 'for' identifier '=' expr ',' (',' expr)? 'in' expression
ExprAST *ParseForExpr() {

	getNextToken(); // eat the for

	if( CurTok != tok_identifier )
		return Error( "expected identifier after for" );

	std::string IdName = IdentifierStr;
	getNextToken();

	if( CurTok != '=' )
		return Error( "expected '=' after for" );
	getNextToken(); // eat '='


	ExprAST *Start = ParseExpression();
	if( Start == 0 )
		return 0;

	if( CurTok != ',' )
		return Error( "expected ',' after for start value" );
	getNextToken();

	ExprAST *End = ParseExpression();
	if( End == 0 )
		return 0;

	// The step value if optional
	ExprAST *Step = 0;
	if( CurTok == ',' ) {

		getNextToken();
		Step = ParseExpression();
		if( Step == 0 )
			return 0;
	}

	if( CurTok != tok_in )
		return Error( "expected 'in' after for" );
	getNextToken(); // eat 'in'

	ExprAST *Body = ParseExpression();
	if( Body == 0 )
		return 0;

	return new ForExprAST( IdName, Start, End, Step, Body );
}


// primary
//   ::= identifierexpr
//   ::= numberexpr
//   ::= parenexpr
static ExprAST *ParsePrimary() {

	switch( CurTok ) {

	default:
		return Error( "Unknown token when expecting an expression" );

	case tok_identifier:
		return ParseIdentifierExpr();

	case tok_number:
		return ParseNumberExpr();

	case '(':
		return ParseParenExpr();

	case tok_if:
		return ParseIfExpr();

	case tok_for:
		return ParseForExpr();
	}
}


// binoprhs
//   ::= ('+' primary)*
static ExprAST *ParseBinOpRHS( int ExprPrec, ExprAST *LHS ) {

	// If this is a binop, find its precedence
	while( 1 ) {

		int TokPrec = GetTokPrecedence();

		// If this is a binop that binds at least as tightly as yhr current binop,
		// consume it, otherwise we are done.
		if( TokPrec < ExprPrec )
			return LHS;

		// Okay, we know this is a binop
		int BinOp = CurTok;
		getNextToken();		// eat binop

		// Parse the primary expression after the binary operator
		ExprAST *RHS = ParsePrimary();
		if( !RHS )
			return 0;

		// If BinOp binds less tightly with RHS than the operator after RHS, Let
		// the pending operator take as its LHS.
		int NextPrec = GetTokPrecedence();
		if( TokPrec < NextPrec ) {

			RHS = ParseBinOpRHS( TokPrec + 1, RHS );
			if( RHS == 0 )
				return 0;
		}

		// Merge LHS/RHS
		LHS = new BinaryExprAST( BinOp, LHS, RHS );
	}
}


// expression
//   ::= primary binoprhs
ExprAST *ParseExpression() {

	ExprAST *LHS = ParsePrimary();

	if( !LHS )
		return 0;

	return ParseBinOpRHS( 0, LHS );
}


// prototype
//   ::= id '(' id* ')'
//   ::= binary LETTER number? (id, id)
static PrototypeAST *ParsePrototype() {

	std::string FnName;

	unsigned Kind = 0; // 0 = identifier, 1 = unary, 2 = binary
	unsigned BinaryPrecedence = 30;

	switch( CurTok ) {

	default:
		return ErrorP( "Expected function name in prototype" );

	case tok_identifier:
		FnName = IdentifierStr;
		Kind = 0;
		getNextToken();
		break;

	case tok_binary:
		getNextToken();
		if( !isascii( CurTok ) )
			return ErrorP( "Expected binary operator" );
		FnName = "binary";
		FnName += (char)CurTok;
		Kind = 2;
		getNextToken();

		// Read the precedence if present
		if( CurTok == tok_number ) {

			if( NumVal < 1 || NumVal > 100 )
				return ErrorP( "Invalid precedence: must be 1..100" );
			BinaryPrecedence = (unsigned)NumVal;
			getNextToken();
		}
		break;
	}

	if( CurTok != '(' )
		return ErrorP( "Expected '(' in prototype" );

	std::vector<std::string> ArgNames;
	while( getNextToken() == tok_identifier )
		ArgNames.push_back( IdentifierStr );

	if( CurTok != ')' )
		return ErrorP( "Expected ')' in prototype" );

	// success.
	getNextToken();		// eat ')'.

	// Verify right number of names for operator
	if( Kind && ArgNames.size() != Kind )
		return ErrorP( "Invalid number of operands for operator" );

	return new PrototypeAST( FnName, ArgNames, Kind != 0, BinaryPrecedence );
}


// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() {

	getNextToken();		// eat def.

	PrototypeAST *Proto = ParsePrototype();

	if( Proto == 0 )
		return 0;

	// Handle the body of the function
	if( ExprAST *E = ParseExpression() )
		return new FunctionAST( Proto, E );

	return 0;
}


// topLevelexpr ::= expression
static FunctionAST *ParseTopLevelExpr() {

	if( ExprAST *E = ParseExpression() ) {

		// Make an anonymous proto.
		PrototypeAST *Proto = new PrototypeAST( "", std::vector<std::string>() );
		return new FunctionAST( Proto, E );
	}

	return 0;
}


// external ::= 'extern' prototype
static PrototypeAST *ParseExtern() {

	getNextToken();		// eat extern
	return ParsePrototype();
}


//
// Top-Level parsing
//
void HandleDefinition() {

	if( FunctionAST *F = ParseDefinition() ) {

		if( Function *LF = F->Codegen() ) {

			fprintf( stderr, "Read function defintion:" );
			LF->dump();
		}
	}
	else {

		// Skip token for error recovery
		getNextToken();
	}
}


void HandleExtern() {

	if( PrototypeAST *P = ParseExtern() ) {

		if( Function *F = P->Codegen() ) {

			fprintf( stderr, "Read extern:" );
			F->dump();
		}
	}
	else {

		// Skip token for error recovery
		getNextToken();
	}
}


void HandleTopLevelExpression() {

	// Evaluate a top-level expression into an anonymous function.
	if( FunctionAST *F = ParseTopLevelExpr() ) {

		if( Function *LF = F->Codegen() ) {

			fprintf( stderr, "Read top-level experssion:" );
			LF->dump();

			// JIT the function, returning a function pointer
			void *FPtr = TheExecutionEngine->getPointerToFunction( LF );

			// Case it to the right type (takes no arguments, returns a 
			// double) so we can call it as a native function.
			double (*FP)() = (double (*)())(intptr_t)FPtr;
			fprintf( stderr, "Evaluated to %f\n", FP() );
		}
	}
	else {

		// Skip token for error recovery
		getNextToken();
	}
}



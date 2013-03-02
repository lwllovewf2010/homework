#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "token.h"
#include "ast.h"

using namespace llvm;


//
// Parser
//

// CurTok/getNextToken - Provide a simple token buffer. CurTok is the current
// token the parser is looking at. getNextToken reads another token from the
// Lexer and updates CurTok with its results.
static int CurTok;


int getNextToken() {

	return CurTok = gettok();
}


// BinopPrecedence - This holds the precedence for each binary operator that is
// defined.
static std::map<char, int> BinopPrecedence;
 

// GetTokPrecedence - Get the precedence of the pending binary operator token.
int GetTokPrecedence() {

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


static ExprAST *ParseExpression();


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
		getNextToken;		// eat binop

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
static ExprAST *ParseExpression() {

	ExprAST *LHS = ParsePrimary();

	if( !LHS )
		return 0;

	return ParseBinOpRHS( 0, LHS );
}


// prototype
//   ::= id '(' id* ')'
static PrototypeAST *ParsePrototype() {

	if( CurTok != tok_identifier )
		return ErrorP( "Expected function name in prototype" );

	std::string FnName = IdentifierStr;
	getNextToken();

	if( CurTok != '(' )
		return ErrorP( "Expected '(' in prototype" );

	std::vector<std::string> ArgNames;
	while( getNextToken() == tok_identifier )
		ArgNames.push_back( IdentifierStr );

	if( CurTok != ')' )
		return ErrorP( "Expected ')' in prototype" );

	// success.
	getNextToken();		// eat ')'.

	return new PrototypeAST( FnName, ArgNames );
}


// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() {

	getNextToken();		// eat def.

	PrototypeAST *Proto = ParsePrototype();

	if( Proto == 0 )
		return 0;

	if( ExprAST *E = ParseExpression() )
		return new FunctionAST( Proto, E );

	return 0;
}


// topLevelexpr ::= expression
static FunctionAST *ParseTpLevelExpr() {

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
static void HandleDefinition() {

	if( ParseDefinition() ) {

		fprintf( stderr, "Parsed a function defintion.\n" );
	}
	else {

		// Skip token for error recovery
		getNextToken();
	}
}


static void HandleExtern() {

	if( ParseExtern() ) {

		fprintf( stderr, "Pased an extrn\n" );
	}
	else {

		// Skip token for error recovery
		getNextToken();
	}
}


static void HandleTopLevelExpression() {

	// Evaluate a top-level expression into an anonymous function.
	if( ParseTopLevelExpr() ) {

		fprintf( stderr, "Parsed a top-level expr\n" );
	}
	else {

		// Skip token for error recovery
		getNextToken();
	}
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
			handleTopLevelExpression();
			break;
		}
	} 
}



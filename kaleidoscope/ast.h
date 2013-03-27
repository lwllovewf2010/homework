//
// Abstract Syntax Tree (aka Parse Tree)
//


// ExprAST - Base class for all express nodes.
class ExprAST {

public:
	virtual ~ExprAST() {}
	virtual Value *Codegen() = 0;
};


// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {

	double Val;

public:
	NumberExprAST( double val ) : Val( val ) {}
	virtual Value *Codegen();
};


// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {

	std::string Name;

public:
	VariableExprAST( const std::string &name ) : Name( name ) {}
	virtual Value *Codegen();
};


// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {

	char Op;
	ExprAST *LHS, *RHS;

public:
	BinaryExprAST( char op, ExprAST *lhs, ExprAST *rhs )
		: Op( op ), LHS( lhs ), RHS( rhs ) {}

	virtual Value *Codegen();
};


// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {

	std::string Callee;
	std::vector<ExprAST *> Args;

public:
	CallExprAST( const std::string &callee, std::vector<ExprAST *> &args )
		: Callee( callee ), Args( args ) {}

	virtual Value *Codegen();
};


// PrototypeAST - This class represents the "prototype" for a function,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
class PrototypeAST {

	std::string Name;
	std::vector<std::string> Args;

public:
	PrototypeAST( const std::string &name, const std::vector<std::string> &args ) 
		: Name( name ), Args( args ) {}

	Function *Codegen();
};


// FunctionAST - This class represents a function definition itself.
class FunctionAST {

	PrototypeAST *Proto;
	ExprAST *Body;

public:
	FunctionAST( PrototypeAST *proto, ExprAST *body )
		: Proto( proto ), Body( body ) {}

	Function *Codegen();
};


// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {

	ExprAST *Cond, *Then, *Else;

public:
	IfExprAST( ExprAST *cond, ExprAST *then, ExprAST *_else )
		: Cond( cond ), Then( then ), Else( _else ) {}

	virtual Value *Codegen();
};


// Prototypes
int getNextToken();
void HandleDefinition();
void HandleExtern();
void HandleTopLevelExpression();
ExprAST *ParseExpression();
ExprAST *Error( const char *str );
FunctionAST *ErrorF( const char *str );



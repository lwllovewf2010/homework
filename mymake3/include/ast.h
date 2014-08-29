#ifndef MYMAKE_AST_H_
#define MYMAKE_AST_H_

#include <vector>


namespace mymake {


class Ast {

public:
	virtual ~Ast() {}
	virtual llvm::Value *codegen() = 0;
};


class TargetAst : public Ast {

	std::string target;
	std::vector<Ast *> preReqs;
	std::string recipe;

public:
	TargetAst( const std::string &target,
				std::vector<Ast *> &preRegs,
				const std::string &recipe ) :
	target( target ), preReqs( preRegs ), recipe( recipe ) {}

	virtual llvm::Value *codegen();
};


class VarReadAst : public Ast {

	std::string variable;

public:
	VarReadAst( const std::string &variable ) :
	variable( variable ) {}

	virtual llvm::Value *codegen() { return 0; }
};


class VarAst : public Ast {

	std::string name;
  std::string val;

public:
	VarAst( const std::string &name, const std::string &val ) :
	name( name ), val( val ) {}

	virtual llvm::Value *codegen() { return 0; }
};


class FuncCallAst : public Ast {

	std::string func;

public:
	FuncCallAst( const std::string &func ) :
	func( func ) {}

	virtual llvm::Value *codegen() { return 0; }
};


} // end namespace mymake

#endif // MYMAKE_AST_H_


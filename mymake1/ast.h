#ifndef GORMAKE_AST_H_
#define GORMAKE_AST_H_

#include <vector>

namespace gormake {


class Ast {

public:
	virtual ~Ast() {}
};


class TokenString {

	std::string 

public:
	virtual ~TokenString() {}
};


class TargetAst : public Ast {

	std::vector<Ast *> target;
	std::vector<Ast *> prerequisite;
	std::vector<Ast *> recipe;

public:
	TargetAst( std::vector<Ast *> &target,
				std::vector<Ast *> &prerequisite;
				const std::vector<Ast *> &recipe ) :
	target( target ), preReqs( prerequisite ), recipe( recipe ) {}
};


} // end namespace gormake

#endif // GORMAKE_AST_H_



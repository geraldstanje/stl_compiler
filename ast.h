#ifndef AST_H
#define AST_H

#include "node.h"
#include <unordered_map>
#include <string>

class AST {
public:
	std::unordered_map<std::string, BooleanExpression*> defineDecl;
	
public:
	BooleanExpression *root;
	AST();
	void setAST(BooleanExpression *root_);
	void addDefine(const char *name, BooleanExpression *b);
	void print(BooleanExpression *nd, std::ostream &os);	
	bool generateDotFormat(const std::string outputfile);
};

#endif

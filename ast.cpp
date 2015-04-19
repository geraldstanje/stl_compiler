#include "ast.h"

#include <iostream>

static std::string intToString(int num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

AST::AST() {}

void AST::setAST(BooleanExpression *root_) {
	root = root_;
}

void AST::addDefine(const char *name, BooleanExpression *b) {
	std::cout << b->nodeId << ", " << name << '\n';
	return;
	
	auto it = defineDecl.find(name);
	if (it == defineDecl.end()) {
		defineDecl.insert(std::make_pair(name, b));
	} 
	else {
		std::cout << "errror....." << '\n';
	}
	
	std::cout << "addDefine called.....: " << name << '\n';
}

void AST::print(BooleanExpression *nd, std::ostream &os) {
	if (!nd) {
		return;
	}
						
	for (auto e = nd->children.begin(); e != nd->children.end(); e++) {
		os << "\t" + intToString(nd->nodeId) + " -> " + intToString((*e)->nodeId) + "\n";
			
		print(*e, os);
	}
		
	os << "\t" + intToString(nd->nodeId) + " [label=\"" + nd->name() + "\"];" + "\n";
}		

bool AST::generateDotFormat(const std::string outputfile) {
	std::ofstream file;
	file.open(outputfile.c_str(), std::ios::out);
	if (!file.is_open()) {
		return false;
	}
		
	file << "digraph G {" << "\n";
	print(root, file);
	file << "}";
	file.close();
	return true;
}
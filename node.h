#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

static int count = 0;

class BooleanExpression {
public:
	int nodeId;
	std::vector<BooleanExpression*> children;
	BooleanExpression(BooleanExpression *l, BooleanExpression *r): nodeId(count++) {
		if (l) {
			children.push_back(l);
		}
		if (r) {
			children.push_back(r);
		}
	}	
	virtual ~BooleanExpression() {}
	virtual std::string name() = 0;
	//virtual llvm::Value* codeGen(CodeGenContext& context) { return NULL; }
};

class NAlways : public BooleanExpression {
public:
	int min_time;
	int max_time;
	NAlways(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}
	std::string name() { 
		return "NAlways"; 
	}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAnd : public BooleanExpression {
public:
	NAnd(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}
	std::string name() { 
		return "NAnd"; 
	}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NEventually : public BooleanExpression {
public:
	int min_time;
	int max_time;
	NEventually(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}
	std::string name() { 
		return "NEventually"; 
	}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NPredicate : public BooleanExpression {
public:
	std::string variable;
	std::string op;
	float condition;
	NPredicate(BooleanExpression *l_, BooleanExpression *r_, const char *variable_): BooleanExpression(l_, r_), variable(variable_) {}
	std::string name() { 
		return std::string("NPredicate") +  std::string(" ") + variable;
	}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NImply : public BooleanExpression {
public:
	int min_time;
	int max_time;
	NImply(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}
	std::string name() { 
		return "NImply"; 
	}
	//virtual llvm::Value* codeGen(CodeGenContext& context);
};

class AST {
public:
	BooleanExpression *root;
	AST() {}
	std::string intToString(int num) {
		std::stringstream ss;
		ss << num;
		return ss.str();
	}
	void setAST(BooleanExpression *root_) {
		root = root_;
	}
	void print(BooleanExpression *nd, std::ostream &os) {
		if (!nd) {
			return;
		}
						
		for (auto e = nd->children.begin(); e != nd->children.end(); e++) {
			os << "\t" + intToString(nd->nodeId) + " -> " + intToString((*e)->nodeId) + "\n";
			
			print(*e, os);
		}
		
		os << "\t" + intToString(nd->nodeId) + " [label=\"" + nd->name() + "\"];" + "\n";
	}		
	bool generateDotFormat(const std::string outputfile) {
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
};

#endif
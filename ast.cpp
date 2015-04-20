#include "ast.h"

#include <cstdlib>     /* exit, EXIT_FAILURE */
#include <iostream>

static std::string intToString(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

AST::AST() {}

void AST::setAST(Node *root_) {
    root = root_;
}

void AST::addDefine(const char *name, Node *b) {
    auto it = defineDecl.find(name);
	
    if (it == defineDecl.end()) {
        defineDecl.insert(std::make_pair(name, b));
    } else {
        std::cerr << "error: duplicated definition" << '\n';
		std::exit(1);
    }
}

Node* AST::getNode(const std::string name) {
    auto it = defineDecl.find(name);
    if (it != defineDecl.end()) {
        return it->second;
    }
    return NULL;
}

void AST::print(Node *nd, std::ostream &os) {
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
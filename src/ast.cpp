#include "ast.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

AST::AST() {}

void AST::setAST(Node *root_) {
    root = root_;
}

void AST::addDefine(const char *name, Node *b) {
    auto it = defineDecl.find(name);
    if (it == defineDecl.end()) {
        defineDecl.insert(std::make_pair(name, b));
    } else {
        std::cerr << "error: duplicated definition for " << name << '\n';
        std::exit(1);
    }
}

Node* AST::getNode(const std::string &name) {
    auto it = defineDecl.find(name);
    if (it != defineDecl.end()) {
        return it->second;
    }
    std::cerr << "error: define " << name << " is undeclared" << '\n';
    std::exit(1);
}

void AST::print(Node *nd, std::ostream &os) {
    nd->print(os, this);
}

void AST::optimize() {
    root->optimize(this);
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

void AST::setPrevNode(Node *curr, Node *prev) {
    if (!curr) {
        return;
    }

    auto it = prevNodeMap.find(curr);
    if (it == prevNodeMap.end()) {
        prevNodeMap.insert(std::make_pair(curr, prev));
    } else {
        std::cerr << "error: prev Node already exists in prevNodeMap" << '\n';
        std::exit(1);
    }
}

Node* AST::getPrevNode(Node *curr) {
    if (!curr) {
        return NULL;
    }

    auto it = prevNodeMap.find(curr);
    if (it != prevNodeMap.end()) {
        return it->second;
    }
    return NULL;
}

void AST::removePrevNode(Node *curr) {
    auto it = prevNodeMap.find(curr);
    if (it != prevNodeMap.end()) {
        prevNodeMap.erase(it);
    } else {
        std::cerr << "error: prev Node doesnt exist in prevNodeMap" << '\n';
        std::exit(1);
    }
}

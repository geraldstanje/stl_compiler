#ifndef AST_H
#define AST_H

#include "node.h"
#include <unordered_map>
#include <string>

class Node;

class AST {
public:
    std::unordered_map<Node*, Node*> prevNodeMap;
    std::unordered_map<std::string, Node*> defineDecl;

public:
    Node *root;
    AST();
    void setAST(Node *root_);
    void addDefine(const char *name, Node *b);
    Node* getNode(const std::string &name);

    void setPrevNode(Node *curr, Node *prev);
    void removePrevNode(Node *curr);
    Node* getPrevNode(Node *curr);

    void print(Node *nd, std::ostream &os);
    bool generateDotFormat(const std::string outputfile);
    void optimize();
};

#endif

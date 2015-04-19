#ifndef AST_H
#define AST_H

#include "node.h"
#include <unordered_map>
#include <string>

class AST {
  public:
    std::unordered_map<std::string, Node*> defineDecl;

  public:
    Node *root;
    AST();
    void setAST(Node *root_);
    void addDefine(const char *name, Node *b);
    Node* getNode(const std::string name);
    void print(Node *nd, std::ostream &os);
    bool generateDotFormat(const std::string outputfile);
};

#endif

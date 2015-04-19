#ifndef CODEGEN_H
#define CODEGEN_H

#include "node.h"
#include "ast.h"

class CodeGenerator {
  public:
    AST *ast;

    CodeGenerator();
    void execute(AST *ast_);
    void execute(Node *nd);
};

#endif

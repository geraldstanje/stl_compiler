#ifndef CODEGEN_H
#define CODEGEN_H

#include "node.h"
#include "ast.h"

class Node;
class AST;

class CodeGenerator {
  public:
    std::ostream *os;
    AST *ast;

    CodeGenerator(AST *ast_);
    void generateCode(std::ostream *os_);
};

#endif

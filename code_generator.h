#ifndef CODEGEN_H
#define CODEGEN_H

#include "node.h"
#include "ast.h"

class Node;
class AST;

class CodeGenerator {
  public:
    int indentNum;    
    std::ostream *os;
    AST *ast;

    CodeGenerator(AST *ast_);

    void indent();
    void deindent();
    void emitLine(const std::string line);
    void generateCode(std::ostream *os_);
};

#endif

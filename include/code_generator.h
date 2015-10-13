#ifndef CODEGEN_H
#define CODEGEN_H

#include "node.h"
#include "ast.h"
#include <fstream>
#include <unordered_map>
#include <string>

class Node;
class AST;

class CodeGenerator {
public:
    std::ostream *os;
    AST *ast;
    int indentPos;
    std::unordered_map<std::string, int> channelMap;

public:
    CodeGenerator(AST *ast_);

    void indent();
    void deindent();
    void emitLine(const std::string &line);
    void generateCode(std::ostream *os_);
    bool loadChanMapXML(const std::string &filename);
    int getChannel(const std::string &variable);

    void writeBoolProp(const std::string &filename);
};

#endif

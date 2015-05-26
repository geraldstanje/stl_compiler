#include "code_generator.h"
#include <iostream>

CodeGenerator::CodeGenerator(AST *ast_): ast(ast_),
    indentNum(0) {}

/*
node *n1 = node_analog_operator_new(NULL, NULL, abs_t, "x");
node *n2 = node_predicate_new(n1, NULL, smaller_t, "x", 1.0);
node *n3 = node_temp_operator_new(n2, NULL, always_t, 0, 300);
node *n4 = node_temp_operator_new(n3, NULL, eventually_t, 0, 1100);
node *n5 = node_event_new(NULL, NULL, rise_t, "trigger", 10);
node *n6 = node_boolean_operator_new(n5, n4, imply_t);
node *n7 = node_analog_operator_new(NULL, NULL, abs_t, "x");
node *n8 = node_predicate_new(n7, NULL, smaller_t, "x", 6.0);
node *n9 = node_boolean_operator_new(n8, n6, and_t);
t->root = node_temp_operator_new(n9, NULL, always_t, 0, INT_MAX);
*/

void CodeGenerator::indent() {
    indentNum++;
}

void CodeGenerator::deindent() {
    indentNum--;

    if (indentNum < 0) {
        indentNum = 0;
    }
}

void CodeGenerator::emitLine(const std::string line) {
    *os << std::string(indentNum, '\t');
    *os << line;
    *os << "\n";
}

void CodeGenerator::generateCode(std::ostream *os_) {
    os = os_;

    indent();
    ast->root->codeGen(*this);
    emitLine(std::string("t->root = n") + ast->root->id() + std::string(";"));
    deindent();
}

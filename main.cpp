#include "ast.h"
#include "code_generator.h"
#include "y.tab.hpp"
#include <iostream>
#include <fstream>

extern "C" {
    int yyparse();
}
int yylex();
extern FILE *yyin;
extern AST ast;

int main() {
    yyin = fopen("property.txt", "r+");
    yyparse();
    fclose(yyin);

    ast.generateDotFormat("plot.dot");

    //std::fstream file;
    //file.open("generated.txt", std::ios::out);

    CodeGenerator generator(&ast);
    generator.generateCode(&std::cout);

    //file.close();

    return 0;
}

#include "ast.h"
#include "code_generator.h"
#include "y.tab.hpp"
#include <iostream>
#include <fstream>

int yyparse();
int yylex();
extern FILE *yyin;
extern AST ast;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[1], "r+");
    yyparse();
    fclose(yyin);

    ast.optimize();
    ast.generateDotFormat("plot.dot");

    std::fstream file;
    file.open("generated/generated_stl.h", std::ios::out);

    CodeGenerator generator(&ast);
    if (!generator.loadChanMapXML("config/mapping.xml")) {
        exit(1);
    }
    generator.generateCode(&file);

    file.close();

    generator.writeBoolProp("generated/bool_prop.xml");

    return 0;
}

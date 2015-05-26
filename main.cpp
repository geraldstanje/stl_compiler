#include "ast.h"
#include "code_generator.h"
#include "y.tab.hpp"
#include <iostream>
#include <fstream>

int yyparse();
int yylex();
extern FILE *yyin;
extern AST ast;

int main() {
    yyin = fopen("property.txt", "r+");
    yyparse();
    fclose(yyin);

    ast.generateDotFormat("plot.dot");

    std::fstream file;
    file.open("../sw_monitor/generated/stl_gen.h", std::ios::out);

    CodeGenerator generator(&ast);

    file << "#include \"../node.h\"" << '\n';
    file << "#include \"../stltree.h\"" << "\n\n";

    file << "void stltree_create(stltree *t) {" << '\n';
    generator.generateCode(&file);//&std::cout);
    file << "}";

    file.close();

    return 0;
}

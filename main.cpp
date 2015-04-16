#include "node.h"
#include "code_generator.h"
#include "y.tab.hpp"

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
	
	CodeGenerator generator;
	generator.execute(ast.root);
	
	return 0;
}

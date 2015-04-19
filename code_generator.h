#ifndef CODEGEN_H
#define CODEGEN_H

#include "node.h"

class CodeGenerator {
public:
    CodeGenerator();
	void execute(BooleanExpression *nd);
};

#endif

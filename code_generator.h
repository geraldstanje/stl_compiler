#ifndef CODEGEN_H
#define CODEGEN_H

#include "node.h"
#include <iostream>

class CodeGenerator {
  public:
    CodeGenerator() {}
    ~CodeGenerator() {}

    void execute(BooleanExpression *nd) {
        if (!nd) {
            return;
        }

        for (auto e = nd->children.begin(); e != nd->children.end(); e++) {
            execute(*e);
        }

        std::cout << nd->name() << '\n';
    }
};

#endif

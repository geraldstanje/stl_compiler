#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "code_generator.h"
#include "AST.h"

class CodeGenerator;

class Bound {
  public:
    double min;
    double max;
    Bound(double min_, double max_): min(min_), max(max_) {}
};

class Node {
  public:
    int nodeId;
    std::string name;
    Node(const std::string name_);
    virtual ~Node();
    std::string id();
    bool isBoolAtomChild();
    std::string getChildParams(CodeGenerator &c, Node *lhs, Node *rhs);
    virtual void print(std::ostream &os, AST *ast) = 0;
    virtual void codeGen(CodeGenerator &c) = 0;
};

class BooleanExpression : public Node {
  public:
    BooleanExpression(const std::string name);
    virtual void print(std::ostream &os, AST *ast) = 0;
    virtual void codeGen(CodeGenerator &c) = 0;
};

class AnalogExpression : public Node {
  public:
    AnalogExpression(const std::string name);
    virtual void print(std::ostream &os, AST *ast) = 0;
    virtual void codeGen(CodeGenerator &c) = 0;
};

class NAlways : public BooleanExpression {
  public:
    BooleanExpression *lhs;
    BooleanExpression *rhs;
    Bound *b;
    NAlways(BooleanExpression *l, BooleanExpression *r, Bound *b);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NAnd : public BooleanExpression {
  public:
    BooleanExpression *lhs;
    BooleanExpression *rhs;
    NAnd(BooleanExpression *l, BooleanExpression *r);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NEventually : public BooleanExpression {
  public:
    BooleanExpression *lhs;
    BooleanExpression *rhs;
    Bound *b;
    NEventually(BooleanExpression *l, BooleanExpression *r, Bound *b);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NPredicate : public BooleanExpression {
  public:
    AnalogExpression *lhs;
    std::string variable;
    std::string op;
    double condition;
    NPredicate(AnalogExpression *l, const std::string op, double condition);
    void setVariable(AnalogExpression *l);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NImply : public BooleanExpression {
  public:
    BooleanExpression *lhs;
    BooleanExpression *rhs;
    int min_time;
    int max_time;
    NImply(BooleanExpression *l, BooleanExpression *r);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NBoolAtom : public BooleanExpression {
  public:
    std::string variable;
    NBoolAtom(const char *variable);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NEvent : public BooleanExpression {
  public:
    BooleanExpression *lhs;
    BooleanExpression *rhs;
    NEvent(BooleanExpression *l, BooleanExpression *r);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

class NAnalog : public AnalogExpression {
  public:
    std::string op;
    std::string variable;
    NAnalog(const char *variable);
    void setOperator(const std::string op_);
    virtual void print(std::ostream &os, AST *ast);
    virtual void codeGen(CodeGenerator &c);
};

#endif
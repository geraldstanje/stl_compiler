#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

class Bound {
  public:
    double min;
    double max;
    Bound(double min_, double max_): min(min_), max(max_) {}
};

class Node {
  public:
    int nodeId;
    std::vector<Node*> children;
    Node(Node *l, Node *r);
    virtual ~Node();
    std::string id();
    std::string childParams();
    virtual bool isBoolAtom() = 0;
    virtual std::string name() = 0;
    virtual std::string codeGen() = 0;
};

class BooleanExpression : public Node {
  public:
    BooleanExpression(BooleanExpression *l, BooleanExpression *r);
};

class AnalogExpression : public Node {
  public:
    AnalogExpression(AnalogExpression *l, AnalogExpression *r);
};

class NAlways : public BooleanExpression {
  public:
    Bound *b;
    NAlways(BooleanExpression *l_, BooleanExpression *r_, Bound *b_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NAnd : public BooleanExpression {
  public:
    NAnd(BooleanExpression *l_, BooleanExpression *r_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NEventually : public BooleanExpression {
  public:
    Bound *b;
    NEventually(BooleanExpression *l_, BooleanExpression *r_, Bound *b_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NPredicate : public BooleanExpression {
  public:
	std::string variable;
    std::string op;
    double condition;
    NPredicate(BooleanExpression *l_, BooleanExpression *r_, const std::string op_, double condition_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NImply : public BooleanExpression {
  public:
    int min_time;
    int max_time;
    NImply(BooleanExpression *l_, BooleanExpression *r_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NBoolAtom : public BooleanExpression {
  public:
    std::string variable;
    NBoolAtom(BooleanExpression *l_, BooleanExpression *r_, const char *variable_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NEvent : public BooleanExpression {
  public:
    NEvent(BooleanExpression *l_, BooleanExpression *r_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
};

class NAnalog : public AnalogExpression {
  public:
	std::string op;
    std::string variable;
    NAnalog(AnalogExpression *l_, AnalogExpression *r_, const char *variable_);
    virtual bool isBoolAtom();
    virtual std::string name();
    virtual std::string codeGen();
	void setOperator(const std::string op_);
};

#endif
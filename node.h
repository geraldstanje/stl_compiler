#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "code_generator.h"
#include "ast.h"

class CodeGenerator;

enum class operator_type {
    anal_t,
    pred_t,
    bool_t,
    futu_t,
    past_t,
    even_t,
    tpro_t,
    boolatom_t
};

const std::string operator_type_strings[] = {
    "analog_operator",
    "predicate",
    "boolean_operator",
    "future_tmp_operator",
    "past_tmp_operator",
    "event",
    "temp_property",
    "boolatom_t"
};

enum class operator_subtype {
    /* analog operators */
    subtract_t,
    add_t,
    multiply_t,
    abs_t,
    /* predicates */
    small_equal_t,
    smaller_t,
    larger_equal_t,
    larger_t,
    equal_t,
    /* boolean operators */
    not_t,
    or_t,
    and_t,
    imply_t,
    dual_imply_t, /* TODO: check if correct naming */
    xor_t,
    /* future temporal operators */
    eventually_t,
    always_t,
    until_t,
    /* past temporal operators */
    once_t,
    historyically_t,
    since_t,
    /* events */
    rise_t,
    fall_t,
    /* template properties */
    distance_t,
    none_t
};

const std::string operator_subtype_strings[] = {
    /* analog operators */
    "subtract_t",
    "add_t",
    "multiply_t",
    "abs_t",
    /* predicates */
    "small_equal_t",
    "smaller_t",
    "larger_equal_t",
    "larger_t",
    "equal_t",
    /* boolean operators */
    "not_t",
    "or_t",
    "and_t",
    "imply_t",
    "dual_imply_t", /* TODO: check if correct naming */
    "xor_t",
    /* future temporal operators */
    "eventually_t",
    "always_t",
    "until_t",
    /* past temporal operators */
    "once_t",
    "historyically_t",
    "since_t",
    /* events */
    "rise_t",
    "fall_t",
    /* template properties */
    "distance_t",
    "none_t"
};

constexpr std::size_t enumIndex(const operator_type value) noexcept;
constexpr std::size_t subTypeIndex(const operator_subtype value) noexcept;

class Bound {
  public:
    double min;
    double max;
    Bound(double min_, double max_): min(min_), max(max_) {}
};

class Node {
  public:
    int nodeId;
    operator_type type;
    operator_subtype subtype;
    Node(operator_type type_, operator_subtype subtype_);
    virtual ~Node();
    std::string id();
    bool isBoolAtom();
    std::string getChildParams(CodeGenerator &c, Node *lhs, Node *rhs);
    virtual void print(std::ostream &os, AST *ast) = 0;
    virtual void codeGen(CodeGenerator &c) = 0;
};

class BooleanExpression : public Node {
  public:
    BooleanExpression(operator_type type_, operator_subtype subtype_);
    virtual void print(std::ostream &os, AST *ast) = 0;
    virtual void codeGen(CodeGenerator &c) = 0;
};

class AnalogExpression : public Node {
  public:
    AnalogExpression(operator_type type_, operator_subtype subtype_);
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
    BooleanExpression *lhs; // TODO: we only need lhs!?
    BooleanExpression *rhs;
    NEvent(BooleanExpression *l, BooleanExpression *r, const operator_subtype subtype);
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

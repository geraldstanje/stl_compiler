#include "node.h"

static int count = 0;

static std::string intToString(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

static std::string doubleToString(double num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

Node::Node(Node *l, Node *r): nodeId(count++) {
    if (l) {
        children.push_back(l);
    }
    if (r) {
        children.push_back(r);
    }
}

Node::~Node() {}

std::string Node::id() {
    return intToString(nodeId);
}

std::string Node::childParams() {
    if (children.size() > 1) {
        return std::string("n") + children[0]->id() + std::string(", ") + std::string("node") + children[1]->id();
    } else if (children.size() > 0) {
        return std::string("n") + children[0]->id() + std::string(", ") + std::string("NULL");
    }
    return std::string("NULL, NULL");
}

BooleanExpression::BooleanExpression(BooleanExpression *l, BooleanExpression *r): Node(l, r) {}

AnalogExpression::AnalogExpression(AnalogExpression *l, AnalogExpression *r): Node(l, r) {}

NAlways::NAlways(BooleanExpression *l_, BooleanExpression *r_, Bound *b_): BooleanExpression(l_, r_), b(b_) {}

bool NAlways::isBoolAtom() {
    return false;
}

std::string NAlways::name() {
    return "NAlways";
}

std::string NAlways::codeGen() {
    std::string timeBound;

    if (b) {
        timeBound = intToString(b->min) + std::string(", ") + intToString(b->max);
    } else {
        timeBound = std::string("0, INT_MAX");
    }

    return "node *n" + id() + std::string(" = ") + std::string("node_temp_operator_new") + "(" + childParams() + std::string(", ") + std::string("always_t") +
           std::string(", ") + timeBound + std::string(");");
}

NAnd::NAnd(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}

bool NAnd::isBoolAtom() {
    return false;
}

std::string NAnd::name() {
    return "NAnd";
}

std::string NAnd::codeGen() {
    return "node *n" + id() + std::string(" = ") + std::string("node_boolean_operator_new") +
           std::string("(") + childParams() + std::string(", ") + std::string("and_t") + std::string(");");
}

NEventually::NEventually(BooleanExpression *l_, BooleanExpression *r_, Bound *b_): BooleanExpression(l_, r_), b(b_) {}

bool NEventually::isBoolAtom() {
    return false;
}

std::string NEventually::name() {
    return "NEventually";
}

std::string NEventually::codeGen() {
    std::string timeBound;

    if (b) {
        timeBound = intToString(b->min) + std::string(", ") + intToString(b->max);
    } else {
        timeBound = std::string("0, INT_MAX");
    }

    return "node *n" + id() + std::string(" = ") + std::string("node_temp_operator_new") +
           std::string("(") + childParams() + std::string(", ") + std::string("eventually_t") + std::string(", ") + timeBound + std::string(");");
}

NPredicate::NPredicate(BooleanExpression *l_, BooleanExpression *r_, const std::string op_, double condition_): BooleanExpression(l_, r_), op(op_), condition(condition_) {
	if (l_) {
		variable = (reinterpret_cast<NAnalog*>(l_))->variable;
	}
}

bool NPredicate::isBoolAtom() {
    return false;
}

std::string NPredicate::name() {
    return std::string("NPredicate");
}

std::string NPredicate::codeGen() {
    return "node *n" + id() + std::string(" = ") + std::string("node_predicate_new") +
           "(" + childParams() + std::string(", ") + op + std::string(", ") + std::string("\"") + variable + std::string("\"") + std::string(", ") + 
		   doubleToString(condition) + std::string(");");
}

NImply::NImply(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}

bool NImply::isBoolAtom() {
    return false;
}

std::string NImply::name() {
    return "NImply";
}

std::string NImply::codeGen() {
    return "node *n" + id() + std::string(" = ") + std::string("node_boolean_operator_new") +
           std::string("(") + childParams() + std::string(", ") + std::string("imply_t") + std::string(");");
}

NBoolAtom::NBoolAtom(BooleanExpression *l_, BooleanExpression *r_, const char *variable_): BooleanExpression(l_, r_), variable(variable_) {}

bool NBoolAtom::isBoolAtom() {
    return true;
}

std::string NBoolAtom::name() {
    return variable;
}

std::string NBoolAtom::codeGen() {
    return "node *n" + id() + std::string(" = ") + std::string("NBoolAtom") +
           "(" + childParams() + std::string(", ") + variable;
}

NEvent::NEvent(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}

bool NEvent::isBoolAtom() {
	return false;
}
	
std::string NEvent::name() {
    return "NEvent";
}

std::string NEvent::codeGen() {
    return "node *n" + id() + std::string(" = ") + std::string("node_event_new") +
           std::string("(") + childParams() + std::string(", ") + std::string("rise_t") + std::string(");");
}

NAnalog::NAnalog(AnalogExpression *l, AnalogExpression *r, const char *variable_): AnalogExpression(l, r), variable(variable_) {}

bool NAnalog::isBoolAtom() {
    return false;
}

std::string NAnalog::name() {
    return variable;
}

std::string NAnalog::codeGen() {
    return "node *n" + id() + std::string(" = ") + std::string("node_analog_operator_new") +
           std::string("(") + childParams() + std::string(", ") + op + std::string(", ") + std::string("\"") + variable + std::string("\"") + std::string(");");
}

void NAnalog::setOperator(const std::string op_) {
	op = op_;
}

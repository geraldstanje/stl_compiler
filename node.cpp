#include "node.h"

static int count = 0;

static std::string intToString(int num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

BooleanExpression::BooleanExpression(BooleanExpression *l, BooleanExpression *r): nodeId(count++) {
	if (l) {
		children.push_back(l);
	}
	if (r) {
		children.push_back(r);
	}
}

BooleanExpression::~BooleanExpression() {}

std::string BooleanExpression::id() {
	return intToString(nodeId);
}

std::string BooleanExpression::childParams() {
	if (children.size() > 1) {
		return std::string("node") + children[0]->id() + std::string(", ") + std::string("node") + children[1]->id();
	}
	else if (children.size() > 0) {
		return std::string("node") + children[0]->id() + std::string(", ") + std::string("NULL");
	}
	return std::string("NULL, NULL");
}

NAlways::NAlways(BooleanExpression *l_, BooleanExpression *r_, Bound *b_): BooleanExpression(l_, r_), b(b_) {}

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

std::string NAnd::name() { 
	return "NAnd"; 
}

std::string NAnd::codeGen() {
	return "node *n" + id() + std::string(" = ") + std::string("node_boolean_operator_new") + 
	       std::string("(") + childParams() + std::string(", ") + std::string("and_t") + std::string(");");
}

NEventually::NEventually(BooleanExpression *l_, BooleanExpression *r_, Bound *b_): BooleanExpression(l_, r_), b(b_) {}

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

NPredicate::NPredicate(BooleanExpression *l_, BooleanExpression *r_, const char *variable_): BooleanExpression(l_, r_), variable(variable_) {}

std::string NPredicate::name() { 
	return std::string("NPredicate") +  std::string(" ") + std::string("=") + variable;
}

std::string NPredicate::codeGen() {
	return "node *n" + id() + std::string(" = ") + std::string("node_predicate_new") + 
	       "(" + childParams() + std::string(", ") + variable;
}

NImply::NImply(BooleanExpression *l_, BooleanExpression *r_): BooleanExpression(l_, r_) {}

std::string NImply::name() { 
	return "NImply"; 
}

std::string NImply::codeGen() {
	return "node *n" + id() + std::string(" = ") + std::string("node_boolean_operator_new") + 
	       std::string("(") + childParams() + std::string(", ") + std::string("imply_t") + std::string(");");
}

NAnalog::NAnalog(BooleanExpression *l_, BooleanExpression *r_, const char *variable_): BooleanExpression(l_, r_), variable(variable_) {}

std::string NAnalog::name() { 
	return ""; 
}

std::string NAnalog::codeGen() {
	return "";
}

NBoolAtom::NBoolAtom(BooleanExpression *l_, BooleanExpression *r_, const char *variable_): BooleanExpression(l_, r_), variable(variable_) {}

std::string NBoolAtom::name() { 
	return std::string("NBoolAtom") +  std::string(" ") + std::string("=") + variable;
}

std::string NBoolAtom::codeGen() {
	return "node *n" + id() + std::string(" = ") + std::string("node_predicate_new") + 
	       "(" + childParams() + std::string(", ") + variable;
}

AnalogExpression::AnalogExpression(const char *name_): name(name_) {}
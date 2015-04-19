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

class BooleanExpression {
public:
	int nodeId;
	std::vector<BooleanExpression*> children;
	BooleanExpression(BooleanExpression *l, BooleanExpression *r);
	virtual ~BooleanExpression();
	std::string id();
	std::string childParams();
	virtual std::string name() = 0;
	virtual std::string codeGen() = 0;
};

class NAlways : public BooleanExpression {
public:
	Bound *b;
	NAlways(BooleanExpression *l_, BooleanExpression *r_, Bound *b_);
	virtual std::string name();
	virtual std::string codeGen();
};

class NAnd : public BooleanExpression {
public:
	NAnd(BooleanExpression *l_, BooleanExpression *r_);
	virtual std::string name();
	virtual std::string codeGen();
};

class NEventually : public BooleanExpression {
public:
	Bound *b;
	NEventually(BooleanExpression *l_, BooleanExpression *r_, Bound *b_);
	virtual std::string name();
	virtual std::string codeGen();
};

class NPredicate : public BooleanExpression {
public:
	std::string variable;
	std::string op;
	float condition;
	NPredicate(BooleanExpression *l_, BooleanExpression *r_, const char *variable_);
	virtual std::string name();
	virtual std::string codeGen();
};

class NImply : public BooleanExpression {
public:
	int min_time;
	int max_time;
	NImply(BooleanExpression *l_, BooleanExpression *r_);
	virtual std::string name();
	virtual std::string codeGen();
};

class NAnalog : public BooleanExpression {
public:
	std::string variable;
	NAnalog(BooleanExpression *l_, BooleanExpression *r_, const char *variable_);
	virtual std::string name();
	virtual std::string codeGen();
};

class NBoolAtom : public BooleanExpression {
public:
	std::string variable;
	NBoolAtom(BooleanExpression *l_, BooleanExpression *r_, const char *variable_);
	virtual std::string name();
	virtual std::string codeGen();
};

class AnalogExpression {
public:
	std::string name;
	AnalogExpression(const char *name_);
};

#endif
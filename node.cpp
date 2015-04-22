#include "node.h"
#include <cassert>

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

Node::Node(std::string name_): nodeId(count++), name(name_) {}

Node::~Node() {}

std::string Node::id() {
    return intToString(nodeId);
}

bool Node::isBoolAtomChild() {
    if (name == "NBoolAtom") {
        return true;
    }
    return false;
}

std::string Node::getChildParams(CodeGenerator &c, Node *lhs, Node *rhs) {
    std::string childParams;

    if (lhs) {
        std::string lhs_id;

        if (lhs->isBoolAtomChild()) {
            Node *lhs_ = c.ast->getNode((static_cast<NBoolAtom*>(lhs))->variable);
            assert(lhs_);
            lhs_id = lhs_->id();
        } else {
            lhs_id = lhs->id();
        }

        childParams = std::string("n") + lhs_id + std::string(", ");
    } else {
        childParams = std::string("NULL, ");
    }

    if (rhs) {
        std::string rhs_id;

        if (rhs->isBoolAtomChild()) {
            Node *rhs_ = c.ast->getNode((static_cast<NBoolAtom*>(rhs))->variable);
            assert(rhs_);
            rhs_id = rhs_->id();
        } else {
            rhs_id = rhs->id();
        }

        childParams += std::string("n") + rhs_id + std::string(", ");
    } else {
        childParams += std::string("NULL, ");
    }

    return childParams;
}

BooleanExpression::BooleanExpression(const std::string name): Node(name) {}

AnalogExpression::AnalogExpression(const std::string name): Node(name) {}

NAlways::NAlways(BooleanExpression *l, BooleanExpression *r, Bound *b): BooleanExpression("NAlways"), lhs(l), rhs(r), b(b) {}

void NAlways::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NAlways::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string timeBound;
    if (b) {
        timeBound = intToString(b->min) + std::string(", ") + intToString(b->max);
    } else {
        timeBound = std::string("0, INT_MAX");
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_temp_operator_new") + std::string("(") + childParams + std::string("always_t") +
          std::string(", ") + timeBound + std::string(");");
    *c.os << "\n";
}

NAnd::NAnd(BooleanExpression *l, BooleanExpression *r): BooleanExpression("NAnd"), lhs(l), rhs(r) {}

void NAnd::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NAnd::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_boolean_operator_new") +
          std::string("(") + childParams + std::string("and_t") + std::string(");");
    *c.os << "\n";
}

NEventually::NEventually(BooleanExpression *l, BooleanExpression *r, Bound *b): BooleanExpression("NEventually"), lhs(l), rhs(r), b(b) {}

void NEventually::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NEventually::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string timeBound;
    if (b) {
        timeBound = intToString(b->min) + std::string(", ") + intToString(b->max);
    } else {
        timeBound = std::string("0, INT_MAX");
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_temp_operator_new") +
          std::string("(") + childParams + std::string("eventually_t") + std::string(", ") + timeBound + std::string(");");
    *c.os << "\n";
}

NPredicate::NPredicate(AnalogExpression *l, const std::string op, double condition): BooleanExpression("NPredicate"), lhs(l), op(op), condition(condition) {}

void NPredicate::setVariable(AnalogExpression *l) {
    if (l) {
        variable = (static_cast<NAnalog*>(l))->variable;
    }
}

void NPredicate::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NPredicate::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, NULL);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_predicate_new") +
          std::string("(") + childParams + op + std::string(", ") + std::string("\"") + variable + std::string("\"") + std::string(", ") +
          doubleToString(condition) + std::string(");");
    *c.os << "\n";
}

NImply::NImply(BooleanExpression *l, BooleanExpression *r): BooleanExpression("NImply"), lhs(l), rhs(r) {}

void NImply::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NImply::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_boolean_operator_new") +
          std::string("(") + childParams + std::string("imply_t") + std::string(");");
    *c.os << "\n";
}

NBoolAtom::NBoolAtom(const char *variable): BooleanExpression("NBoolAtom"), variable(variable) {}

void NBoolAtom::print(std::ostream &os, AST *ast) {
    Node *lhs = ast->getNode(variable);
    if (lhs) {
        //os << "\t" + intToString(nodeId+100) + " -> " + intToString(lhs->nodeId) + "\n";
        //os << "\t" + intToString(nodeId+100) + " [label=\"" + "define: " + variable + "\"];" + "\n";

        lhs->print(os, ast);
    }
    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NBoolAtom::codeGen(CodeGenerator &c) {
    Node *lhs = c.ast->getNode(variable);
    if (lhs) {
        lhs->codeGen(c);
    }
}

NEvent::NEvent(BooleanExpression *l, BooleanExpression *r): BooleanExpression("NEvent"), lhs(l), rhs(r) {}

void NEvent::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NEvent::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_event_new") +
          std::string("(") + childParams + std::string("rise_t") + std::string(");");
    *c.os << "\n";
}

NAnalog::NAnalog(const char *variable): AnalogExpression("NAnalog"), variable(variable) {}

void NAnalog::print(std::ostream &os, AST *ast) {
    os << "\t" + intToString(nodeId) + " [label=\"" + name + "\"];" + "\n";
}

void NAnalog::codeGen(CodeGenerator &c) {
    std::string childParams = getChildParams(c, NULL, NULL);

    *c.os << std::string("node *n") + id() + std::string(" = ") + std::string("node_analog_operator_new") +
          std::string("(") + childParams + op + std::string(", ") + std::string("\"") + variable + std::string("\"") + std::string(");");
    *c.os << "\n";
}

void NAnalog::setOperator(const std::string op_) {
    op = op_;
}

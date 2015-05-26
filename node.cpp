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

constexpr std::size_t enumIndex(const operator_type value) noexcept {
    static_assert(std::is_enum<operator_type>::value, "Not an enum of type operator_type");
    return static_cast<std::size_t>(value);
}

constexpr std::size_t subTypeIndex(const operator_subtype value) noexcept {
    static_assert(std::is_enum<operator_subtype>::value, "Not an enum of type operator_subtype");
    return static_cast<std::size_t>(value);
}

Node::Node(operator_type type_, operator_subtype subtype_): nodeId(count++), 
                                                            type(type_), 
                                                            subtype(subtype_) {}

Node::~Node() {}

std::string Node::id() {
    return intToString(nodeId);
}

bool Node::isBoolAtom() {
    if (type == operator_type::boolatom_t) {
        return true;
    }
    return false;
}

std::string Node::getChildParams(CodeGenerator &c, Node *lhs, Node *rhs) {
    std::string childParams;

    if (lhs) {
        std::string lhs_id;

        if (lhs->isBoolAtom()) {
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

        if (rhs->isBoolAtom()) {
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

BooleanExpression::BooleanExpression(operator_type type_, operator_subtype subtype_): Node(type_, subtype_) {}

AnalogExpression::AnalogExpression(operator_type type_, operator_subtype subtype_): Node(type_, subtype_) {}

NAlways::NAlways(BooleanExpression *l, BooleanExpression *r, Bound *bd): BooleanExpression(operator_type::futu_t, operator_subtype::always_t), 
                                                                         lhs(l), 
                                                                         rhs(r), 
                                                                         b(bd) {}

void NAlways::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
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
    
    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_temp_operator_new") + std::string("(") + childParams + 
                       std::string("always_t") + std::string(", ") + timeBound + std::string(");");
    c.emitLine(line);
}

NAnd::NAnd(BooleanExpression *l, BooleanExpression *r): BooleanExpression(operator_type::anal_t, operator_subtype::and_t), 
                                                        lhs(l), 
                                                        rhs(r) {}

void NAnd::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
}

void NAnd::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_boolean_operator_new") +
                       std::string("(") + childParams + std::string("and_t") + std::string(");");
    c.emitLine(line);
}

NEventually::NEventually(BooleanExpression *l, BooleanExpression *r, Bound *bd): BooleanExpression(operator_type::futu_t, operator_subtype::eventually_t), 
                                                                                 lhs(l), 
                                                                                 rhs(r), 
                                                                                 b(bd) {}

void NEventually::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
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

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_temp_operator_new") +
                       std::string("(") + childParams + std::string("eventually_t") + std::string(", ") + timeBound + std::string(");");
    c.emitLine(line);
}

NPredicate::NPredicate(AnalogExpression *l, const std::string op, double condition): BooleanExpression(operator_type::pred_t, operator_subtype::smaller_t), 
                                                                                     lhs(l), 
                                                                                     op(op), 
                                                                                     condition(condition) {}

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

    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
}

void NPredicate::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, NULL);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_predicate_new") +
                       std::string("(") + childParams + op + std::string(", ") + std::string("\"") + variable + std::string("\"") + std::string(", ") +
                       doubleToString(condition) + std::string(");");
    c.emitLine(line);
}

NImply::NImply(BooleanExpression *l, BooleanExpression *r): BooleanExpression(operator_type::bool_t, operator_subtype::imply_t), 
                                                            lhs(l), 
                                                            rhs(r) {}

void NImply::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
}

void NImply::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_boolean_operator_new") +
                       std::string("(") + childParams + std::string("imply_t") + std::string(");");
    c.emitLine(line);
}

NBoolAtom::NBoolAtom(const char *variable): BooleanExpression(operator_type::boolatom_t, operator_subtype::none_t), 
                                            variable(variable) {}

void NBoolAtom::print(std::ostream &os, AST *ast) {
    Node *lhs = ast->getNode(variable);
    if (lhs) {
        //os << "\t" + intToString(nodeId+100) + " -> " + intToString(lhs->nodeId) + "\n";
        //os << "\t" + intToString(nodeId+100) + " [label=\"" + "define: " + variable + "\"];" + "\n";

        lhs->print(os, ast);
    }
    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
}

void NBoolAtom::codeGen(CodeGenerator &c) {
    Node *lhs = c.ast->getNode(variable);
    if (lhs) {
        lhs->codeGen(c);
    }
}

NEvent::NEvent(BooleanExpression *l, BooleanExpression *r, const operator_subtype subtype): BooleanExpression(operator_type::even_t, subtype), 
                                                                                            lhs(l), 
                                                                                            rhs(r) {}

void NEvent::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + " -> " + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
}

void NEvent::codeGen(CodeGenerator &c) {
    std::string variable;

    if (lhs) {
        if (lhs->isBoolAtom()) {
            Node *lhs_ = c.ast->getNode((static_cast<NBoolAtom*>(lhs))->variable);
            variable = std::string(", \"") + (static_cast<NPredicate*>(lhs_))->variable + std::string("\"");
            variable += std::string(", ") + doubleToString((static_cast<NPredicate*>(lhs_))->condition/2); // TODO: /2 ... check if required
        }

        //lhs->codeGen(c);
    }
    //if (rhs) {
        //rhs->codeGen(c);
    //}

    //std::string childParams = getChildParams(c, lhs, rhs);
    std::string childParams = getChildParams(c, NULL, NULL);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_event_new") +
                       std::string("(") + childParams + operator_subtype_strings[subTypeIndex(subtype)] + variable + std::string(");");
    c.emitLine(line);
}

NAnalog::NAnalog(const char *variable): AnalogExpression(operator_type::anal_t, operator_subtype::none_t), 
                                        variable(variable) {}

void NAnalog::print(std::ostream &os, AST *ast) {
    os << "\t" + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + "\n";
}

void NAnalog::codeGen(CodeGenerator &c) {
    std::string childParams = getChildParams(c, NULL, NULL);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_analog_operator_new") +
                       std::string("(") + childParams + op + std::string(", ") + std::string("\"") + variable + std::string("\"") + std::string(");");
    c.emitLine(line);
}

void NAnalog::setOperator(const std::string op_) {
    op = op_;
}

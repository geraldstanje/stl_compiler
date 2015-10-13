#include "node.h"
#include "util.h"
#include <cassert>
#include <iostream>

static int count = 0;

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

void Node::optimizeFutureOp(AST *ast, Node *start) {
    std::vector<Node*> prevNodes;
    Node *prev = NULL;
    Node *curr = start;

    // put all prev nodes into a vector called prevNodes
    while ((prev = ast->getPrevNode(curr)) != NULL) {
        if (prev->type == operator_type::futu_t) {
            prevNodes.push_back(prev);
        } else {
            break;
        }

        if (prevNodes.size() == 2) {
            break;
        }

        curr = prev;
    }

    // if we found two future operators next to each other, we can transform the AST
    // transform: NEventually -> NAlways into: NEventuallyAlways
    if (prevNodes.size() == 2) {
        Node *prev = ast->getPrevNode(prevNodes[1]);
        if (!prev || (prev->type != operator_type::bool_t)) { // TODO: check if prev->type != operator_type::bool_t can be removed
            return;
        }

        NAlways *tmp1 = static_cast<NAlways*>(prevNodes[0]);
        NEventually *tmp2 = static_cast<NEventually*>(prevNodes[1]);
        NAnd *tmp3 = static_cast<NAnd*>(prev);

        if (tmp3->lhs == tmp2) {
            tmp3->lhs = new NEventuallyAlways(static_cast<NBoolAtom*>(start), NULL, tmp2->b, tmp1->b);
        } else if (tmp3->rhs == tmp2) {
            tmp3->rhs = new NEventuallyAlways(static_cast<NBoolAtom*>(start), NULL, tmp2->b, tmp1->b);
        }

        ast->removePrevNode(prevNodes[0]);
        ast->removePrevNode(prevNodes[1]);

        delete tmp1;
        delete tmp2;
    }
}

BooleanExpression::BooleanExpression(operator_type type_, operator_subtype subtype_): Node(type_, subtype_) {}

AnalogExpression::AnalogExpression(operator_type type_, operator_subtype subtype_): Node(type_, subtype_) {}

NAlways::NAlways(BooleanExpression *l, BooleanExpression *r, Bound *bd): BooleanExpression(operator_type::futu_t, operator_subtype::always_t),
    lhs(l),
    rhs(r),
    b(bd) {}

void NAlways::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + std::string("\n");
        lhs->print(os, ast);
    }
    if (rhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(rhs->nodeId) + std::string("\n");
        rhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NAlways::optimize(AST *ast) {
    ast->setPrevNode(lhs, this);
    ast->setPrevNode(rhs, this);

    if (lhs) {
        lhs->optimize(ast);
    }
    if (rhs) {
        rhs->optimize(ast);
    }
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

NAnd::NAnd(BooleanExpression *l, BooleanExpression *r): BooleanExpression(operator_type::bool_t, operator_subtype::and_t),
    lhs(l),
    rhs(r) {}

void NAnd::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + std::string("\n");
        lhs->print(os, ast);
    }
    if (rhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(rhs->nodeId) + std::string("\n");
        rhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NAnd::optimize(AST *ast) {
    ast->setPrevNode(lhs, this);
    ast->setPrevNode(rhs, this);

    if (lhs) {
        lhs->optimize(ast);
    }
    if (rhs) {
        rhs->optimize(ast);
    }
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
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + std::string("\n");
        lhs->print(os, ast);
    }
    if (rhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(rhs->nodeId) + std::string("\n");
        rhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NEventually::optimize(AST *ast) {
    ast->setPrevNode(lhs, this);
    ast->setPrevNode(rhs, this);

    if (lhs) {
        lhs->optimize(ast);
    }
    if (rhs) {
        rhs->optimize(ast);
    }
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

NEventuallyAlways::NEventuallyAlways(BooleanExpression *l, BooleanExpression *r, Bound *bd1, Bound *bd2): BooleanExpression(operator_type::futu_t, operator_subtype::eventually_always_t),
    lhs(l),
    rhs(r),
    b1(bd1),
    b2(bd2) {}

void NEventuallyAlways::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + std::string("\n");
        lhs->print(os, ast);
    }
    if (rhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(rhs->nodeId) + std::string("\n");
        rhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NEventuallyAlways::optimize(AST *ast) {
}

void NEventuallyAlways::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }
    if (rhs) {
        rhs->codeGen(c);
    }

    std::string timeBound1;
    if (b1) {
        timeBound1 = intToString(b1->min) + std::string(", ") + intToString(b1->max);
    } else {
        timeBound1 = std::string("0, INT_MAX");
    }

    std::string timeBound2;
    if (b2) {
        timeBound2 = intToString(b2->min) + std::string(", ") + intToString(b2->max);
    } else {
        timeBound2 = std::string("0, INT_MAX");
    }

    std::string childParams = getChildParams(c, lhs, rhs);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_temp_operator_extended_new") +
                       std::string("(") + childParams + std::string("eventually_always_t") + std::string(", ") + timeBound1 + std::string(", ") + timeBound2 + std::string(");");
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
        os << "\t" + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)] + std::string("\n");
    label += std::string("variable: ") + variable + std::string("\n");
    label += std::string("operator: ") + op + std::string("\n");
    label += std::string("condition: ") + doubleToString(condition);
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NPredicate::optimize(AST *ast) {
}

void NPredicate::codeGen(CodeGenerator &c) {
    if (lhs) {
        lhs->codeGen(c);
    }

    std::string childParams = getChildParams(c, lhs, NULL);
    int channel = c.getChannel(variable);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_predicate_new") +
                       std::string("(") + childParams + op + std::string(", ") + intToString(channel) + std::string(", ") +
                       doubleToString(condition) + std::string(");");
    c.emitLine(line);
}

NImply::NImply(BooleanExpression *l, BooleanExpression *r): BooleanExpression(operator_type::bool_t, operator_subtype::imply_t),
    lhs(l),
    rhs(r) {}

void NImply::print(std::ostream &os, AST *ast) {
    if (lhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + std::string("\n");
        lhs->print(os, ast);
    }
    if (rhs) {
        os << std::string("\t") + intToString(nodeId) + std::string(" -> ") + intToString(rhs->nodeId) + std::string("\n");
        rhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NImply::optimize(AST *ast) {
    ast->setPrevNode(lhs, this);
    ast->setPrevNode(rhs, this);

    if (lhs) {
        lhs->optimize(ast);
    }
    if (rhs) {
        rhs->optimize(ast);
    }
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
        //os << "\t" + intToString(nodeId+100) + std::string(" -> ") + intToString(lhs->nodeId) + "\n";
        //os << "\t" + intToString(nodeId+100) + " [label=\"" + "define: " + variable + "\"];" + "\n";

        lhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NBoolAtom::optimize(AST *ast) {
    Node *lhs = ast->getNode(variable);
    if (lhs) {
        lhs->optimize(ast);
    }

    optimizeFutureOp(ast, this);
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
        os << "\t" + intToString(nodeId) + std::string(" -> ") + intToString(lhs->nodeId) + "\n";
        lhs->print(os, ast);
    }
    if (rhs) {
        os << "\t" + intToString(nodeId) + std::string(" -> ") + intToString(rhs->nodeId) + "\n";
        rhs->print(os, ast);
    }

    std::string label;
    label = intToString(nodeId) + std::string("\n");
    label += operator_type_strings[enumIndex(type)] + std::string("\n");
    label += operator_subtype_strings[subTypeIndex(subtype)];
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + label + "\"];" + std::string("\n");
}

void NEvent::optimize(AST *ast) {
    ast->setPrevNode(lhs, this);
    ast->setPrevNode(rhs, this);

    if (lhs) {
        lhs->optimize(ast);
    }
    if (lhs) {
        lhs->optimize(ast);
    }
}

void NEvent::codeGen(CodeGenerator &c) {
    std::string channel;
    std::string condition;

    if (lhs) {
        if (lhs->isBoolAtom()) {
            Node *lhs_ = c.ast->getNode((static_cast<NBoolAtom*>(lhs))->variable);
            int channel_ = c.getChannel((static_cast<NPredicate*>(lhs_))->variable);
            channel = std::string(", ") + intToString(channel_);
            condition = std::string(", ") + doubleToString((static_cast<NPredicate*>(lhs_))->condition);
        }
    }

    std::string childParams = getChildParams(c, NULL, NULL);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_event_new") +
                       std::string("(") + childParams + operator_subtype_strings[subTypeIndex(subtype)] + channel + condition + std::string(");");
    c.emitLine(line);
}

NAnalog::NAnalog(const char *variable): AnalogExpression(operator_type::anal_t, operator_subtype::none_t),
    variable(variable) {}

void NAnalog::print(std::ostream &os, AST *ast) {
    os << std::string("\t") + intToString(nodeId) + " [label=\"" + operator_type_strings[enumIndex(type)] + "\"];" + std::string("\n");
}

void NAnalog::optimize(AST *ast) {
}

void NAnalog::codeGen(CodeGenerator &c) {
    std::string childParams = getChildParams(c, NULL, NULL);
    int channel = c.getChannel(variable);

    std::string line = std::string("node *n") + id() + std::string(" = ") + std::string("node_analog_operator_new") +
                       std::string("(") + childParams + op + std::string(", ") + intToString(channel) + std::string(");");
    c.emitLine(line);
}

void NAnalog::setOperator(const std::string op_) {
    op = op_;
}

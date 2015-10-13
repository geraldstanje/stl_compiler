#include "code_generator.h"
#include "util.h"
#include "tinyxml2.h"
#include "bool_property.h"
#include <iostream>
#include <sstream>

CodeGenerator::CodeGenerator(AST *ast_): ast(ast_),
    indentPos(0) {}

void CodeGenerator::indent() {
    indentPos++;
}

void CodeGenerator::deindent() {
    indentPos--;

    if (indentPos < 0) {
        indentPos = 0;
    }
}

void CodeGenerator::emitLine(const std::string &line) {
    *os << std::string(indentPos, '\t');
    *os << line;
    *os << std::string("\n");
}

bool CodeGenerator::loadChanMapXML(const std::string &filename) {
    tinyxml2::XMLDocument xmldoc;
    tinyxml2::XMLError eResult = xmldoc.LoadFile(filename.c_str());
    if (eResult != tinyxml2::XML_NO_ERROR) {
        std::cerr << "error: xmldoc.LoadFile" << '\n';
        return false;
    }

    tinyxml2::XMLNode *pRoot = xmldoc.FirstChild();
    if (pRoot == nullptr) {
        std::cerr << "error: xmldoc.FirstChild" << '\n';
        return false;
    }

    tinyxml2::XMLElement *pElement = pRoot->FirstChildElement("StlAnalogProperty");

    while (pElement != nullptr) {
        const char *attributeName = pElement->Attribute("Name");
        if (attributeName == nullptr) {
            std::cerr << "error: pElement->Attribute" << '\n';
            return false;
        }
        const char *attributePort = pElement->Attribute("Port");
        if (attributePort == nullptr) {
            std::cerr << "error: pElement->Attribute" << '\n';
            return false;
        }

        channelMap.insert(std::make_pair(std::string(attributeName), stringToInt(std::string(attributePort))));

        pElement = pElement->NextSiblingElement("StlAnalogProperty");
    }

    return true;
}

int CodeGenerator::getChannel(const std::string &variable) {
    auto it = channelMap.find(variable);
    if (it != channelMap.end()) {
        return it->second;
    } else {
        std::cerr << variable << '\n';
        std::cerr << "error: variable doesnt exist in channelMap" << '\n';
        std::exit(1);
    }
}

void CodeGenerator::generateCode(std::ostream *os_) {
    os = os_;

    emitLine(std::string("#include \"node.h\""));
    emitLine(std::string("#include \"stltree.h\""));
    emitLine(std::string(""));
    emitLine(std::string("void stltree_create(stltree *t) {"));

    indent();
    ast->root->codeGen(*this);
    emitLine(std::string("t->root = n") + ast->root->id() + std::string(";"));
    deindent();

    emitLine(std::string("}"));
}

void CodeGenerator::writeBoolProp(const std::string &filename) {
    BoolProperty p;

    for (auto kv : ast->defineDecl) {
        if (kv.second->type == operator_type::pred_t) {
            std::string name = kv.first;
            int port = getChannel(static_cast<NPredicate*>(kv.second)->variable);

            auto analogOpNode = static_cast<NPredicate*>(kv.second)->lhs;
            std::string analogOp = static_cast<NAnalog*>(analogOpNode)->op;

            std::string binOp = static_cast<NPredicate*>(kv.second)->op;
            std::string binOpCond = doubleToString(static_cast<NPredicate*>(kv.second)->condition);

            p.add(name, intToString(port), analogOp, binOp, binOpCond);
        }
    }

    p.writeXML(filename);
}

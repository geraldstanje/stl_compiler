#include "bool_property.h"
#include "tinyxml2.h"
#include <iostream>

BoolProperty::BoolProperty() {}


void BoolProperty::add(std::string name_, std::string port_, std::string analogOp_, std::string binaryOp_, std::string condition_) {
    vec.push_back(BoolPropData(name_, port_, analogOp_, binaryOp_, condition_));
}

void BoolProperty::writeXML(const std::string &filename) {
    tinyxml2::XMLDocument xmldoc;
    tinyxml2::XMLNode *pRoot = xmldoc.NewElement("StlBoolProperty");
    xmldoc.InsertFirstChild(pRoot);

    for (auto it = vec.begin(); it != vec.end(); it++) {
        tinyxml2::XMLElement *pElement = xmldoc.NewElement("Property");

        pElement->SetAttribute("Name", it->name.c_str());
        pElement->SetAttribute("Port", it->port.c_str());
        pElement->SetAttribute("AnalogOp", it->analogOp.c_str());
        pElement->SetAttribute("BinaryOp", it->binaryOp.c_str());
        pElement->SetAttribute("Condition", it->condition.c_str());

        pRoot->InsertEndChild(pElement);
    }

    tinyxml2::XMLError eResult = xmldoc.SaveFile(filename.c_str());
    if (eResult != tinyxml2::XML_NO_ERROR) {
        std::cerr << "error: xmldoc.LoadFile";
        return;
    }
}

bool BoolProperty::readXML(const std::string &filename, std::vector<BoolPropData> &boolProp) {
    tinyxml2::XMLDocument xmldoc;
    tinyxml2::XMLError eResult = xmldoc.LoadFile(filename.c_str());
    if (eResult != tinyxml2::XML_NO_ERROR) {
        std::cerr << "error: xmldoc.LoadFile";
        return false;
    }

    tinyxml2::XMLNode *pRoot = xmldoc.FirstChild();
    if (pRoot == nullptr) {
        std::cerr << "error: xmldoc.FirstChild";
        return false;
    }

    tinyxml2::XMLElement *pElement = pRoot->FirstChildElement("Property");

    while (pElement != nullptr) {
        const char *attributeName = pElement->Attribute("Name");
        if (attributeName == nullptr) {
            std::cerr << "error: pElement->Attribute";
            return false;
        }
        const char *attributePort = pElement->Attribute("Port");
        if (attributePort == nullptr) {
            std::cerr << "error: pElement->Attribute";
            return false;
        }
        const char *attributeAnalogOp = pElement->Attribute("AnalogOp");
        if (attributePort == nullptr) {
            std::cerr << "error: pElement->Attribute";
            return false;
        }
        const char *attributeBinaryOp = pElement->Attribute("BinaryOp");
        if (attributePort == nullptr) {
            std::cerr << "error: pElement->Attribute";
            return false;
        }
        const char *attributeCondition = pElement->Attribute("Condition");
        if (attributePort == nullptr) {
            std::cerr << "error: pElement->Attribute";
            return false;
        }

        boolProp.push_back(BoolPropData(std::string(attributeName),
                                        std::string(attributePort),
                                        std::string(attributeAnalogOp),
                                        std::string(attributeBinaryOp),
                                        std::string(attributeCondition)));

        pElement = pElement->NextSiblingElement("Property");
    }

    return true;
}

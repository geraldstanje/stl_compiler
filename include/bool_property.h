#ifndef BOOL_PROP_H
#define BOOL_PROP_H

#include <string>
#include <vector>

class BoolPropData {
public:
    std::string name;
    std::string port;
    std::string analogOp;
    std::string binaryOp;
    std::string condition;

public:
    BoolPropData(std::string name_, std::string port_, std::string analogOp_, std::string binaryOp_, std::string condition_): name(name_),
        port(port_),
        analogOp(analogOp_),
        binaryOp(binaryOp_),
        condition(condition_) {}
};

class BoolProperty {
private:
    std::vector<BoolPropData> vec;

public:
    BoolProperty();

    void add(std::string name_, std::string port_, std::string analogOp_, std::string binaryOp_, std::string condition_);
    void writeXML(const std::string &filename);
    bool readXML(const std::string &filename, std::vector<BoolPropData> &boolProp);
};

#endif

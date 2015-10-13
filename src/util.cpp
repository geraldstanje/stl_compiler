#include "util.h"
#include <sstream>
#include <locale>
#include <iomanip>

double stringToDouble(const std::string &in) {
    double res = 0.0;
    std::stringstream ss;
    ss << in;
    ss >> res;
    return res;
}

std::string doubleToString(const double &num) {
    std::stringstream ss;
    ss << std::setprecision(3) << std::fixed << num;
    return ss.str();
}

int stringToInt(const std::string &in) {
    int res = 0;
    std::stringstream ss;
    ss << in;
    ss >> res;
    return res;
}

std::string intToString(const int &num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

bool isPrefix(const std::string &a, const std::string &b) {
    if (a.size() > b.size()) {
        return a.substr(0, b.size()) == b;
    }
    return b.substr(0, a.size()) == a;
}

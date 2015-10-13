#ifndef UTIL_H
#define UTIL_H

#include <string>

double stringToDouble(const std::string &in);
std::string doubleToString(const double &num);
int stringToInt(const std::string &in);
std::string intToString(const int &num);
bool isPrefix(const std::string &a, const std::string &b);

#endif

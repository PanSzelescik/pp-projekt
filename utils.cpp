#ifndef utils_cpp
#define utils_cpp

#include <iomanip>
#include <sstream>
#include <string>
#include "utils.cpp"

using namespace std;

string toLowercase(string s) {
    return string(1, (char) tolower(s.at(0)));
}

string doubleToString(const double &d) {
    stringstream stream;
    stream << fixed << setprecision(2) << d;
    return stream.str();
}

int safeStringToInt(const string &str) {
    try {
        return stoi(str);
    } catch (invalid_argument) {
        return -1;
    }
}

int stringToIndex(const string &choice) {
    auto number = safeStringToInt(choice);
    return number == -1 ? number : number - 1;
}

#endif
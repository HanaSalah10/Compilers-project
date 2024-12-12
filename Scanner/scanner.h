#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
//#include "parser.h"
typedef char CHAR;
typedef unsigned long DWORD;
typedef char* PZPSTR; // Define PZPSTR

// Token structure
struct Token {
    std::string value;
    std::string type;
};

// Function declarations
bool isNumber(const std::string& str);
bool isIdentifier(const std::string& str);
std::string getTokenType(const std::string& token);
std::vector<Token> tokenize(const std::string& code);

#endif // SCANNER_H
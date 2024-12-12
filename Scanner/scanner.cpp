#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "scanner.h" // Include the scanner header file
#include "../Parser/parser.h"
#include "../Parser/Syntax-tree-parser/Node.h" // Include the Node class
using namespace std;

const unordered_map<string, string> reservedWords = {
    {"if", "IF"}, {"then", "THEN"}, {"end", "END"}, {"repeat", "REPEAT"},
    {"until", "UNTIL"}, {"read", "READ"}, {"write", "WRITE"}, {"else", "ELSE"}
};

// Symbols
const unordered_map<string, string> symbols = {
    {";", "SEMICOLON"}, {":=", "ASSIGN"}, {"<", "LESSTHAN"}, {"=", "EQUAL"},
    {"+", "PLUS"}, {"-", "MINUS"}, {"*", "MULT"}, {"/", "DIV"},
    {"(", "OPENBRACKET"}, {")", "CLOSEDBRACKET"}
};

// Get token type
string getTokenType(const string& token) {
    if (reservedWords.count(token)) return reservedWords.at(token);
    if (isNumber(token)) return "NUMBER";
    if (isIdentifier(token)) return "IDENTIFIER";

    // Exit with error if the token is unknown
    cerr << "Error: Unknown token encountered: " << token << endl;
    exit(1);
}

// Check if a string is a number
bool isNumber(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Check if a string is an identifier
bool isIdentifier(const string& str) {
    if (str.empty() || !isalpha(str[0])) return false;
    for (char c : str) {
        if (!isalpha(c)) return false;
    }
    return true;
}

// Tokenizer function
vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    string currentToken;
    bool inComment = false;

    for (int i = 0; i < code.length(); i++) {
        char c = code[i];

        if (c == '{') {inComment = true;}

        if (inComment) {
            if (c == '}') {inComment = false;}
            continue;
        }

        // Check for assign
        if (c == ':' && i + 1 < code.length() && code[i + 1] == '=') {
            tokens.push_back({":=", "ASSIGN"});
            i++;
        }
        // Check if character is symbol
        else if (symbols.count(string(1, c))) {
            if (!currentToken.empty()) {
                tokens.push_back({currentToken, getTokenType(currentToken)});
                currentToken.clear();
            }
            tokens.push_back({string(1, c), symbols.at(string(1, c))});
        }
        else if (isspace(c)) {
            if (!currentToken.empty()) {
                tokens.push_back({currentToken, getTokenType(currentToken)});
                currentToken.clear();
            }
        }
        else {
            currentToken += c;
        }
    }

    // Process any remaining token at end of string
    if (!currentToken.empty()) {
        tokens.push_back({currentToken, getTokenType(currentToken)});
    }

    return tokens;
}


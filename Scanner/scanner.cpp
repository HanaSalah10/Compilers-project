#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "parser.cpp" // Include the parser header file

// Forward declaration of Node structure
struct Node;

using namespace std;

// Token structure
struct Token {
    string value;
    string type;
};

// Forward declarations of helper functions
bool isNumber(const string& str);
bool isIdentifier(const string& str);
string getTokenType(const string& token);

// Reserved words
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
        if (!isalnum(c)) return false;
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

// Main function
int main() {
    string inputFileName, outputFileName;
    cout << "Enter the input file name: ";
    cin >> inputFileName;
    cout << "Enter the output file name: ";
    cin >> outputFileName;

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    // Check if input file is open
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file: " << inputFileName << endl;
        return 1;
    }

    // Check if output file is open
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file: " << outputFileName << endl;
        return 1;
    }


    stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str();

    vector<Token> tokens = tokenize(code);

        // Convert tokens to lists for the parser
    vector<string> tokens_list;
    vector<string> code_list;
    for (const auto& token : tokens) {
        tokens_list.push_back(token.type);
        code_list.push_back(token.value);
    }

    // Initialize and run the parser
    Parser parser;
    parser.set_tokens_list_and_code_list(tokens_list, code_list);
    parser.run();

    // Output the syntax tree to the output file
    printSyntaxTree(parser.parse_tree, outputFile);

    cout << "Tokenization and parsing completed. Check the output file." << endl;

    /*
    for (const auto& token : tokens) {
        outputFile << token.value << ", " << token.type << endl;
    }

*/
    inputFile.close();
    outputFile.close();
    return 0;

}

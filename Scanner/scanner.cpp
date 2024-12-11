#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

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

    // Read input file
    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error: Unable to open file!" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str();

    vector<Token> tokens = tokenize(code);

    // Write output file
    for (const auto& token : tokens) {
        outputFile << token.value << ", " << token.type << endl;
    }

    cout << "Tokenization completed. Check the output file." << endl;

    inputFile.close();
    outputFile.close();
    return 0;
}

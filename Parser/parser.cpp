#include "parser.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

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

    // Tokenize the input source code
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
    Node* parse_tree = parser.stmt_sequence(); // create parse tree
    if (parser.tmp_index == parser.tokens_list.size() - 1) {
        cout << "success" << endl;
    } else if (parser.tmp_index < parser.tokens_list.size()) {
        cerr << "SyntaxError: " << parser.token << endl;
        return 1;
    }

    cout << "Tokenization and parsing completed. Check the output file." << endl;

    inputFile.close();
    outputFile.close();
    return 0;
}

// Parser class implementation
Parser::Parser() : tmp_index(0) {}

void Parser::set_tokens_list_and_code_list(vector<string> x, vector<string> y) {
    code_list = y;
    tokens_list = x;
    tmp_index = 0;
    token = tokens_list[tmp_index];
}

bool Parser::next_token() {
    if (tmp_index == tokens_list.size() - 1) {
        return false; // we have reached the end of the list
    }
    tmp_index++;
    token = tokens_list[tmp_index];
    return true;
}

bool Parser::match(string x) {
    if (token == x) {
        cout<<"Token"<<token<<endl;
        next_token();
        return true;
    } else {
        cerr << "Token Mismatch: " << token << endl;
        exit(1);
    }
}

Node* Parser::statement() {
    if (token == "IF") {
        return if_stmt();
    } else if (token == "REPEAT") {
        return repeat_stmt();
    } else if (token == "IDENTIFIER") {
        return assign_stmt();
    } else if (token == "READ") {
        return read_stmt();
    } else if (token == "WRITE") {
        return write_stmt();
    } else {
        cerr << "SyntaxError: " << token << endl;
        exit(1);
    }
}

Node* Parser::stmt_sequence() {
    Node* t = statement();
    Node* p = t;
    while (token == "SEMICOLON") {
        match("SEMICOLON");
        Node* q = statement();
        if (q == nullptr) {
            break;
        } else {
            if (t == nullptr) {
                t = p = q;
            } else {
                p->setSibling(q);
                p = q;
            }
        }
    }
    return t;
}

Node* Parser::factor() {
    Node* t = nullptr;
    if (token == "OPENBRACKET") {
        match("OPENBRACKET");
        t = exp();
        match("CLOSEDBRACKET");
    } else if (token == "NUMBER") {
        t = new Node(Token{"CONSTANT", "(" + code_list[tmp_index] + ")"},code_list[tmp_index]);
        match("NUMBER");
    } else if (token == "IDENTIFIER") {
        t = new Node(Token{"IDENTIFIER", "(" + code_list[tmp_index] + ")"},code_list[tmp_index]);
        match("IDENTIFIER");
    } else {
        cerr << "SyntaxError: " << token << endl;
        exit(1);
    }
    return t;
}

Node* Parser::term() {
    Node* t = factor();
    while (token == "MULT" || token == "DIV") {
        Node* p = new Node(Token{"OPERATOR", "(" + code_list[tmp_index] + ")"},code_list[tmp_index]);
        p->addChild(t);
        t = p;
        mulop();
        p->addChild(factor());
    }
    return t;
}

Node* Parser::simple_exp() {
    Node* t = term();
    while (token == "PLUS" || token == "MINUS") {
        Node* p = new Node(Token{"OPERATOR", "(" + code_list[tmp_index] + ")"});
        p->addChild(t);
        t = p;
        addop();
        t->addChild(term());
    }
    return t;
}

Node* Parser::exp() {
    Node* t = simple_exp();
    if (token == "LESSTHAN" || token == "EQUAL" || token == "GREATERTHAN") {
        Node* p = new Node(Token{"OPERATOR", "(" + code_list[tmp_index] + ")"},code_list[tmp_index]);
        p->addChild(t);
        t = p;
        comparison_op();
        t->addChild(simple_exp());
    }
    return t;
}

Node* Parser::if_stmt() {
    Node* t = new Node(Token{"IF", ""});
    if (token == "IF") {
        match("IF");
        t->addChild(exp());
        match("THEN");
        t->addChild(stmt_sequence());
        if (token == "ELSE") {
            match("ELSE");
            t->addChild(stmt_sequence());
        }
        match("END");
    }
    return t;
}

void Parser::comparison_op() {
    if (token == "LESSTHAN") {
        match("LESSTHAN");
    } else if (token == "EQUAL") {
        match("EQUAL");
    } else if (token == "GREATERTHAN") {
        match("GREATERTHAN");
    }
}

void Parser::addop() {
    if (token == "PLUS") {
        match("PLUS");
    } else if (token == "MINUS") {
        match("MINUS");
    }
}

void Parser::mulop() {
    if (token == "MULT") {
        match("MULT");
    } else if (token == "DIV") {
        match("DIV");
    }
}

Node* Parser::repeat_stmt() {
    Node* t = new Node(Token{"REPEAT", ""});
    if (token == "REPEAT") {
        match("REPEAT");
        t->addChild(stmt_sequence());
        match("UNTIL");
        t->addChild(exp());
    }
    return t;
}

Node* Parser::assign_stmt() {
    Node* t = new Node(Token{"ASSIGN", "(" + code_list[tmp_index] + ")"},code_list[tmp_index]);
    match("IDENTIFIER");
    match("ASSIGN");
    t->addChild(exp());
    return t;
}

Node* Parser::read_stmt() {
    Node* t = new Node(Token{"READ", "(" + code_list[tmp_index + 1] + ")"},code_list[tmp_index]);
    match("READ");
    match("IDENTIFIER");
    return t;
}

Node* Parser::write_stmt() {
    Node* t = new Node(Token{"WRITE", ""});
    match("WRITE");
    t->addChild(exp());
    return t;
}




#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"
#include "scanner.h"
#include "syntaxtreewidget.h"

using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::ifstream inputFile("E:/Fall 2024/Design of Compilers/Compilers-project/Scanner/input-1.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str();

    // Tokenize the input source code
    vector<Token> tokens = tokenize(code);
    if (tokens.empty()) {
        std::cerr << "Error: No tokens found." << std::endl;
        return 1;
    }
    for(Token token:tokens)
    {
        cout<<token.type<<endl;
         cout<<token.value<<endl;
    }

    // Convert tokens to lists for the parser
    vector<string> tokens_list;
    vector<string> code_list;
    for (const auto& token : tokens) {
        tokens_list.push_back(token.type);
        code_list.push_back(token.value);
    }
    cout<<"token_list"<<tokens_list.size();

    // // Initialize and run the parser
    Parser parser;
    parser.set_tokens_list_and_code_list(tokens_list, code_list);
    Node* parse_tree = parser.stmt_sequence();
    if(parse_tree->getSibling())
    {
        cout<<"has sibling";
    }
    else
    {
         cout<<"doe not hassibling";
    }
    // if (!parse_tree) {
    //     std::cerr << "Error: Parse tree is null." << std::endl;
    //     return 1;
    // }

    // cout << "Parse tree root value: " << parse_tree->get().value << endl;

    SyntaxTreeWidget widget(parse_tree);
    widget.show();

    return app.exec();
}

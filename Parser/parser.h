#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Node.h" // Include the Node class
#include "SyntaxTree.h" // Include the SyntaxTree class
//#include "scanner.h" // Include the scanner header file
#include "../Scanner/scanner.h"

class Parser {
public:
    std::vector<std::string> tokens_list;
    std::vector<std::string> code_list;
    int tmp_index;
    std::string token;
    std::unordered_map<int, std::vector<std::string>> nodes_table;
    std::vector<std::pair<int, int>> edges_table;
    std::vector<std::pair<int, int>> same_rank_nodes;

    Parser();

    void set_tokens_list_and_code_list(std::vector<std::string> x, std::vector<std::string> y);
    bool next_token();
    bool match(std::string x);
    Node* statement();
    Node* stmt_sequence();
    Node* factor();
    Node* term();
    Node* simple_exp();
    Node* exp();
    Node* if_stmt();
    void comparison_op();
    void addop();
    void mulop();
    Node* repeat_stmt();
    Node* assign_stmt();
    Node* read_stmt();
    Node* write_stmt();
    void run();
};

// Function to print the syntax tree
void printSyntaxTree(Node* node, std::ostream& out, int indent = 0);

#endif // PARSER_H
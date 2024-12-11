#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Node.h" // Include the Node class

using namespace std;

class Parser {
public:
    vector<string> tokens_list;
    vector<string> code_list;
    int tmp_index;
    string token;
    Node* parse_tree;
    unordered_map<int, vector<string>> nodes_table;
    vector<pair<int, int>> edges_table;
    vector<pair<int, int>> same_rank_nodes;

    Parser() : tmp_index(0), parse_tree(nullptr) {}

    void set_tokens_list_and_code_list(vector<string> x, vector<string> y) {
        code_list = y;
        tokens_list = x;
        tmp_index = 0;
        token = tokens_list[tmp_index];
    }

    bool next_token() {
        if (tmp_index == tokens_list.size() - 1) {
            return false; // we have reached the end of the list
        }
        tmp_index++;
        token = tokens_list[tmp_index];
        return true;
    }

    bool match(string x) {
        if (token == x) {
            next_token();
            return true;
        } else {
            cerr << "Token Mismatch: " << token << endl;
            exit(1);
        }
    }

    Node* statement() {
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

    Node* stmt_sequence() {
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

    Node* factor() {
        Node* t = nullptr;
        if (token == "OPENBRACKET") {
            match("OPENBRACKET");
            t = exp();
            match("CLOSEDBRACKET");
        } else if (token == "NUMBER") {
            t = new Node(Token{"CONSTANT", "(" + code_list[tmp_index] + ")"});
            match("NUMBER");
        } else if (token == "IDENTIFIER") {
            t = new Node(Token{"IDENTIFIER", "(" + code_list[tmp_index] + ")"});
            match("IDENTIFIER");
        } else {
            cerr << "SyntaxError: " << token << endl;
            exit(1);
        }
        return t;
    }

    Node* term() {
        Node* t = factor();
        while (token == "MULT" || token == "DIV") {
            Node* p = new Node(Token{"OPERATOR", "(" + code_list[tmp_index] + ")"});
            p->addChild(t);
            t = p;
            mulop();
            p->addChild(factor());
        }
        return t;
    }

    Node* simple_exp() {
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

    Node* exp() {
        Node* t = simple_exp();
        if (token == "LESSTHAN" || token == "EQUAL" || token == "GREATERTHAN") {
            Node* p = new Node(Token{"OPERATOR", "(" + code_list[tmp_index] + ")"});
            p->addChild(t);
            t = p;
            comparison_op();
            t->addChild(simple_exp());
        }
        return t;
    }

    Node* if_stmt() {
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

    void comparison_op() {
        if (token == "LESSTHAN") {
            match("LESSTHAN");
        } else if (token == "EQUAL") {
            match("EQUAL");
        } else if (token == "GREATERTHAN") {
            match("GREATERTHAN");
        }
    }

    void addop() {
        if (token == "PLUS") {
            match("PLUS");
        } else if (token == "MINUS") {
            match("MINUS");
        }
    }

    void mulop() {
        if (token == "MULT") {
            match("MULT");
        } else if (token == "DIV") {
            match("DIV");
        }
    }

    Node* repeat_stmt() {
        Node* t = new Node(Token{"REPEAT", ""});
        if (token == "REPEAT") {
            match("REPEAT");
            t->addChild(stmt_sequence());
            match("UNTIL");
            t->addChild(exp());
        }
        return t;
    }

    Node* assign_stmt() {
        Node* t = new Node(Token{"ASSIGN", "(" + code_list[tmp_index] + ")"});
        match("IDENTIFIER");
        match("ASSIGN");
        t->addChild(exp());
        return t;
    }

    Node* read_stmt() {
        Node* t = new Node(Token{"READ", "(" + code_list[tmp_index + 1] + ")"});
        match("READ");
        match("IDENTIFIER");
        return t;
    }

    Node* write_stmt() {
        Node* t = new Node(Token{"WRITE", ""});
        match("WRITE");
        t->addChild(exp());
        return t;
    }

    void create_nodes_table(Node* args = nullptr) {
        if (args == nullptr) {
            parse_tree->setIndex(tmp_index);
            nodes_table[tmp_index] = {parse_tree->get().value, parse_tree->get().type};
            tmp_index++;
            if (!parse_tree->getChildren().empty()) {
                for (Node* i : parse_tree->getChildren()) {
                    create_nodes_table(i);
                }
            }
            if (parse_tree->getSibling() != nullptr) {
                create_nodes_table(parse_tree->getSibling());
            }
        } else {
            args->setIndex(tmp_index);
            nodes_table[tmp_index] = {args->get().value, args->get().type};
            tmp_index++;
            if (!args->getChildren().empty()) {
                for (Node* i : args->getChildren()) {
                    create_nodes_table(i);
                }
            }
            if (args->getSibling() != nullptr) {
                create_nodes_table(args->getSibling());
            }
        }
    }

    void create_edges_table(Node* args = nullptr) {
        if (args == nullptr) {
            if (!parse_tree->getChildren().empty()) {
                for (Node* i : parse_tree->getChildren()) {
                    edges_table.push_back({parse_tree->getIndex(), i->getIndex()});
                }
                for (Node* j : parse_tree->getChildren()) {
                    create_edges_table(j);
                }
            }
            if (parse_tree->getSibling() != nullptr) {
                edges_table.push_back({parse_tree->getIndex(), parse_tree->getSibling()->getIndex()});
                same_rank_nodes.push_back({parse_tree->getIndex(), parse_tree->getSibling()->getIndex()});
                create_edges_table(parse_tree->getSibling());
            }
        } else {
            if (!args->getChildren().empty()) {
                for (Node* i : args->getChildren()) {
                    edges_table.push_back({args->getIndex(), i->getIndex()});
                }
                for (Node* j : args->getChildren()) {
                    create_edges_table(j);
                }
            }
            if (args->getSibling() != nullptr) {
                edges_table.push_back({args->getIndex(), args->getSibling()->getIndex()});
                same_rank_nodes.push_back({args->getIndex(), args->getSibling()->getIndex()});
                create_edges_table(args->getSibling());
            }
        }
    }

    void run() {
        parse_tree = stmt_sequence(); // create parse tree
        create_nodes_table(); // create nodes_table
        create_edges_table(); // create edges_table
        if (tmp_index == tokens_list.size() - 1) {
            cout << "success" << endl;
        } else if (tmp_index < tokens_list.size()) {
            cerr << "SyntaxError: " << token << endl;
            exit(1);
        }
    }

    void clear_tables() {
        nodes_table.clear();
        edges_table.clear();
    }
};

// Function to print the syntax tree
void printSyntaxTree(Node* node, ostream& out, int indent = 0) {
    if (node == nullptr) return;
    for (int i = 0; i < indent; ++i) out << "  ";
    out << node->get().value << " (" << node->get().type << ")" << endl;
    for (Node* child : node->getChildren()) {
        printSyntaxTree(child, out, indent + 1);
    }
}
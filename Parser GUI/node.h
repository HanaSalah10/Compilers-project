#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "scanner.h"
typedef char CHAR;
typedef unsigned long DWORD;

using namespace std;

class Node
{
private:
    Token token;             // Token associated with this node
    vector<Node *> children; // Children of this node
    Node *sibling;           // Pointer to the next sibling node (for same-level nodes)
    int index;               // Optional index or position in the tree

public:
    // Constructors
    Node(); // Default constructor

    // Constructor: Initialize with token type and optional value
    Node(const Token &token, const std::string &value = "");


    // Methods
    void makeOpToken(Token token); // Set the node's token
    Token get();                   // Get the node's token
    vector<Node *> getChildren();  // Get the children of the node
    void addChild(Node *node);     // Add a child node
    void setSibling(Node *node);   // Set the sibling node
    Node *getSibling();            // Get the next sibling node
    void setIndex(int idx);        // Set the index of the node
    int getIndex();                // Get the index of the node

    // Destructor to clean up dynamically allocated memory for children and siblings
    ~Node();
};

#endif // NODE_H

/* ************************************************************************ */
/*  File: main.cpp                                                          */
/*  Description: This file contains the main function to test               */
/*               node creation and  syntax tree structure.                  */
/* ************************************************************************ */

#include <iostream>
#include "SyntaxTree.h" // Include "SyntaxTree.h"

void printTree(Node *node, int depth = 0)
{
    for (int i = 0; i < depth; ++i)
        cout << "  ";                  // Indentation for tree structure
    cout << node->get().value << endl; // Print the node value (from Token)

    for (Node *child : node->getChildren())
    {
        printTree(child, depth + 1); // Recursively print children with increased depth
    }

    if (node->getSibling() != nullptr)
    {
        printTree(node->getSibling(), depth); // Print siblings on the same level
    }
}

int main()
{
    // Example: Create Token objects for each node
    Token assignToken = {"Assign", "KEYWORD"};
    Token identifierToken = {"x", "IDENTIFIER"};
    Token numberToken = {"4", "NUMBER"};
    Token plusToken = {"+", "PLUS"};
    Token multiplyToken = {"*", "MULT"};

    // Create nodes using the Token constructor
    Node *assignNode = new Node(assignToken);         // Create the "Assign" node
    Node *identifierNode = new Node(identifierToken); // Create the "x" node
    Node *numberNode = new Node(numberToken);         // Create the "4" node
    Node *plusNode = new Node(plusToken);             // Create the "+" node
    Node *multiplyNode = new Node(multiplyToken);     // Create the "*" node

    // Build the tree structure
    assignNode->addChild(identifierNode); // Add identifier as a child to the "Assign" node
    assignNode->addChild(numberNode);     // Add number as a child to the "Assign" node

    // Adding siblings correctly:
    assignNode->addChild(plusNode);       // Add plus node as a child
    numberNode->setSibling(multiplyNode); // Add multiply node as a sibling to the "Number"

    // Create a syntax tree and set the root
    SyntaxTree tree(assignNode);

    // Print the tree structure
    cout << "Syntax Tree Structure:" << endl;
    printTree(tree.getRoot(), 0);

    return 0;
}

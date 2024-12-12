#include "SyntaxTree.h"

// Default constructor
SyntaxTree::SyntaxTree() : root(nullptr) {}

// Constructor with a root node
SyntaxTree::SyntaxTree(Node *root) : root(root) {}

// Add a child node under a parent node
void SyntaxTree::makeChild(Node *parent, Node *node)
{
    if (parent != nullptr && node != nullptr)
    {
        parent->addChild(node);
    }
}

// Link two nodes as siblings (same level)
void makeSibling(Node *node1, Node *node2)
{
    node1->setSibling(node2);
}

// Getter for the root node
Node *SyntaxTree::getRoot() const
{
    return root;
}

// Setter for the root node
void SyntaxTree::setRoot(Node *root)
{
    this->root = root;
}

// Destructor to clean up resources (if needed, for deep copy implementation)

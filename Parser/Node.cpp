
#include "Node.h"

// Default constructor
Node::Node() : sibling(nullptr), index(-1) {}

// Constructor with a token
Node::Node(const Token& token, const std::string& value)
    : token(token), sibling(nullptr), index(-1) 
{
    // If the value is provided, override the token's value
    if (!value.empty()) {
        this->token.value = value;
    }
}


// Set the node's token
void Node::makeOpToken(Token token) {
    this->token = token;
}

// Get the node's token
Token Node::get() {
    return token;
}

// Get the children of the node
vector<Node*> Node::getChildren() {
    return children;
}

// Add a child node
void Node::addChild(Node* node) {
    children.push_back(node);
}

// Set the sibling node
void Node::setSibling(Node* node) {
    sibling = node;
}

// Get the next sibling node
Node* Node::getSibling() {
    return sibling;
}

// Set the index of the node
void Node::setIndex(int idx) {
    index = idx;
}

// Get the index of the node
int Node::getIndex() {
    return index;
}

// Destructor to clean up dynamically allocated memory for children and siblings
Node::~Node() {
    for (Node* child : children) {
        delete child; // Recursively delete children
    }
    if (sibling) {
        delete sibling; // Optionally delete sibling
    }
}

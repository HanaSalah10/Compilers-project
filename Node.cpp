#include "Node.h"

Node::Node() : sibling(nullptr), index(-1) {}

Node::Node(Token token) : token(token), sibling(nullptr), index(-1) {}

void Node::makeOpToken(Token token) {
    this->token = token;
}

Token Node::get() {
    return token;
}

std::vector<Node*> Node::getChildren() {
    return children;
}

void Node::addChild(Node* node) {
    children.push_back(node);
}

void Node::setSibling(Node* node) {
    sibling = node;
}

Node* Node::getSibling() {
    return sibling;
}

void Node::setIndex(int idx) {
    index = idx;
}

int Node::getIndex() {
    return index;
}

Node::~Node() {}
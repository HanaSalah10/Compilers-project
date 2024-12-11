#include "Node.h"

Node::Node(){}

Node::Node(Token token)
{
    this->token = token;
}
void Node::makeOpToken(Token token)
{
    this->token = token;
}   
Token Node::get()
{
    return token;
}
std::vector<Node*> Node::getChildren()
{
    return children;
}
void Node::addChild(Node *node)
{
    this->children.push_back(node);
}
Node::~Node(){}
#include "SyntaxTree.h"

SyntaxTree::SyntaxTree(){}
SyntaxTree::SyntaxTree(Node *root)
{
    this->root = root;
}
void SyntaxTree::makeChild(Node * parent,Node *node)
{
    parent->addChild(node);
}

SyntaxTree::~SyntaxTree(){}
#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#pragma once

#include <vector>
#include "node.h"

class SyntaxTree
{
public:
    SyntaxTree();
    SyntaxTree(Node *root);
    SyntaxTree(SyntaxTree &tree);
    void makeChild(Node*parent,Node *node);

    ~SyntaxTree();

private:
  Node *root;
};

#endif
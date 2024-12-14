#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#pragma once

#include "Node.h"

class SyntaxTree
{
private:
  Node *root; // Root node of the syntax tree

public:
  // Constructors
  SyntaxTree();           // Default constructor
  SyntaxTree(Node *root); // Constructor with root node

  // Method to add a child node
  void makeChild(Node *parent, Node *node);

  // Link two nodes as siblings (same level)
  void makeSibling(Node *node1, Node *node2);

  // Getter for the root node
  Node *getRoot() const;

  // Setter for the root node
  void setRoot(Node *root);

  // Destructor to clean up resources
  ~SyntaxTree();
};

#endif // SYNTAXTREE_H

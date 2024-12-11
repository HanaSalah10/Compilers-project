#ifndef NODE_H
#define NODE_H

#pragma once

#include <string>
#include <vector>

struct Token {
    std::string value;
    std::string type;
};
class Node
{
public:
    Node();
    Node(Token token);
    void makeOpToken(Token token);
    void addChild(Node *node);
    Token get();
    std::vector<Node*> getChildren();
    ~Node();

private:
  Token token;
  std::vector<Node*> children;

};

#endif
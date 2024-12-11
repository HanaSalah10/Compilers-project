#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

struct Token {
    std::string value;
    std::string type;
};

class Node {
public:
    Node();
    Node(Token token);
    void makeOpToken(Token token);
    Token get();
    std::vector<Node*> getChildren();
    void addChild(Node* node);
    void setSibling(Node* node);
    Node* getSibling();
    void setIndex(int idx);
    int getIndex();
    ~Node();

private:
    Token token;
    std::vector<Node*> children;
    Node* sibling;
    int index;
};

#endif // NODE_H
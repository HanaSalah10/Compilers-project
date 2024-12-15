#include "syntaxtreewidget.h"

SyntaxTreeWidget::SyntaxTreeWidget(Node* root,QWidget *parent)
    :QWidget(parent),m_root(root)
{}

int SyntaxTreeWidget::calculateTotalWidth(Node* node, int horizontalSpacing) {
    if (!node) return 0;

    int nodeWidth = 50;  // Default width for a single node

    // Calculate total width for children
    int childrenWidth = 0;
    if (!node->getChildren().empty()) {
        for (auto child : node->getChildren()) {
            childrenWidth += calculateTotalWidth(child, horizontalSpacing);
        }
        childrenWidth += horizontalSpacing * (node->getChildren().size() - 1);
    }

    // Width of current subtree includes children or the node itself
    int subtreeWidth = std::max(nodeWidth, childrenWidth);

    // Add the width of the sibling (next)
    if (node->getSibling()) {
        subtreeWidth += calculateTotalWidth(node->getSibling(), horizontalSpacing) + horizontalSpacing;
    }

    return subtreeWidth;
}


void SyntaxTreeWidget::drawTree(QPainter& painter, Node* node, int x, int y, int horizontalSpacing, int verticalSpacing) {
    if (!node) return;

    QFontMetrics metrics(painter.font());
    QString text;

    if(node->get().type !="")
      text=QString::fromStdString(node->get().value + "\n" +node->get().type);
    else
      text=QString::fromStdString(node->get().value);


    int textWidth = metrics.horizontalAdvance(text);
    int textHeight = metrics.height();
    int padding = 10;

    int nodeWidth = textWidth + 2 * padding;
    int nodeHeight = textHeight + 2 * padding;

    // Draw the current node
    QRect nodeRect(x - nodeWidth / 2, y - nodeHeight / 2, nodeWidth, nodeHeight);
    painter.setBrush(Qt::lightGray);
    painter.setPen(Qt::black);

    if(node->get().value == "IF" ||
        node->get().value == "read" ||
        node->get().value == "ASSIGN" ||
        node->get().value == "REPEAT" ||
        node->get().value == "WRITE"){
        painter.drawRect(nodeRect);
    }
    else
    {
        painter.drawEllipse(nodeRect);
    }
    painter.drawText(nodeRect, Qt::AlignCenter, text);
    // Calculate total width for all children
    int totalWidth = calculateTotalWidth(node, horizontalSpacing);
    int childX = x - totalWidth / 2;  // Start from the leftmost position for children
    int childY = y + verticalSpacing;

    // Draw lines to children and recursively draw them
    for (auto child : node->getChildren()) {
        int childSubtreeWidth = calculateTotalWidth(child, horizontalSpacing);
        int childCenterX = childX + childSubtreeWidth / 2;

        // Draw the line to the child
        painter.setPen(Qt::white);
        painter.drawLine(x, y + nodeHeight / 2, childCenterX, childY - nodeHeight / 2);

        // Draw the child recursively
        drawTree(painter, child, childCenterX, childY, horizontalSpacing, verticalSpacing);

        // Move to the next position for the next child
        childX += childSubtreeWidth + horizontalSpacing;
    }

    // Handle siblings (the next pointer)
    if (node->getSibling()) {
        // Calculate the position of the sibling node
        int siblingX = x + nodeWidth + horizontalSpacing;  // Place the sibling directly to the right
        int siblingY = y;  // Same vertical level as the current node

        // Draw the line to the sibling
        painter.setPen(Qt::white);
        painter.drawLine(x + nodeWidth / 2, y, siblingX, siblingY);

        // Draw the sibling recursively
        drawTree(painter, node->getSibling(), siblingX, siblingY, horizontalSpacing, verticalSpacing);
    }
}

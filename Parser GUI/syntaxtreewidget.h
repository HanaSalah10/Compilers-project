#ifndef SYNTAXTREEWIDGET_H
#define SYNTAXTREEWIDGET_H

#include <QWidget>
#include<QPainter>
#include "node.h"

class SyntaxTreeWidget:public QWidget
{
    Q_OBJECT
public:
    explicit SyntaxTreeWidget(Node* root,QWidget *parent = nullptr);

private:
    Node* m_root = nullptr;
    int calculateTotalWidth(Node* node, int horizontalSpacing);
    void drawTree(QPainter& painter, Node* node, int x, int y, int horizontalSpacing, int verticalSpacing);
protected:
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);  // Smooth rendering

        drawTree(painter, m_root, width() / 3, 40, 50,100);  // Start from the center and top
    }
};

#endif // SYNTAXTREEWIDGET_H

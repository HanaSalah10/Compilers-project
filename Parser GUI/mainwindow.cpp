#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QGraphicsScene *scene = new QGraphicsScene(this);  // Scene for holding items
    QGraphicsView *view = new QGraphicsView(scene, this);  // View to display the scene

    // Create a rectangle and add it to the scene
    QGraphicsRectItem *rect = scene->addRect(80, 120, 200, 100);

    // Set up the layout and add the view to the layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);

    // Set the layout to the central widget of the main window
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Set window properties
    setWindowTitle("Syntax Tree Viewer");
    resize(600, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.drawRect(QRect(80,120,200,100));
}

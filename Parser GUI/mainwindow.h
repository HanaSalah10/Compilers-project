#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QHBoxLayout>
#include <vector>
#include <string>
#include "parser.h"
#include "scanner.h"
#include "syntaxtreewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBrowseFile();

private:
    Ui::MainWindow *ui;
    SyntaxTreeWidget *tree = nullptr;
    QWidget *treePlaceholder = nullptr;
    QVBoxLayout *layout ;
    QScrollArea * scrollArea;

    void processCode(const std::string &code);
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set window properties
    setWindowTitle("Syntax Tree Viewer");
    resize(800, 600);

    // Create a button to browse files
    QPushButton *browseButton = new QPushButton("Browse File", this);
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseFile);

    // Create a layout and add the button
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(browseButton);

    // Set the layout to the central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBrowseFile()
{
    // Open a file dialog to choose the input file
    QString fileName = QFileDialog::getOpenFileName(this, "Select a File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) {
        return; // User canceled the dialog
    }

    // Read the file content
    std::ifstream inputFile(fileName.toStdString());
    if (!inputFile.is_open()) {
        QMessageBox::critical(this, "Error", "Could not open the selected file.");
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string code = buffer.str();

    // Call the parsing logic (integrating your existing functions)
    processCode(code);
}

void MainWindow::processCode(const std::string &code)
{
    // Tokenize the input source code
    std::vector<Token> tokens = tokenize(code);
    if (tokens.empty()) {
        QMessageBox::critical(this, "Error", "No tokens found in the input file.");
        return;
    }

    // Convert tokens to lists for the parser
    std::vector<std::string> tokens_list;
    std::vector<std::string> code_list;
    for (const auto &token : tokens) {
        tokens_list.push_back(token.type);
        code_list.push_back(token.value);
    }

    // Initialize and run the parser
    Parser parser;
    parser.set_tokens_list_and_code_list(tokens_list, code_list);
    Node *parse_tree = parser.stmt_sequence();
    if (!parse_tree) {
        QMessageBox::critical(this, "Error", "Failed to generate parse tree.");
        return;
    }

    // Display the syntax tree using your existing logic
    SyntaxTreeWidget *widget = new SyntaxTreeWidget(parse_tree);
    widget->show();
}

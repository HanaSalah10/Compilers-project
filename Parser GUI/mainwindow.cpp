#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tree(nullptr) // Initialize tree pointer
{
    ui->setupUi(this);

    // Set window properties
    setWindowTitle("Syntax Tree Viewer");
    resize(800, 600);
    QHBoxLayout *fileLayout= new QHBoxLayout();
    QLabel *fileLabel = new QLabel("File", this);
    QPushButton *browseButton = new QPushButton("Browse File", this);
    browseButton->setFixedSize(80,25);

    fileLayout->setSpacing(10);

    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(browseButton);
    fileLayout->addStretch(0);


    QGroupBox *groupBox = new QGroupBox("Syntax Tree", this);
    groupBox->setAlignment(Qt::AlignLeft);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true); // Allow the widget to resize within the scroll area

    treePlaceholder = new QWidget(this);
    scrollArea->setWidget(treePlaceholder);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout();
    groupBoxLayout->addWidget(scrollArea);
    groupBox->setLayout(groupBoxLayout);

    // Main layout
    layout = new QVBoxLayout();
    layout->addLayout(fileLayout);
    layout->addWidget(groupBox);


    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

     connect(browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseFile);
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
    try {
        // Tokenize the input source code
        std::vector<Token> tokens = tokenize(code); // This might throw an exception

        if (tokens.empty()) {
            throw std::runtime_error("No tokens found in the input file. Please select a valid file.");
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
            throw std::runtime_error("Failed to generate parse tree.");
        }

        // Remove the old tree widget if it exists
        if (tree) {
            scrollArea->takeWidget(); // Detach the old widget
            tree->deleteLater();
        }

        // Add the new tree widget
        tree = new SyntaxTreeWidget(parse_tree, this);
        scrollArea->setWidget(tree);

    } catch (const std::runtime_error &e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Unknown Error", "An unexpected error occurred.");
    }
}


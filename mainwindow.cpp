#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sudokuboard.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->board = new SudokuBoard(this);
    this->ui->verticalLayout->addWidget(this->board);
}

MainWindow::~MainWindow()
{
    delete this->ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

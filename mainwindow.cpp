#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sudokuboard.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->board = new SudokuBoard(this);
    this->ui->verticalLayout->addWidget(this->board);
    connect(this->board, SIGNAL(Clicked(int, int)), this, SLOT(OnClick(int, int)));
}

MainWindow::~MainWindow()
{
    delete this->ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::OnClick(int row, int col)
{
    this->ui->statusbar->showMessage("Selected row " + QString::number(row) + " col " + QString::number(col));
}

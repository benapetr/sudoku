#include "mainwindow.h"
#include "global.h"
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

QString ErrorToString(int error)
{
    switch(error)
    {
        case E_ALREADY_USED:
            return "Value is already used";
        case E_NOT_EMPTY:
            return "This item already has a value";
        case E_INVALID_COL:
            return "Invalid column";
        case E_INVALID_ROW:
            return "Invalid row";
        case E_INVALID_VALUE:
            return "Invalid value";
    }
    return "Unknown error (" + QString::number(error) + ")";
}

void MainWindow::SetValue(int value, bool read_only)
{
    if (this->board->SelectedCol == 0 || this->board->SelectedRow == 0)
        return;

    int result = this->board->SetValue(value, read_only);
    if (result > 1)
    {
        this->ui->statusbar->showMessage(ErrorToString(result));
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::OnClick(int row, int col)
{
    this->ui->statusbar->showMessage("Selected row " + QString::number(row) + " col " + QString::number(col));
}

void MainWindow::on_pushButton1_clicked()
{
    this->SetValue(1);
}

void MainWindow::on_pushButton2_clicked()
{
    this->SetValue(2);
}

void MainWindow::on_pushButton3_clicked()
{
    this->SetValue(3);
}

void MainWindow::on_pushButton4_clicked()
{
    this->SetValue(4);
}

void MainWindow::on_pushButton5_clicked()
{
    this->SetValue(5);
}

void MainWindow::on_pushButton6_clicked()
{
    this->SetValue(6);
}

void MainWindow::on_pushButton7_clicked()
{
    this->SetValue(7);
}

void MainWindow::on_pushButton8_clicked()
{
    this->SetValue(8);
}

void MainWindow::on_pushButton9_clicked()
{
    this->SetValue(9);
}

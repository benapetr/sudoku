// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sudokuboard.h"
#include "commandprocessor.h"
#include "errors.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->labelMode = new QLabel(this);
    this->labelStatus = new QLabel(this);
    this->ui->statusbar->addWidget(this->labelMode);
    this->ui->statusbar->addWidget(this->labelStatus);
    this->board = nullptr;
    this->New();
}

MainWindow::~MainWindow()
{
    delete this->ui;
}

void MainWindow::New()
{
    this->ui->verticalLayout->removeWidget(this->board);
    delete this->board;
    this->board = new SudokuBoard(this);
    this->ui->verticalLayout->addWidget(this->board);
    connect(this->board, SIGNAL(Clicked(int, int)), this, SLOT(OnClick(int, int)));
    this->SwitchMode(GameMode_Editor);
}

void MainWindow::SetValue(int value)
{
    if (this->board->SelectedCol == 0 || this->board->SelectedRow == 0)
        return;

    int result;

    if (this->gameMode == GameMode_Player && this->ui->checkBox->isChecked())
    {
        result = this->board->SetValueHint(value);
    } else
    {
        result = this->board->SetValue(value, this->gameMode == GameMode_Editor);
    }

    if (result > 1)
    {
        this->UpdateStatus(Errors::ToString(result));
    }
}

void MainWindow::UpdateMode(QString mode)
{
    this->labelMode->setText("Mode: " + mode);
}

void MainWindow::UpdateStatus(QString tx)
{
    this->labelStatus->setText(tx);
}

void MainWindow::SwitchMode(GameMode mode)
{
    this->gameMode = mode;
    switch (mode)
    {
        case GameMode_Editor:
            this->UpdateMode("editor");
            this->ui->checkBox->hide();
            this->ui->pushButton_PlayGame->show();
            this->ui->labelInfo->show();
            break;
        case GameMode_Player:
            this->ui->pushButton_PlayGame->hide();
            this->UpdateMode("game");
            this->ui->checkBox->show();
            this->ui->labelInfo->hide();
            break;
    }
}

bool MainWindow::Save(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return false;
    }

    file.write(this->board->ExportToCommandList().toUtf8());
    file.close();
    return true;
}

void MainWindow::on_actionExit_triggered()
{
    if (!this->NotifyChanges())
        return;
    QApplication::exit();
}

void MainWindow::OnClick(int row, int col)
{
    this->UpdateStatus("Selected row " + QString::number(row) + " col " + QString::number(col));
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

void MainWindow::on_pushButton_Wipe_clicked()
{
    int result = this->board->ClearValue(this->gameMode == GameMode_Editor);
    if (result > 1)
    {
        this->UpdateStatus(Errors::ToString(result));
    }
}

void MainWindow::on_pushButton_PlayGame_clicked()
{
    if (this->gameMode != GameMode_Editor)
        return;

    this->SwitchMode(GameMode_Player);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save as", "", "Sudoku command batch file (*.scb);;All Files (*)");
    if (path.isEmpty())
        return;
    if (this->Save(path))
        this->currentFile = path;
}

void MainWindow::on_actionNew_triggered()
{
    if (!this->NotifyChanges())
        return;

    // Start a new game
    this->New();
}

void MainWindow::on_actionLoad_triggered()
{
    if (!this->NotifyChanges())
        return;

    QString path = QFileDialog::getOpenFileName(this, "Open sudoku", "", "Sudoku command batch file (*.scb);;All Files (*)");
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QString input_text = file.readAll();
    file.close();

    this->New();

    CommandProcessor cp(this->board);
    if (!cp.ProcessText(input_text))
    {
        QMessageBox::warning(this, "Unable to load file", "File can't be processed, error at line " + QString::number(cp.LastLine) + ": " + cp.LastError);
        return;
    }

    if (cp.GetGM() != this->gameMode)
        this->SwitchMode(cp.GetGM());

    this->currentFile = path;
}

void MainWindow::on_actionSave_triggered()
{
    if (this->currentFile.isEmpty())
    {
        this->on_actionSave_as_triggered();
        return;
    }
    this->Save(this->currentFile);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!this->NotifyChanges())
        event->ignore();
    else
        event->accept();
}

bool MainWindow::NotifyChanges()
{
    if (this->board->IsModified())
    {
        if (QMessageBox::question(this, "Are you sure?", "Sudoku has unsaved changes, are you sure you want to drop it without saving?") == QMessageBox::StandardButton::No)
            return false;
    }
    return true;
}


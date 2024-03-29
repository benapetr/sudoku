// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sudokuboard.h"
#include "commandprocessor.h"
#include "about.h"
#include "options.h"
#include "recursivesolver.h"
#include "errors.h"
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    Options::Initialize();
    this->ui->setupUi(this);
    this->ui->actionAutoload_last_file->setChecked(Options::Autoload());
    this->ui->actionRemove_hints_for_same_number_as_entered->setChecked(Options::GetRemoveHints());
    this->ui->actionAssisted_mode_informs_you_immediatelly_on_mistake->setChecked(Options::GetAssistedMode());
    this->labelMode = new QLabel(this);
    this->labelStatus = new QLabel(this);
    this->ui->statusbar->addWidget(this->labelMode);
    this->ui->statusbar->addWidget(this->labelStatus);
    this->board = nullptr;
    this->New();
    if (Options::Autoload() && !Options::LastFile().isEmpty())
    {
        this->Load(Options::LastFile());
    }
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
    this->currentFile = "";
    this->UpdateTitle();
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
        if (result == E_WRONG_VALUE)
        {
            QMessageBox::information(this, "Mistake", "This is a wrong value");
        }
        this->UpdateStatus(Errors::ToString(result));
    } else
    {
        this->UpdateTitle();
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
            this->ui->actionTry_to_make_sudoku_harder->setEnabled(true);
            this->ui->pushButton_PlayGame->show();
            this->ui->labelInfo->show();
            break;
        case GameMode_Player:
            this->ui->pushButton_PlayGame->hide();
            this->UpdateMode("game");
            this->ui->actionTry_to_make_sudoku_harder->setEnabled(false);
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
    this->currentFile = path;
    this->UpdateTitle();
    Options::SetLastDir(QFileInfo(path).absoluteDir().absolutePath());
    return true;
}

bool MainWindow::Load(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return false;
    }

    QString input_text = file.readAll();
    file.close();

    this->New();

    this->board->TemporarilyDisableAssistedMode = true;
    CommandProcessor cp(this->board);
    if (!cp.ProcessText(input_text))
    {
        this->board->TemporarilyDisableAssistedMode = false;
        QMessageBox::warning(this, "Unable to load file", "File (" + path + ") can't be processed, error at line " + QString::number(cp.LastLine) + ": " + cp.LastError);
        return false;
    }
    this->board->TemporarilyDisableAssistedMode = false;

    if (cp.GetGM() != this->gameMode)
        this->SwitchMode(cp.GetGM());

    this->currentFile = path;
    this->board->ResetChange();
    this->UpdateTitle();
    Options::SetLastDir(QFileInfo(path).absoluteDir().absolutePath());
    return true;
}

void MainWindow::UpdateTitle()
{
    QString title;
    if (this->currentFile.isEmpty())
        title = "SudokuPro [unsaved]";
    else
        title = "SudokuPro [" + this->currentFile + "]";

    if (this->board->IsModified())
        title += "*";

    this->setWindowTitle(title);
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
    this->UpdateTitle();
}

void MainWindow::on_pushButton_PlayGame_clicked()
{
    if (this->gameMode != GameMode_Editor)
        return;

    this->SwitchMode(GameMode_Player);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save as", Options::GetLastDir(), "Sudoku command batch file (*.scb);;All Files (*)");
    if (path.isEmpty())
        return;
    if (this->Save(path))
    {
        if (Options::Autoload())
            Options::SetLastFile(path);
    }
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

    QString path = QFileDialog::getOpenFileName(this, "Open sudoku", Options::GetLastDir(), "Sudoku command batch file (*.scb);;All Files (*)");
    if (path.isEmpty())
        return;

    if (!this->Load(path))
        return;

    if (Options::Autoload())
        Options::SetLastFile(path);
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

void MainWindow::on_actionRemove_hints_for_same_number_as_entered_triggered()
{
    Options::SetRemoveHints(this->ui->actionRemove_hints_for_same_number_as_entered->isChecked());
}

void MainWindow::on_actionAutoload_last_file_triggered()
{
    Options::SetAutoload(this->ui->actionAutoload_last_file->isChecked());
}

void MainWindow::on_actionFlag_invalid_hints_triggered()
{
    this->board->FlagInvalidHints();
}

void MainWindow::on_actionFind_all_hints_triggered()
{
    this->UpdateStatus("New hints: " + QString::number(this->board->FindAllHints()));
}

void MainWindow::on_actionFind_hints_triggered()
{
    if (!this->board->FindHint())
        this->UpdateStatus("No hints found");
}

void MainWindow::on_actionSolve_recursively_triggered()
{
    int result = this->board->SolveRecursively();
    if (result != SUCCESS)
    {
        this->UpdateStatus("Unable to resolve: " + Errors::ToString(result));
    }
}

void MainWindow::on_actionCount_solutions_triggered()
{
    int result = this->board->CountSolutions();
    if (result > 100)
    {
        QMessageBox::information(this, "Solutions found", "This sudoku has more than 100 solutions, search was aborted to prevent lock up");
    } else
    {
        QMessageBox::information(this, "Solutions found", "This sudoku has " + QString::number(result) + " solutions");
    }
}

void MainWindow::on_actionRemove_all_hints_triggered()
{
    this->board->RemoveAllHints();
}

void MainWindow::on_actionAssisted_mode_informs_you_immediatelly_on_mistake_triggered()
{
    Options::SetAssistedMode(this->ui->actionAssisted_mode_informs_you_immediatelly_on_mistake->isChecked());
}

void MainWindow::on_actionNew_random_triggered()
{
    if (!this->NotifyChanges())
        return;

    // Start a new game
    this->New();
    QHash<int, QList<int>> sudoku = RecursiveSolver::GenerateRandom();
    int row, col;
    row = 0;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            if (sudoku[row][col] != 0)
            {
                this->board->SetValue(row, col, sudoku[row][col], true);
            }
            col++;
        }
        row++;
    }
}

void MainWindow::on_actionTry_to_make_sudoku_harder_triggered()
{
    if (this->gameMode != GameMode_Editor)
    {
        return;
    }

    QHash<int, QList<int>> sudoku = this->board->GetSudoku();
    if (RecursiveSolver::HasMoreThanOne(sudoku))
    {
        QMessageBox::warning(this, "Error", "This sudoku already has more than 1 solution");
        return;
    }
    QHash<int, QList<int>> original_sudoku = sudoku;
    RecursiveSolver::MakeHard(&sudoku);
    int row, col;
    int removed_clues = 0;
    row = 0;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            if (original_sudoku[row][col] != 0 && sudoku[row][col] == 0)
            {
                this->board->ClearValue(row, col, true);
                removed_clues++;
            }
            col++;
        }
        row++;
    }
    this->UpdateStatus("Removed: " + QString::number(removed_clues));
}

void MainWindow::on_actionAbout_triggered()
{
    About form;
    form.exec();
}

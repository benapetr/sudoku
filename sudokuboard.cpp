// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "sudokuboard.h"
#include "sudokuboxwidget.h"
#include "sudokuitemwidget.h"
#include "ui_sudokuboard.h"
#include "global.h"
#include <QDateTime>
SudokuBoard::SudokuBoard(QWidget *parent) : QFrame(parent), ui(new Ui::SudokuBoard)
{
    this->ui->setupUi(this);
    this->populate();
}

SudokuBoard::~SudokuBoard()
{
    delete this->ui;
}

bool SudokuBoard::IsModified()
{
    return this->isModified;
}

int GetBoxID(int p)
{
    switch (p)
    {
        case 0:
        case 1:
        case 2:
            return 0;
        case 3:
        case 4:
        case 5:
            return 1;
        case 6:
        case 7:
        case 8:
            return 2;
    }
    return -1;
}

int SudokuBoard::SetValue(int row, int col, unsigned int value, bool read_only)
{
    if (row < 0 || row > 8)
        return E_INVALID_ROW;
    if (col < 0 || col > 8)
        return E_INVALID_COL;

    if (value > 0)
    {
        // Check hints to detect if this value is already in some row or column, if yes, we might want to find where
        // this is much faster than traversing the actual box structure
        int c = 0;
        while (c < 9)
        {
            if (this->valueHint[row][c++] == value)
                return E_ALREADY_USED;
        }
        // Same with whole column
        int r = 0;
        while (r < 9)
        {
            if (this->valueHint[r++][col] == value)
                return E_ALREADY_USED;
        }
    }

    int boxr = GetBoxID(row);
    int boxc = GetBoxID(col);
    int result = this->boxes[boxr][boxc]->SetValue(row - (3 * boxr), col - (3 * boxc), value, read_only);
    if (result == SUCCESS)
    {
        this->valueHint[row][col] = value;
        this->isModified = true;
    }
    return result;
}

int SudokuBoard::SetValue(unsigned int value, bool read_only)
{
    return this->SetValue(this->SelectedRow-1, this->SelectedCol-1, value, read_only);
}

int SudokuBoard::ClearValue(int row, int col, bool read_only)
{
    if (row < 0 || row > 8)
        return E_INVALID_ROW;
    if (col < 0 || col > 8)
        return E_INVALID_COL;

    int boxr = GetBoxID(row);
    int boxc = GetBoxID(col);
    int result = this->boxes[boxr][boxc]->ClearValue(row - (3 * boxr), col - (3 * boxc), read_only);
    if (result == SUCCESS)
    {
        this->isModified = true;
        this->valueHint[row][col] = 0;
    }
    return result;
}

int SudokuBoard::ClearValue(bool read_only)
{
    return this->ClearValue(this->SelectedRow - 1, this->SelectedCol - 1, read_only);
}

int SudokuBoard::SetValueHint(int row, int col, unsigned int value)
{
    if (row < 0 || row > 8)
        return E_INVALID_ROW;
    if (col < 0 || col > 8)
        return E_INVALID_COL;

    int boxr = GetBoxID(row);
    int boxc = GetBoxID(col);
    int result = this->boxes[boxr][boxc]->SetValueHint(row - (3 * boxr), col - (3 * boxc), value);
    if (result == SUCCESS)
    {
        this->isModified = true;
    }
    return result;
}

int SudokuBoard::SetValueHint(unsigned int value)
{
    return this->SetValueHint(this->SelectedRow-1, this->SelectedCol-1, value);
}

QString SudokuBoard::ExportToCommandList()
{
    QString result = "# Export of SudokuPro version " + QString(APP_VERSION) + "\n";
    result += "# This file contains a list of commands that can be issued to reconstruct this sudoku:\n\n";
    int row = 0;
    int col;
    while (row < 9)
    {
        result += "# ";
        col = 0;
        while (col < 9)
        {
            QString value;
            if (this->valueHint[row][col] == 0)
                value = " ";
            else
                value = QString::number(this->valueHint[row][col]);

            result += value;
            col++;
        }
        result += "\n";
        row++;
    }

    result += "\nSwitchMode editor\n";
    foreach (SudokuBoxWidget *box, this->allBoxes)
    {
        QList<SudokuItemWidget *> items = box->GetItems();
        foreach (SudokuItemWidget *item, items)
        {
            if (item->ReadOnly)
            {
                result += "SetValue " + QString::number(item->HintBoxPosRow + (box->HintRow * 3) + 1) +
                        " " + QString::number(item->HintBoxPosCol + (box->HintCol * 3) + 1) +
                        " " + QString::number(item->GetValue()) + "\n";
            }
        }
    }

    result += "\nSwitchMode game\n";

    foreach (SudokuBoxWidget *box, this->allBoxes)
    {
        QList<SudokuItemWidget *> items = box->GetItems();
        foreach (SudokuItemWidget *item, items)
        {
            if (!item->ReadOnly && !item->IsEmpty())
            {
                result += "SetValue " + QString::number(item->HintBoxPosRow + (box->HintRow * 3) + 1) +
                        " " + QString::number(item->HintBoxPosCol + (box->HintCol * 3) + 1) +
                        " " + QString::number(item->GetValue()) + "\n";
            } else if (item->GetCurrentViewMode() == SudokuItemWidget_ViewMode_Hint)
            {
                unsigned int n = 1;
                while (n < 10)
                {
                    if (item->GetValueHint(n++))
                    {
                        result += "SetHint " + QString::number(item->HintBoxPosRow + (box->HintRow * 3) + 1) +
                                    " " + QString::number(item->HintBoxPosCol + (box->HintCol * 3) + 1) +
                                    " " + QString::number(n) + "\n";
                    }
                }
            }
        }
    }

    result += "# Finished at " + QDateTime::currentDateTime().toString() + "\n\n";
    this->isModified = false;
    return result;
}

void SudokuBoard::ResetChange()
{
    this->isModified = false;
}

void SudokuBoard::OnClick(int brow, int bcol, int row, int col)
{
    this->SelectedCol = (bcol * 3) + col + 1;
    this->SelectedRow = (brow * 3) + row + 1;
    emit Clicked(this->SelectedRow, this->SelectedCol);
}

void SudokuBoard::populate()
{
    qDeleteAll(this->allBoxes);
    // Initialize boxes
    int row = 0;
    int col;
    while (row < 3)
    {
        col = 0;
        while (col < 3)
        {
            SudokuBoxWidget *bw = new SudokuBoxWidget(this);
            this->boxes[row][col] = bw;
            this->ui->gridLayout->addWidget(bw, row, col);
            this->allBoxes.append(bw);
            bw->HintCol = col;
            bw->HintRow = row;
            connect(bw, SIGNAL(Clicked(int, int, int, int)), this, SLOT(OnClick(int, int, int, int)));
            col++;
        }
        row++;
    }

    // Reset hint
    row = 0;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            this->valueHint[row][col++] = 0;
        }
        row++;
    }
}

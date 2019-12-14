#include "sudokuboard.h"
#include "sudokuboxwidget.h"
#include "ui_sudokuboard.h"
#include "global.h"

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
    foreach (SudokuBoxWidget *box, this->allBoxes)
    {
        if (box->IsModified())
            return true;
    }
    return false;
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

    int boxr = GetBoxID(row);
    int boxc = GetBoxID(col);
    return this->boxes[boxr][boxc]->SetValue(row - (3 * boxr), col - (3 * boxc), value, read_only);
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
            this->boxes[row][col] = new SudokuBoxWidget(this);
            this->ui->gridLayout->addWidget(this->boxes[row][col], row, col);
            this->allBoxes.append(this->boxes[row][col]);
            this->boxes[row][col]->HintCol = col;
            this->boxes[row][col]->HintRow = row;
            col++;
        }
        row++;
    }
}

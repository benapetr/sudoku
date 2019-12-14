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

    if (value > 1)
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
        this->valueHint[row][col] = value;
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
        this->valueHint[row][col] = 0;
    return result;
}

int SudokuBoard::ClearValue(bool read_only)
{
    return this->ClearValue(this->SelectedRow - 1, this->SelectedCol - 1, read_only);
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
            this->valueHint[row][col] = 0;
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
}

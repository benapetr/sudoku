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

int SudokuBoard::SetValue(unsigned int value, bool read_only)
{
    return this->SetValue(this->SelectedRow-1, this->SelectedCol-1, value, read_only);
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
}

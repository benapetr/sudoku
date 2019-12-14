#include "sudokuboxwidget.h"
#include "sudokuitemwidget.h"
#include "ui_sudokuboxwidget.h"
#include "global.h"

SudokuBoxWidget::SudokuBoxWidget(QWidget *parent) : QFrame(parent), ui(new Ui::SudokuBoxWidget)
{
    this->ui->setupUi(this);
    this->populate();
}

SudokuBoxWidget::~SudokuBoxWidget()
{
    delete this->ui;
}

bool SudokuBoxWidget::IsModified()
{
    return this->isModified;
}

int SudokuBoxWidget::SetValue(int row, int col, unsigned int value, bool read_only)
{
    if (row < 0 || row > 2)
        return E_INVALID_ROW;
    if (col < 0 || col > 2)
        return E_INVALID_COL;
    return this->items[row][col]->SetValue(value, read_only);
}

void SudokuBoxWidget::populate()
{
    int row = 0;
    int col;
    while (row < 3)
    {
        col = 0;
        while (col < 3)
        {
            SudokuItemWidget *item = new SudokuItemWidget(this);
            this->items[row][col] = item;
            item->HintBoxPosCol = col;
            item->HintBoxPosRow = row;
            this->ui->gridLayout->addWidget(item, row, col);
            col++;
        }
        row++;
    }
}

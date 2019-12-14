// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "sudokuboxwidget.h"
#include "sudokuitemwidget.h"
#include "options.h"
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
    if (value < 1 || value > 9)
        return E_INVALID_VALUE;
    if (this->isUsed[value])
        return E_ALREADY_USED;
    this->items[row][col]->SwitchView(SudokuItemWidget_ViewMode_Value);
    int result = this->items[row][col]->SetValue(value, read_only);
    if (result == 0)
    {
        this->isUsed[value] = true;
        // if this is not inserted in editor mode, check if there aren't some hints to get rid of
        if (!read_only && Options::GetRemoveHints())
        {
            foreach (SudokuItemWidget *item, this->allItems)
            {
                if (item->GetCurrentViewMode() == SudokuItemWidget_ViewMode_Hint && item->GetValueHint(value))
                    item->UnsetValueHint(value);
            }
        }
    }
    return result;
}

int SudokuBoxWidget::ClearValue(int row, int col, bool read_only)
{
    unsigned int value = this->items[row][col]->GetValue();
    int result = this->items[row][col]->SetValue(0, read_only);
    if (result == SUCCESS)
    {
        if (value >= 1 && value <= 9)
            this->isUsed[value] = false;
        return SUCCESS;
    } else
    {
        return result;
    }
}

int SudokuBoxWidget::SetValueHint(int row, int col, unsigned int value)
{
    if (row < 0 || row > 2)
        return E_INVALID_ROW;
    if (col < 0 || col > 2)
        return E_INVALID_COL;
    if (value < 1 || value > 9)
        return E_INVALID_VALUE;
    if (!this->items[row][col]->IsEmpty())
        return E_NOT_EMPTY;

    int result;

    this->items[row][col]->SwitchView(SudokuItemWidget_ViewMode_Hint);

    // Check if this hint is already present, if not, unset it
    if (!this->items[row][col]->GetValueHint(value))
        result = this->items[row][col]->SetValueHint(value);
    else
        result = this->items[row][col]->UnsetValueHint(value);

    return result;
}

bool SudokuBoxWidget::IsUsed(unsigned int value)
{
    return this->isUsed[value];
}

SudokuItemWidget *SudokuBoxWidget::GetItem(int row, int col)
{
    return this->items[row][col];
}

QList<SudokuItemWidget *> SudokuBoxWidget::GetItems()
{
    return this->allItems;
}

void SudokuBoxWidget::OnClicked(int row, int col)
{
    emit Clicked(this->HintRow, this->HintCol, row, col);
}

void SudokuBoxWidget::populate()
{
    int n = 0;
    while (n < 10)
        this->isUsed[n++] = false;
    int row = 0;
    int col;
    while (row < 3)
    {
        col = 0;
        while (col < 3)
        {
            SudokuItemWidget *item = new SudokuItemWidget(this);
            connect(item, SIGNAL(Clicked(int, int)), this, SLOT(OnClicked(int, int)));
            this->items[row][col] = item;
            this->allItems.append(item);
            item->HintBoxPosCol = col;
            item->HintBoxPosRow = row;
            this->ui->gridLayout->addWidget(item, row, col);
            col++;
        }
        row++;
    }
}

#include "sudokuitemwidget.h"
#include "global.h"
#include "ui_sudokuitemwidget.h"

SudokuItemWidget::SudokuItemWidget(QWidget *parent) : QFrame(parent), ui(new Ui::SudokuItemWidget)
{
    this->ui->setupUi(this);
    this->SetValue(0);
    this->RenderValue();
}

SudokuItemWidget::~SudokuItemWidget()
{
    delete this->ui;
}

int SudokuItemWidget::SetValue(unsigned int new_value, bool read_only)
{
    if (this->value != 0 && new_value > 0)
        return E_NOT_EMPTY;
    if (this->value == new_value)
        return E_NO_CHANGE;
    if (new_value > 9)
        return E_INVALID_VALUE;
    this->value = new_value;
    this->ReadOnly = read_only;
    this->RenderValue();
    return SUCCESS;
}

void SudokuItemWidget::RenderValue()
{
    if (this->value == 0)
    {
        this->ui->pushButton->setText(" ");
        return;
    }
    this->ui->pushButton->setText(QString::number(this->value));
}

unsigned int SudokuItemWidget::GetValue()
{
    return this->value;
}

void SudokuItemWidget::on_pushButton_clicked()
{
    emit Clicked(this->HintBoxPosRow, this->HintBoxPosCol);
}

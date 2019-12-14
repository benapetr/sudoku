#include "sudokuitemwidget.h"
#include "ui_sudokuitemwidget.h"

SudokuItemWidget::SudokuItemWidget(QWidget *parent) : QFrame(parent), ui(new Ui::SudokuItemWidget)
{
    this->ui->setupUi(this);
    this->SetValue(0);
}

SudokuItemWidget::~SudokuItemWidget()
{
    delete this->ui;
}

int SudokuItemWidget::SetValue(unsigned int new_value, bool read_only)
{
    if (this->value == new_value)
        return 1;
    if (new_value > 9)
        return 2;
    this->value = new_value;
    this->ReadOnly = read_only;
    return 0;
}

void SudokuItemWidget::RenderValue()
{
    if (this->value == 0)
    {
        this->ui->pushButton->setText("N");
        return;
    }
    this->ui->pushButton->setText(QString::number(this->value));
}

void SudokuItemWidget::on_pushButton_clicked()
{
    emit Clicked();
}

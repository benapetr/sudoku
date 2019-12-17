// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include <QMouseEvent>
#include "sudokuitemwidget.h"
#include "global.h"
#include "ui_sudokuitemwidget.h"

SudokuItemWidget::SudokuItemWidget(QWidget *parent) : QFrame(parent), ui(new Ui::SudokuItemWidget)
{
    this->ui->setupUi(this);
    this->Reset();
    this->SwitchView(SudokuItemWidget_ViewMode_Value);
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
    if (this->ReadOnly && !read_only)
        return E_READ_ONLY;
    this->value = new_value;
    if (value == 0)
        this->ReadOnly = false;
    else
        this->ReadOnly = read_only;
    this->RenderValue();
    this->UpdateColors();
    return SUCCESS;
}

int SudokuItemWidget::SetValueHint(unsigned int hint_value)
{
    QPushButton *ptr = this->bFVH(hint_value);
    if (ptr == nullptr)
        return E_INVALID_VALUE;
    ptr->setText(QString::number(hint_value));
    ptr->setStyleSheet("color: rgb(0, 0, 0)");
    this->valueHint[hint_value] = true;
    return SUCCESS;
}

int SudokuItemWidget::UnsetValueHint(unsigned int hint_value)
{
    QPushButton *ptr = this->bFVH(hint_value);
    if (ptr == nullptr)
        return E_INVALID_VALUE;
    ptr->setText(" ");
    this->valueHint[hint_value] = false;
    return SUCCESS;
}

bool SudokuItemWidget::GetValueHint(unsigned int hint)
{
    return this->valueHint[hint];
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

bool SudokuItemWidget::IsEmpty()
{
    return this->value == 0;
}

unsigned int SudokuItemWidget::GetValue()
{
    return this->value;
}

void SudokuItemWidget::Reset()
{
    int n = 0;
    while (n < 10)
        this->valueHint[n++] = false;
    this->ui->pushButton_Hint1->setText(" ");
    this->ui->pushButton_Hint2->setText(" ");
    this->ui->pushButton_Hint3->setText(" ");
    this->ui->pushButton_Hint4->setText(" ");
    this->ui->pushButton_Hint5->setText(" ");
    this->ui->pushButton_Hint6->setText(" ");
    this->ui->pushButton_Hint7->setText(" ");
    this->ui->pushButton_Hint8->setText(" ");
    this->ui->pushButton_Hint9->setText(" ");
    this->SetValue(0);
    this->RenderValue();
}

void SudokuItemWidget::SwitchView(SudokuItemWidget_ViewMode mode)
{
    this->viewMode = mode;
    switch (mode)
    {
        case SudokuItemWidget_ViewMode_Hint:
            this->ui->pushButton->hide();
            this->ui->pushButton_Hint1->show();
            this->ui->pushButton_Hint2->show();
            this->ui->pushButton_Hint3->show();
            this->ui->pushButton_Hint4->show();
            this->ui->pushButton_Hint5->show();
            this->ui->pushButton_Hint6->show();
            this->ui->pushButton_Hint7->show();
            this->ui->pushButton_Hint8->show();
            this->ui->pushButton_Hint9->show();
            break;
        case SudokuItemWidget_ViewMode_Value:
            this->ui->pushButton->show();
            this->ui->pushButton_Hint1->hide();
            this->ui->pushButton_Hint2->hide();
            this->ui->pushButton_Hint3->hide();
            this->ui->pushButton_Hint4->hide();
            this->ui->pushButton_Hint5->hide();
            this->ui->pushButton_Hint6->hide();
            this->ui->pushButton_Hint7->hide();
            this->ui->pushButton_Hint8->hide();
            this->ui->pushButton_Hint9->hide();
            break;
    }
}

SudokuItemWidget_ViewMode SudokuItemWidget::GetCurrentViewMode()
{
    return this->viewMode;
}

void SudokuItemWidget::FlagInvalidHint(unsigned int value)
{
    QPushButton *ptr = this->bFVH(value);
    if (ptr == nullptr)
        return;
    ptr->setText(QString::number(value));
    ptr->setStyleSheet("color: rgb(255, 20, 0)");
}

void SudokuItemWidget::FlagValidHint(unsigned int value)
{
    QPushButton *ptr = this->bFVH(value);
    if (ptr == nullptr)
        return;
    ptr->setText(QString::number(value));
    this->valueHint[value] = true;
    ptr->setStyleSheet("color: rgb(0, 255, 0)");
}

void SudokuItemWidget::UpdateColors()
{
    if (this->ReadOnly)
        this->ui->pushButton->setStyleSheet("color: rgb(0, 0, 0)");
    else
        this->ui->pushButton->setStyleSheet("color: rgb(0, 60, 180)");
}

void SudokuItemWidget::on_pushButton_clicked()
{
    emit Clicked(this->HintBoxPosRow, this->HintBoxPosCol);
}

void SudokuItemWidget::on_pushButton_Hint2_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint1_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint3_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint4_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint5_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint6_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint7_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint8_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::on_pushButton_Hint9_clicked()
{
    this->on_pushButton_clicked();
}

void SudokuItemWidget::mousePressEvent(QMouseEvent *event)
{
    this->on_pushButton_clicked();
    event->accept();
    return;
}

QPushButton *SudokuItemWidget::bFVH(unsigned int value)
{
    switch(value)
    {
        case 1:
            return this->ui->pushButton_Hint1;
            break;
        case 2:
            return this->ui->pushButton_Hint2;
            break;
        case 3:
            return this->ui->pushButton_Hint3;
            break;
        case 4:
            return this->ui->pushButton_Hint4;
            break;
        case 5:
            return this->ui->pushButton_Hint5;
            break;
        case 6:
            return this->ui->pushButton_Hint6;
            break;
        case 7:
            return this->ui->pushButton_Hint7;
            break;
        case 8:
            return this->ui->pushButton_Hint8;
            break;
        case 9:
            return this->ui->pushButton_Hint9;
            break;
    }
    return nullptr;
}

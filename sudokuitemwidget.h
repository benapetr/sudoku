// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef SUDOKUITEMWIDGET_H
#define SUDOKUITEMWIDGET_H

#include <QFrame>
#include <QPushButton>

namespace Ui
{
    class SudokuItemWidget;
}

enum SudokuItemWidget_ViewMode
{
    SudokuItemWidget_ViewMode_Value,
    SudokuItemWidget_ViewMode_Hint
};

enum SudokuItemWidget_SelectionMode
{
    SudokuItemWidget_SelectionMode_None,
    SudokuItemWidget_SelectionMode_This,
    SudokuItemWidget_SelectionMode_Vector
};

class SudokuItemWidget : public QFrame
{
        Q_OBJECT
    public:
        explicit SudokuItemWidget(QWidget *parent = nullptr);
        ~SudokuItemWidget();
        //! Changes value of box
        int SetValue(unsigned int new_value, bool read_only = false);
        int SetValueHint(unsigned int hint_value);
        int UnsetValueHint(unsigned int hint_value);
        bool GetValueHint(unsigned int hint);
        void RenderValue();
        bool IsEmpty();
        unsigned int GetValue();
        void Reset();
        void SwitchView(SudokuItemWidget_ViewMode mode);
        SudokuItemWidget_ViewMode GetCurrentViewMode();
        void FlagInvalidHint(unsigned int value);
        void FlagValidHint(unsigned int value);
        void UpdateColors();
        int HintBoxPosRow;
        int HintBoxPosCol;
        bool ReadOnly = false;

    signals:
        void Clicked(int row, int col);

    private slots:
        void on_pushButton_clicked();
        void on_pushButton_Hint2_clicked();
        void on_pushButton_Hint1_clicked();
        void on_pushButton_Hint3_clicked();
        void on_pushButton_Hint4_clicked();
        void on_pushButton_Hint5_clicked();
        void on_pushButton_Hint6_clicked();
        void on_pushButton_Hint7_clicked();
        void on_pushButton_Hint8_clicked();
        void on_pushButton_Hint9_clicked();

    private:
        void mousePressEvent(QMouseEvent *event);
        QPushButton *bFVH(unsigned int value);
        SudokuItemWidget_SelectionMode selectionMode = SudokuItemWidget_SelectionMode_None;
        SudokuItemWidget_ViewMode viewMode = SudokuItemWidget_ViewMode_Value;
        bool valueHint[10];
        unsigned int value = 0;
        Ui::SudokuItemWidget *ui;
};

#endif // SUDOKUITEMWIDGET_H

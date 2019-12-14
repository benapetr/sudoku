#ifndef SUDOKUITEMWIDGET_H
#define SUDOKUITEMWIDGET_H

#include <QFrame>

namespace Ui
{
    class SudokuItemWidget;
}

class SudokuItemWidget : public QFrame
{
        Q_OBJECT
    public:
        explicit SudokuItemWidget(QWidget *parent = nullptr);
        ~SudokuItemWidget();
        //! Changes value of box
        int SetValue(unsigned int new_value, bool read_only = false);
        void RenderValue();
        unsigned int GetValue();
        int HintBoxPosRow;
        int HintBoxPosCol;
        bool ReadOnly = false;

    signals:
        void Clicked(int row, int col);

    private slots:
        void on_pushButton_clicked();

    private:
        unsigned int value = 0;
        Ui::SudokuItemWidget *ui;
};

#endif // SUDOKUITEMWIDGET_H

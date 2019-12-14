#ifndef SUDOKUBOXWIDGET_H
#define SUDOKUBOXWIDGET_H

#include <QFrame>

namespace Ui
{
    class SudokuBoxWidget;
}

class SudokuItemWidget;

class SudokuBoxWidget : public QFrame
{
        Q_OBJECT

    public:
        explicit SudokuBoxWidget(QWidget *parent = nullptr);
        ~SudokuBoxWidget();
        bool IsModified();
        //! Change value in box, returns
        //! 0 - success
        //! 1 - no change
        //! 2 - invalid value
        //! 3 - invalid row
        //! 4 - invalid column
        //! 5 - already used
        int SetValue(int row, int col, unsigned int value, bool read_only = false);
        int HintRow;
        int HintCol;

    private slots:
        void OnClicked(int row, int col);

    signals:
        void Clicked(int box_row, int box_col, int row, int col);

    private:
        void populate();
        bool isModified = false;
        Ui::SudokuBoxWidget *ui;
        SudokuItemWidget *items[3][3];
};

#endif // SUDOKUBOXWIDGET_H

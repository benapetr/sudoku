#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QFrame>
#include <QList>

namespace Ui
{
    class SudokuBoard;
}

class SudokuBoxWidget;
class SudokuItemWidget;

class SudokuBoard : public QFrame
{
        Q_OBJECT

    public:
        explicit SudokuBoard(QWidget *parent = nullptr);
        ~SudokuBoard();
        bool IsModified();
        //! Change value in box, returns
        //! 0 - success
        //! 1 - no change
        //! 2 - invalid value
        //! 3 - invalid row
        //! 4 - invalid column
        //! 5 - already used
        int SetValue(int row, int col, unsigned int value, bool read_only = false);
        int SelectedRow = 0;
        int SelectedCol = 0;

    signals:
        void Clicked(int row, int col);

    private slots:
        void OnClick(int brow, int bcol, int row, int col);

    private:
        void populate();
        SudokuBoxWidget *boxes[3][3];
        //! To make enumerations easy
        QList<SudokuBoxWidget*> allBoxes;
        Ui::SudokuBoard *ui;
};

#endif // SUDOKUBOARD_H

// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

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

//! Represents a sudoku board
//! Note: this program count numbers from 0, so first column in program has ID 0 and last nineth has ID 8, the human representation as well as exported
//! data however work from 1 to 9
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
        int SetValue(unsigned int value, bool read_only = false);
        int ClearValue(int row, int col, bool read_only = false);
        int ClearValue(bool read_only = false);
        int SetValueHint(int row, int col, unsigned int value);
        int SetValueHint(unsigned int value);
        int CheckIfValueCanBePlaced(int row, int col, unsigned int value);
        void FlagInvalidHints();
        void FlagInvalidHints(int row, int col);
        bool FindAllHints();
        bool FindAllHints(int row, int col);
        bool FindHint();
        bool FindHint(int row, int col);
        int SolveRecursively();
        int CountSolutions();
        QString ExportToCommandList();
        SudokuItemWidget *GetItem(int row, int col);
        SudokuBoxWidget *GetBox(int row, int col);
        void ResetChange();
        int SelectedRow = 0;
        int SelectedCol = 0;

    signals:
        void Clicked(int row, int col);

    private slots:
        void OnClick(int brow, int bcol, int row, int col);

    private:
        void populate();
        //! Removes all hints for a value in whole axis surrounding
        void removeHints(int row, int col, unsigned int value);
        bool isModified = false;
        SudokuBoxWidget *boxes[3][3];
        unsigned int valueHint[9][9];
        //! To make enumerations easy
        QList<SudokuBoxWidget*> allBoxes;
        Ui::SudokuBoard *ui;
};

#endif // SUDOKUBOARD_H

// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef SUDOKUCOORD_H
#define SUDOKUCOORD_H

class SudokuCoord
{
    public:
        SudokuCoord()=default;
        SudokuCoord(int row, int col);
        int Row = 0;
        int Col = 0;
};

#endif // SUDOKUCOORD_H

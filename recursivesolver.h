// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef RECURSIVESOLVER_H
#define RECURSIVESOLVER_H

#include <QHash>

class RecursiveSolver
{
    public:
        static QHash<int, QList<int>> Solve(unsigned int sudoku[9][9], int *solutions);
        static QHash<int, QList<int>> Solve(QHash<int, QList<int>> sudoku, int *solutions);
    private:
        static int GetBoxID(int p);
        static bool solve(QHash<int, QList<int>> *sudoku, int *solutions);
        static bool findNextEmptyCell(QHash<int, QList<int>> *sudoku, int *row, int *col);
        static bool valueCanBeUsed(QHash<int, QList<int>> *sudoku, int row, int col, int value);
};

#endif // RECURSIVESOLVER_H

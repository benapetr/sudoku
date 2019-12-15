// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef RECURSIVESOLVER_H
#define RECURSIVESOLVER_H

#include <QHash>
#include "global.h"
#include "sudokucoord.h"

class RecursiveSolver
{
    public:
        static int Count(unsigned int sudoku[9][9]);
        static int Count(QHash<int, QList<int>> sudoku);
        static bool HasMoreThanOne(QHash<int, QList<int>> sudoku);
        //! Try to randomly remove as many clues as possible so that sudoku can still have max 1 solutions
        static bool MakeHard(QHash<int, QList<int>> *sudoku, int max_attempts = 100);
        static QHash<int, QList<int>> Solve(unsigned int sudoku[9][9], int *solutions);
        static QHash<int, QList<int>> Solve(QHash<int, QList<int>> sudoku, int *solutions);
        static QHash<int, QList<int>> GenerateRandom(unsigned int required_clues = 18, unsigned int preseed = 10);
        static bool CanBeSolved(QHash<int, QList<int>> sudoku);
        static QHash<int, QList<int>> VectorsToHash(unsigned int sudoku[9][9]);
    private:
        static int GetBoxID(int p);
        static bool solve(QHash<int, QList<int>> *sudoku, int *solutions);
        static bool count(QHash<int, QList<int>> *sudoku, int *solutions, int max_solutions = SUDOKU_MAX_SOLUTIONS);
        static bool findNextEmptyCell(QHash<int, QList<int>> *sudoku, int *row, int *col);
        static bool valueCanBeUsed(QHash<int, QList<int>> *sudoku, int row, int col, int value);
        static bool seedRandomly(QHash<int, QList<int>> *sudoku, unsigned int preseed, unsigned int max_attempts);
        //! For N attempts try to find a random place in sudoku which is missing a clue and add that clue from solution mask
        //! if no free place is found in N attempts this function gives up
        static void insertRandomClue(QHash<int, QList<int>> *sudoku, QHash<int, QList<int>> *solution);
        //! Insert clue to next free space
        static void insertNextClue(QHash<int, QList<int>> *sudoku, QHash<int, QList<int>> *solution);
        static QList<SudokuCoord> getCoordsOfUsedValues(const QHash<int, QList<int>> &sudoku);
        static QList<SudokuCoord> getCoordsOfEmptyValues(const QHash<int, QList<int>> &sudoku);
        static int getRandom(int min, int max);
};

#endif // RECURSIVESOLVER_H

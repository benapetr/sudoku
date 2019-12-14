// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "recursivesolver.h"

int RecursiveSolver::Count(unsigned int sudoku[9][9])
{
    QHash<int, QList<int>> hash = RecursiveSolver::vectorsToHash(sudoku);
    int result = 0;
    RecursiveSolver::count(&hash, &result);
    return result;
}

QHash<int, QList<int>> RecursiveSolver::Solve(unsigned int sudoku[9][9], int *solutions)
{
    return Solve(RecursiveSolver::vectorsToHash(sudoku), solutions);
}

QHash<int, QList<int>> RecursiveSolver::Solve(QHash<int, QList<int>> sudoku, int *solutions)
{
    if (!RecursiveSolver::solve(&sudoku, solutions))
        *solutions = 0;
    return sudoku;
}

int RecursiveSolver::GetBoxID(int p)
{
    switch (p)
    {
        case 0:
        case 1:
        case 2:
            return 0;
        case 3:
        case 4:
        case 5:
            return 1;
        case 6:
        case 7:
        case 8:
            return 2;
    }
    return -1;
}

QHash<int, QList<int> > RecursiveSolver::vectorsToHash(unsigned int sudoku[9][9])
{
    QHash<int, QList<int>> qsudoku;
    int row, col;
    row = 0;
    while (row < 9)
    {
        col = 0;
        qsudoku.insert(row, QList<int>());
        while (col < 9)
        {
            qsudoku[row].insert(col, sudoku[row][col]);
            col++;
        }
        row++;
    }
    return qsudoku;
}

bool RecursiveSolver::solve(QHash<int, QList<int>> *sudoku, int *solutions)
{
    int empty_row, empty_col;
    if (!RecursiveSolver::findNextEmptyCell(sudoku, &empty_row, &empty_col))
    {
        // Solved
        (*solutions)++;
        return true;
    }
    // Try all 9 numbers on this cell, starting with 1
    int number = 1;
    while (number < 10)
    {
        if (RecursiveSolver::valueCanBeUsed(sudoku, empty_row, empty_col, number))
        {
            (*sudoku)[empty_row][empty_col] = number;
            if (RecursiveSolver::solve(sudoku, solutions))
                return true;
        }
        number++;
    }
    // No solutions from here
    (*sudoku)[empty_row][empty_col] = 0;
    return false;
}

bool RecursiveSolver::count(QHash<int, QList<int> > *sudoku, int *solutions)
{
    if (*solutions > 100)
        return true;
    int empty_row, empty_col;
    if (!RecursiveSolver::findNextEmptyCell(sudoku, &empty_row, &empty_col))
    {
        // Solved
        (*solutions)++;
        return true;
    }
    // Try all 9 numbers on this cell, starting with 1
    int number = 1;
    while (number < 10)
    {
        if (RecursiveSolver::valueCanBeUsed(sudoku, empty_row, empty_col, number))
        {
            (*sudoku)[empty_row][empty_col] = number;
            // Here we are counting solutions, so even if this sudoku is already solved, let's try another number
            RecursiveSolver::count(sudoku, solutions);
        }
        number++;
    }
    // No more solutions from here
    (*sudoku)[empty_row][empty_col] = 0;
    return false;
}

bool RecursiveSolver::findNextEmptyCell(QHash<int, QList<int>> *sudoku, int *row, int *col)
{
    int r, c;
    r = 0;
    while (r < 9)
    {
        c = 0;
        while (c < 9)
        {
            if (sudoku->value(r)[c] == 0)
            {
                *row = r;
                *col = c;
                return true;
            }
            c++;
        }
        r++;
    }
    return false;
}

bool RecursiveSolver::valueCanBeUsed(QHash<int, QList<int> > *sudoku, int row, int col, int value)
{
    // Check axis
    int c = 0;
    while (c < 9)
    {
        if (sudoku->value(row)[c++] == value)
            return false;
    }
    int r = 0;
    while (r < 9)
    {
        if (sudoku->value(r++)[col] == value)
            return false;
    }
    // Check the box
    int box_start_row = GetBoxID(row) * 3;
    int box_start_col = GetBoxID(col) * 3;
    r = box_start_row;
    while (r < (box_start_row + 3))
    {
        c = box_start_col;
        while (c < (box_start_col + 3))
        {
            if (sudoku->value(r)[c++] == value)
                return false;
        }
        r++;
    }
    return true;
}

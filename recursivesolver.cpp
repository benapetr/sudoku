// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "recursivesolver.h"
#include "global.h"
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

int RecursiveSolver::Count(unsigned int sudoku[9][9])
{
    QHash<int, QList<int>> hash = RecursiveSolver::VectorsToHash(sudoku);
    return RecursiveSolver::Count(hash);
}

int RecursiveSolver::Count(QHash<int, QList<int>> sudoku)
{
    int result = 0;
    QHash<int, QList<int>> c = sudoku;
    RecursiveSolver::count(&c, &result);
    return result;
}

bool RecursiveSolver::HasMoreThanOne(QHash<int, QList<int>> sudoku)
{
    int result = 0;
    QHash<int, QList<int>> c = sudoku;
    RecursiveSolver::count(&c, &result, 2);
    return result > 1;
}

bool RecursiveSolver::MakeHard(QHash<int, QList<int>> *sudoku, int max_attempts)
{
    // Find all used values
    QList<SudokuCoord> used = getCoordsOfUsedValues(*sudoku);
    while (used.count() > 0 && max_attempts > 0)
    {
        // Take random coord
        int random_coord_id = getRandom(0, used.count());
        SudokuCoord random_item = used[random_coord_id];

        // Save its value
        int value = (*sudoku)[random_item.Row][random_item.Col];
        Q_ASSERT(value != 0);

        // Remove it from sudoku
        (*sudoku)[random_item.Row][random_item.Col] = 0;

        // Remove from pool
        used.removeAt(random_coord_id);

        // Check if it still has only 1 solution
        if (RecursiveSolver::HasMoreThanOne(*sudoku))
        {
            // return value back to sudoku puzzle
            (*sudoku)[random_item.Row][random_item.Col] = value;

            // increase fuckups
            max_attempts--;
        }
    }
    return (max_attempts > 0);
}

QHash<int, QList<int>> RecursiveSolver::Solve(unsigned int sudoku[9][9], int *solutions)
{
    return Solve(RecursiveSolver::VectorsToHash(sudoku), solutions);
}

QHash<int, QList<int>> RecursiveSolver::Solve(QHash<int, QList<int>> sudoku, int *solutions)
{
    if (!RecursiveSolver::solve(&sudoku, solutions))
        *solutions = 0;
    return sudoku;
}

QHash<int, QList<int>> RecursiveSolver::GenerateRandom(unsigned int required_clues, unsigned int preseed)
{
    unsigned int clues;
    // Seed an empty sudoku
    QHash<int, QList<int>> sudoku;
    QHash<int, QList<int>> sudoku_empty;
    sudoku_empty.insert(0, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(1, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(2, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(3, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(4, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(5, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(6, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(7, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));
    sudoku_empty.insert(8, QList<int>({0, 0, 0, 0, 0, 0, 0, 0, 0}));

    int required_preseed_attempts = 0;

    do
    {
        clues = 0;
        required_preseed_attempts++;
        // Reset the sudoku
        sudoku = sudoku_empty;
        while (!RecursiveSolver::seedRandomly(&sudoku, preseed, 100))
        {
            // We weren't able to construct a valid sudoku with this preseed value, probably it's too high, the CPU entropy is not good
            qDebug() << "Unable to preseed random sudoku using " << preseed << " starting values, decreasing";
            required_preseed_attempts++;
            sudoku = sudoku_empty;
            preseed--;
        }
    } while (!RecursiveSolver::CanBeSolved(sudoku));
    qDebug() << "Preseeded random sudoku after " << required_preseed_attempts << " attempts";

    // We have as many clues as values we managed to preseed
    clues = preseed;

    int unused = 0;
    // So now we have a random entropy that leads to 1 or more solutions sudoku, let's get the first possible solution and add random N numbers
    QHash<int, QList<int>> solution = sudoku;
    RecursiveSolver::solve(&solution, &unused);

    // Now that we have a solution, let just reveal more clues until we have as many as we wanted
    while (clues < required_clues)
    {
        insertRandomClue(&sudoku, &solution);
        clues++;
    }

    // Make sure this sudoku can have only 1 solution
    while (RecursiveSolver::HasMoreThanOne(sudoku))
    {
        insertRandomClue(&sudoku, &solution);
        clues++;
    }

    return sudoku;
}

bool RecursiveSolver::CanBeSolved(QHash<int, QList<int>> sudoku)
{
    int solutions = 0;
    QHash<int, QList<int>> sc = sudoku;
    return RecursiveSolver::solve(&sc, &solutions);
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

QHash<int, QList<int>> RecursiveSolver::VectorsToHash(unsigned int sudoku[9][9])
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

bool RecursiveSolver::count(QHash<int, QList<int> > *sudoku, int *solutions, int max_solutions)
{
    if (*solutions > max_solutions)
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
    while (number < 10 && *solutions <= max_solutions)
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
            if ((*sudoku)[r][c] == 0)
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
        if ((*sudoku)[row][c++] == value)
            return false;
    }
    int r = 0;
    while (r < 9)
    {
        if ((*sudoku)[r++][col] == value)
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
            if ((*sudoku)[r][c++] == value)
                return false;
        }
        r++;
    }
    return true;
}

bool RecursiveSolver::seedRandomly(QHash<int, QList<int>> *sudoku, unsigned int preseed, unsigned int max_attempts)
{
    // Fill in some random values, this is rather slow operation, because we need to keep checking if sudoku still has solutions
    while (preseed > 0)
    {
        if (max_attempts <= 0)
            return false;
        int random_row = getRandom(0, 9);
        int random_col = getRandom(0, 9);

        // If there is already some value make sure we don't replace it
        if ((*sudoku)[random_row][random_col] != 0)
            continue;

        int random_value = getRandom(0, 9);

        if (!RecursiveSolver::valueCanBeUsed(sudoku, random_row, random_col, random_value))
        {
            max_attempts--;
            continue;
        }

        (*sudoku)[random_row][random_col] = random_value;
        preseed--;
    }
    return true;
}

void RecursiveSolver::insertRandomClue(QHash<int, QList<int>> *sudoku, QHash<int, QList<int>> *solution)
{
    // Get a list of all empty coords
    QList<SudokuCoord> empty_coords = getCoordsOfEmptyValues(*sudoku);
    SudokuCoord random_coord = empty_coords.at(getRandom(0, empty_coords.count()));
    (*sudoku)[random_coord.Row][random_coord.Col] = (*solution)[random_coord.Row][random_coord.Col];
}

void RecursiveSolver::insertNextClue(QHash<int, QList<int>> *sudoku, QHash<int, QList<int>> *solution)
{
    int row = 0, col;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            if ((*sudoku)[row][col] == 0)
            {
                (*sudoku)[row][col] = (*solution)[row][col];
                return;
            }
            col++;
        }
        row++;
    }
}

QList<SudokuCoord> RecursiveSolver::getCoordsOfUsedValues(const QHash<int, QList<int> > &sudoku)
{
    QList<SudokuCoord> result;
    int row = 0, col;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            if (sudoku[row][col] != 0)
            {
                result.append(SudokuCoord(row, col));
            }
            col++;
        }
        row++;
    }
    return result;
}

QList<SudokuCoord> RecursiveSolver::getCoordsOfEmptyValues(const QHash<int, QList<int> > &sudoku)
{
    QList<SudokuCoord> result;
    int row = 0, col;
    while (row < 9)
    {
        col = 0;
        while (col < 9)
        {
            if (sudoku[row][col] == 0)
            {
                result.append(SudokuCoord(row, col));
            }
            col++;
        }
        row++;
    }
    return result;
}

int RecursiveSolver::getRandom(int min, int max)
{
    QRandomGenerator rand(QDateTime::currentMSecsSinceEpoch());
    return rand.bounded(min, max);
}

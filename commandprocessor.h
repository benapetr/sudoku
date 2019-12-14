// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QString>

class SudokuBoard;
class SudokuItemWidget;
class SudokuBoxWidget;

//!
//! \brief The CommandProcessor class is dealing with simple commands that are used in exports and imports of sudokus
//!
//! Syntax:
//! Each command is case in-sensitive
//! Lines starting with # are comments
//!
//! Commands:
//! SwitchMode editor|game
//! Switches between editor and game modes, you can only switch from editor to game mode
//!
//! SetValue row col value (example: "SetValue 1 2 5")
//! Sets a value in sudoku board
//!
class CommandProcessor
{
    public:
        CommandProcessor(SudokuBoard *sudoku_board);
        void ProcessText(QString text);
        void ProcessCommand(QString line);

    private:
        SudokuBoard *board;
};

#endif // COMMANDPROCESSOR_H

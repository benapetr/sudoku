#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

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
        CommandProcessor();
};

#endif // COMMANDPROCESSOR_H

// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "commandprocessor.h"
#include "errors.h"
#include "sudokuboard.h"
#include "sudokuboxwidget.h"

CommandProcessor::CommandProcessor(SudokuBoard *sudoku_board)
{
    this->board = sudoku_board;
}

bool CommandProcessor::ProcessText(QString text)
{
    QList<QString> lines = text.split("\n");
    foreach (QString cmd, lines)
    {
        this->LastLine++;
        if (!this->ProcessCommand(cmd))
            return false;
    }
    return true;
}

bool CommandProcessor::ProcessCommand(QString line)
{
    line = line.trimmed();

    if (line.isEmpty() || line.startsWith("#"))
        return true;

    QList<QString> parts = line.split(" ");
    QString command = parts[0].toLower();
    parts.removeAt(0);

    if (command == "switchmode")
        return this->processSwitchMode(parts);
    if (command == "sethint")
        return this->processSetHint(parts);
    if (command == "setvalue")
        return this->processSetValue(parts);

    this->LastError = "Unknown command: " + command;
    return false;
}

GameMode CommandProcessor::GetGM()
{
    return this->gameMode;
}

bool CommandProcessor::processSetValue(QList<QString> parameters)
{
    if (parameters.count() != 3)
    {
        this->LastError = "SetValue expects exactly 3 parameters";
        return false;
    }

    int result = this->board->SetValue(parameters[0].toInt(), parameters[1].toInt(), parameters[2].toUInt(), this->gameMode == GameMode_Editor);
    if (result > 1)
    {
        this->LastError = Errors::ToString(result);
        return false;
    }
    return true;
}

bool CommandProcessor::processSetHint(QList<QString> parameters)
{
    if (this->gameMode != GameMode_Player)
    {
        this->LastError = "SetHint works only in game mode";
        return false;
    }

    if (parameters.count() != 3)
    {
        this->LastError = "SetHint expects exactly 3 parameters";
        return false;
    }

    int result = this->board->SetValueHint(parameters[0].toInt(), parameters[1].toInt(), parameters[2].toUInt());
    if (result > 1)
    {
        this->LastError = Errors::ToString(result);
        return false;
    }
    return true;
}

bool CommandProcessor::processSwitchMode(QList<QString> parameters)
{
    if (parameters.count() != 1)
    {
        this->LastError = "SwitchMode expects exactly 1 parameter";
        return false;
    }

    QString mode = parameters[0].toLower();
    if (mode == "editor")
    {
        if (this->gameMode == GameMode_Player)
        {
            this->LastError = "It's not allowed to switch from player mode back to editor mode";
            return false;
        }
        this->gameMode = GameMode_Editor;
    } else if (mode == "game")
    {
        this->gameMode = GameMode_Player;
    } else
    {
        this->LastError = "Unknown game mode: " + mode;
        return false;
    }
    return true;
}

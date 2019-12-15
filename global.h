// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef GLOBAL_H
#define GLOBAL_H

#define APP_VERSION "1.0.0.0"

#define SUCCESS         0
#define E_NO_CHANGE     1
#define E_INVALID_VALUE 2
#define E_INVALID_ROW   3
#define E_INVALID_COL   4
#define E_ALREADY_USED  5
#define E_NOT_EMPTY     6
#define E_NO_SOLUTIONS                7
#define E_MULTIPLE_SOLUTIONS          8
#define E_INTERNAL_RESOLVER_ERR1      9
#define E_WRONG_VALUE   10
#define E_READ_ONLY     20

enum GameMode
{
    GameMode_Editor,
    GameMode_Player
};

#endif // GLOBAL_H

# SudokuPro

## What is this
This is a simple yet very powerful sudoku game "client", which can be used to play or create sudoku.

## Why did you create this? Doesn't million others exist?
Yes, they do, but either they aren't open source enough, or they simply don't meet my requirements:

* Ability to load / hand create custom sudokus (from paper)
* Simulate actual hard-core sudoku - paper-like, that is:
* Turn off all hints
* Turn off all suggestions
* Allow me to make a mistake and continue the game
* Let me copy / snapshot current sudoku so I can do some "expert-guessing" strategies

I simply couldn't find any so I made this. All in all, it took me 7 hours to make this program.
That's probably less than I would need should I want to google already existing tool that can do this.

## How can I compile this
This is standard C++/Qt solution which is CMake based, so you need to have:
* CMake
* Qt 5.4+
* C++ build environment

On GNU/Linux / MacOS you would simply run these commands from this folder:

```
mkdir build
cd build
cmake ..
make
```

## I need to talk to you
I am idling at irc.tm-irc.org or chat.freenode.net in channels #petan

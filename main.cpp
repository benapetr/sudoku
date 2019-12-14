// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "mainwindow.h"
#include "global.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("SudokuPro");
    QApplication::setApplicationVersion(APP_VERSION);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

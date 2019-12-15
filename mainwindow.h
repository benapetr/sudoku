// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SudokuBoard;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void New();
        void SetValue(int value);
        void UpdateMode(QString mode);
        void UpdateStatus(QString tx);
        void SwitchMode(GameMode mode);
        bool Save(QString path);
        bool Load(QString path);
        void UpdateTitle();

    private slots:
        void on_actionExit_triggered();
        void OnClick(int row, int col);
        void on_pushButton1_clicked();
        void on_pushButton2_clicked();
        void on_pushButton3_clicked();
        void on_pushButton4_clicked();
        void on_pushButton5_clicked();
        void on_pushButton6_clicked();
        void on_pushButton7_clicked();
        void on_pushButton8_clicked();
        void on_pushButton9_clicked();
        void on_pushButton_Wipe_clicked();
        void on_pushButton_PlayGame_clicked();
        void on_actionSave_as_triggered();
        void on_actionNew_triggered();
        void on_actionLoad_triggered();
        void on_actionSave_triggered();
        void on_actionRemove_hints_for_same_number_as_entered_triggered();
        void on_actionAutoload_last_file_triggered();
        void on_actionFlag_invalid_hints_triggered();
        void on_actionFind_all_hints_triggered();
        void on_actionFind_hints_triggered();
        void on_actionSolve_recursively_triggered();
        void on_actionCount_solutions_triggered();
        void on_actionRemove_all_hints_triggered();
        void on_actionAssisted_mode_informs_you_immediatelly_on_mistake_triggered();

    private:
        void closeEvent(QCloseEvent *event);
        bool NotifyChanges();
        QString currentFile;
        GameMode gameMode = GameMode_Editor;
        QLabel *labelMode;
        QLabel *labelStatus;
        SudokuBoard *board;
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

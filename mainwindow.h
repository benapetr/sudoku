#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum GameMode
{
    GameMode_Editor,
    GameMode_Player
};

class SudokuBoard;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void SetValue(int value);
        void UpdateMode(QString mode);
        void UpdateStatus(QString tx);
        void SwitchMode(GameMode mode);

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

    private:
        GameMode gameMode = GameMode_Editor;
        QLabel *labelMode;
        QLabel *labelStatus;
        SudokuBoard *board;
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

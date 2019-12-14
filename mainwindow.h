#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
        void SetValue(int value, bool read_only = false);

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

    private:
        SudokuBoard *board;
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

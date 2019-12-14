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

    private slots:
        void on_actionExit_triggered();
        void OnClick(int row, int col);

    private:
        SudokuBoard *board;
        Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

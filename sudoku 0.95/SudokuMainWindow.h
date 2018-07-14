#ifndef SUDOKUMAINWINDOW_H
#define SUDOKUMAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <thread>
#include <chrono>
#include "sudoku.h"

namespace Ui {
class SudokuMainWindow;
}

class SudokuMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SudokuMainWindow(QWidget *parent = nullptr);
    ~SudokuMainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionSavee_triggered();
    void on_actionExit_triggered();

    void on_thread_0_clicked();
	void on_thread_1_clicked();
	void on_thread_2_clicked();
    void on_thread_3_clicked();

private:
    Ui::SudokuMainWindow *ui;
    std::fstream file;
	std::thread *threads[4];
	sudoku_wt::Sudoku *sudoku[4];
	std::chrono::duration<double> elapsed_time;

    inline void run_the_threads();

};

#endif // SUDOKUMAINWINDOW_H

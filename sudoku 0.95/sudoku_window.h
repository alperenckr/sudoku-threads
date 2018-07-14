#ifndef SUDOKU_WINDOW_H
#define SUDOKU_WINDOW_H

#include <QDialog>
#include <QPushbutton>
#include <QTimer>
#include "Sudoku.h"

namespace Ui {
	class sudoku_window;
}

class sudoku_window : public QDialog
{
	Q_OBJECT

public:
	explicit sudoku_window(sudoku_wt::Sudoku *sudoku, QWidget *parent = nullptr);
	~sudoku_window();
	void add_solve_numbers();

private:
	void delay();
	sudoku_wt::Sudoku *sudoku;
	Ui::sudoku_window *ui;
	QPushButton *cells[9][9];
};

#endif // SUDOKU_WINDOW_H
#include "sudokumainwindow.h"
#include "ui_sudokumainwindow.h"
#include "sudoku_window.h"
#include <QFileDialog>
#include <QString>

SudokuMainWindow::SudokuMainWindow(QWidget *parent) :
	QMainWindow(parent),
	sudoku{ nullptr,nullptr,nullptr,nullptr },
	threads{ nullptr,nullptr,nullptr,nullptr },
	ui(new Ui::SudokuMainWindow)
{
	ui->setupUi(this);
}

SudokuMainWindow::~SudokuMainWindow()
{
	delete ui;
	for (int i = 0; i < 4; i++)
	{
		delete sudoku[i];
		delete threads[i];
	}
}

void SudokuMainWindow::on_actionOpen_triggered()
{
	QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*txt) "));
	file.open(filepath.toStdString());
	if (!filepath.isNull())
	{
		int index = 0;
		run_the_threads();
		if (sudoku[0]->is_finished())
		{
			ui->thread_0->setText("Thread 1 !!!WINNER!!!");
			index = 0;
		}
		else if (sudoku[1]->is_finished())
		{
			ui->thread_1->setText("Thread 2 !!!WINNER!!!");
			index = 1;
		}
		else if (sudoku[2]->is_finished())
		{
			ui->thread_2->setText("Thread 3 !!!WINNER!!!");
			index = 2;
		}
		else
		{
			ui->thread_3->setText("Thread 4 !!!WINNER!!!");
			index = 3;
		}
		sudoku_window *window = new sudoku_window(sudoku[index]);
		window->show();
		window->add_solve_numbers();
	}
}

void SudokuMainWindow::on_actionSavee_triggered()
{
	QString filepath = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath(), tr("Text files (*.txt)"));
	std::ofstream file(filepath.toStdString());
	if (!filepath.isNull())
	{
		for (int i{}; i < 4; i++)
		{
			file << "THREAD " << i << "\n";
			auto &temp_vec = sudoku[i]->get_game_order();
			for (auto it : temp_vec)
			{
				file << it.first << " " << it.second << " - " << sudoku[i]->at(it.first, it.second).value << "\n";
			}
		}
	}
}

void SudokuMainWindow::on_actionExit_triggered()
{
	exit(1);
}

void SudokuMainWindow::run_the_threads()
{
	for (int i{}; i < 4; i++) sudoku[i] = new sudoku_wt::Sudoku(file);

	auto begin = std::chrono::high_resolution_clock::now();
	for (int i{}; i < 4; i++)
	{
		threads[i] = new std::thread(sudoku_wt::Sudoku::solve, sudoku[i], i);
	}
	while (sudoku_wt::Sudoku::finished == sudoku_wt::NOBODY_FINISHED); //busy waiting
	auto end = std::chrono::high_resolution_clock::now();
	elapsed_time = end - begin;
	ui->timer->display(elapsed_time.count());
}

void SudokuMainWindow::on_thread_0_clicked()
{
	sudoku_window *window = new sudoku_window(sudoku[0]);
	window->show();
	window->add_solve_numbers();
}

void SudokuMainWindow::on_thread_3_clicked()
{
	sudoku_window *window = new sudoku_window(sudoku[3]);
	window->show();
	window->add_solve_numbers();
}

void SudokuMainWindow::on_thread_2_clicked()
{
	sudoku_window *window = new sudoku_window(sudoku[2]);
	window->show();
	window->add_solve_numbers();
}

void SudokuMainWindow::on_thread_1_clicked()
{
	sudoku_window *window = new sudoku_window(sudoku[1]);
	window->show();
	window->add_solve_numbers();
}

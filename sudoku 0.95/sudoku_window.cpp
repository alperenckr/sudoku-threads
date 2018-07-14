#include "sudoku_window.h"
#include "ui_sudoku_window.h"
#include <QTime>

sudoku_window::sudoku_window(sudoku_wt::Sudoku *sudoku, QWidget *parent) :
	sudoku(sudoku),
	QDialog(parent),
	ui(new Ui::sudoku_window)
{
	ui->setupUi(this);
	ui->textEdit->append("     X            Y             Value");
	for (int i{}; i < 9; i++)
	{
		for (int j{}; j < 9; j++)
		{
			cells[i][j] = new QPushButton();
			cells[i][j]->setFixedSize(QSize(40, 40));
			if ((i / 3 == 0 && j / 3 == 0) ||
				(i / 3 == 0 && j / 3 == 2) ||
				(i / 3 == 1 && j / 3 == 1) ||
				(i / 3 == 2 && j / 3 == 0) ||
				(i / 3 == 2 && j / 3 == 2))
			{
				cells[i][j]->setStyleSheet("background-color: rgb(150,150,150);");
			}


			if (!sudoku->at(i, j).changeable)
			{
				cells[i][j]->setText(QString::number(sudoku->at(i, j).value));
			}
			ui->table->addWidget(cells[i][j], i, j);
		}
	}
}

sudoku_window::~sudoku_window()
{
	delete ui;
	for (int i{}; i < 9; i++)
	{
		for (int j{}; j < 9; j++)
		{
			delete cells[i][j];
		}
	}
}

void sudoku_window::add_solve_numbers()
{
	auto& temp_vec = sudoku->get_game_order();
	for (auto it : temp_vec)
	{
		QString oldstyle = cells[it.first][it.second]->styleSheet();
		if (sudoku->at(it.first, it.second).value != 0)
		{
			
			ui->textEdit->append("     " + QString(QString::number(it.first + 1)) + "            " + QString::number(it.second + 1)
								+ "                 " + QString::number(sudoku->at(it.first, it.second).value));
			cells[it.first][it.second]->setText(QString::number(sudoku->at(it.first, it.second).value));
			cells[it.first][it.second]->setStyleSheet("background-color: rgb(50,50,235);");
		}
		delay();
		cells[it.first][it.second]->setStyleSheet(oldstyle);
	}
}
void sudoku_window::delay()
{
	QTime dietime = QTime::currentTime().addSecs(1);
	while (QTime::currentTime() < dietime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

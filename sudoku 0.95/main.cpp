#include "sudokumainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SudokuMainWindow w;
    w.show();

    return a.exec();
}

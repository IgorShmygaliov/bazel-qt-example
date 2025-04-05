#include <QApplication>
#include "okno.h"
#include "ticket.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    okno mainwindow;
    mainwindow.resize(960, 640);
    mainwindow.setWindowTitle("прокрастинатор");
    mainwindow.show();

    return QApplication::exec();
}


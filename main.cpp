#include "mainwindow.h"
#include "playerwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.autoplay(argc,argv);

    w.show();

    return a.exec();

    PlayerWindow playerWindow;
    playerWindow.show();
}

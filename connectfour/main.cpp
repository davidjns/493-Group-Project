#include "connectfour.h"
#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectFour w;
    w.show();

    return a.exec();
}

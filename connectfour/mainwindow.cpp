#include "mainwindow.h"
#include "connectfour.h"
#include "chatlog.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    resize(300,200);
    connectfour = new ConnectFour(this);
    connect(connectfour, SIGNAL(game_over()), this, SLOT(game_over()));
    connect(connectfour->chat, SIGNAL(second_player_joined()), this, SLOT(game_start()));
    setCentralWidget(connectfour);
}

void MainWindow::game_start() {
    resize(1000, 650);
}

void MainWindow::game_over() {
    resize(500,500);
    delete connectfour;
    connectfour = new ConnectFour(this);
}

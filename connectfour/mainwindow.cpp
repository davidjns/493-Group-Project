#include "mainwindow.h"
#include "connectfour.h"
#include "chatlog.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    initialize_game();
}

void MainWindow::game_start() {
    resize(1000, 600);
}

void MainWindow::initialize_game() {
    resize(300,200);
    connectfour = new ConnectFour(this);
    connect(connectfour, SIGNAL(game_over()), this, SLOT(game_over()));
    connect(connectfour->chat, SIGNAL(became_host()), this, SLOT(game_start()));
    connect(connectfour->chat, SIGNAL(player_joined()), this, SLOT(game_start()));
    setCentralWidget(connectfour);
}

void MainWindow::game_over() {
    qDebug() << "Game is over, going back to original state";
    initialize_game();
}

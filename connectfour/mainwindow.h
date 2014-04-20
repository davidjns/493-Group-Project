#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConnectFour;
class QHBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QHBoxLayout * box_layout;
    ConnectFour * connectfour;

    void initialize_game();

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void game_start();
    void game_over();

};

#endif // MAINWINDOW_H

#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <QWidget>

class QGridLayout;

class ConnectFour : public QWidget
{
    Q_OBJECT

    QGridLayout * grid_layout;          // visual grid

    enum color_t { RED, BLACK, NONE };
    color_t** space_grid;               // data representation of grid
    color_t player_turn;                // red or black, who's turn it is
    int turn_number;

    void increment_turn();

    void check_for_win();
    color_t check_horizontal_win();
    color_t check_vertical_win();
    color_t check_diagonal_down_right_win();
    color_t check_diagonal_up_right_win();
    void announce_winner(color_t);

public slots:
    void column_button_pressed();

public:
    ConnectFour(QWidget *parent = 0);
    ~ConnectFour();
};

#endif // CONNECTFOUR_H

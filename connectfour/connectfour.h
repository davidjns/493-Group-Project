#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <QWidget>

class QGridLayout;
class QHBoxLayout;
class ChatLog;

class ConnectFour : public QWidget
{
    Q_OBJECT
public:
    ConnectFour(QWidget *parent = 0);
    ~ConnectFour();

    ChatLog *chat;

private:
    QHBoxLayout *box_layout;
    QWidget *grid_widget;
    QGridLayout *grid_layout;          // visual grid

    enum color_t { RED, BLACK, NONE };
    static const QString colorNames[];
    const QString colorToString(color_t);

    color_t** space_grid;               // data representation of grid
    color_t player_turn;                // red or black, who's turn it is
    bool made_a_move;
    color_t my_color;                   // red or black, this instance's color
    int turn_number;

    void initializeGrid();
    void increment_turn();
    void check_for_win();
    color_t check_horizontal_win();
    color_t check_vertical_win();
    color_t check_diagonal_down_right_win();
    color_t check_diagonal_up_right_win();
    void announce_winner(color_t);

    bool place_token(int column_number);

signals:
    void game_over();

public slots:
    void square_clicked(int);
    void start_game();
    void host_game();
    void apply_move(int);

};

#endif // CONNECTFOUR_H

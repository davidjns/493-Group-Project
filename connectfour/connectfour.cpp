#include "connectfour.h"
#include "chatlog.h"
#include "connectfoursquare.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QUdpSocket>

const int NUM_ROWS = 6;
const int NUM_COLS = 7;

ConnectFour::ConnectFour(QWidget *parent)
    : QWidget(parent), player_turn(RED), my_color(NONE), turn_number(1), made_a_move(false)
{
    box_layout = new QHBoxLayout(this);
    initializeGrid();
    chat = new ChatLog(this);
    connect(chat, SIGNAL(became_host()), this, SLOT(host_game()));
    connect(chat, SIGNAL(player_joined()), this, SLOT(start_game()));
    connect(chat, SIGNAL(move_recieved(int)), this, SLOT(apply_move(int)));
    box_layout->addWidget(chat);
}

ConnectFour::~ConnectFour()
{
    // deallocate memory for data grid
    for(int i = 0; i < NUM_ROWS; i++)
        delete space_grid[i];
    delete space_grid;
}

void ConnectFour::initializeGrid()
{
    // create grid of data, fill with empty cells
    space_grid = new color_t*[NUM_ROWS];
    for(int i = 0; i < NUM_ROWS; i++) {
        space_grid[i] = new color_t[NUM_COLS];
        for(int j = 0; j < NUM_COLS; j++)
            space_grid[i][j] = NONE;
    }

    // create visual grid, leave blank for now
    grid_widget = new QWidget();
    grid_layout = new QGridLayout(grid_widget);
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);
    QImage empty_cell(":/empty_square.png");
    for(int i = 0; i < NUM_ROWS; i++) {
        for(int j = 0; j < NUM_COLS; j++) {
            ConnectFourSquare * label = new ConnectFourSquare(j, this);
            connect(label, SIGNAL(clicked(int)), this, SLOT(square_clicked(int)));
            label->setPixmap(QPixmap::fromImage(empty_cell));
            label->setFixedSize(QSize(100,100));
            grid_layout->addWidget(label, i, j);
        }
    }

    grid_widget->setLayout(grid_layout);
    grid_widget->hide();
    box_layout->addWidget(grid_widget);
}

void ConnectFour::start_game() {
    my_color = BLACK;
    grid_widget->show();
    chat->turnIndicator->setText(tr("Turn %1 Opponent's move").arg(turn_number));
    chat->turnIndicator->setStyleSheet("Color : " + colorToString(player_turn));
}

void ConnectFour::host_game() {
    my_color = RED;
    grid_widget->show();
    chat->turnIndicator->setText(tr("Turn %1 Your move").arg(turn_number));
    chat->turnIndicator->setStyleSheet("Color : " + colorToString(player_turn));
}

void ConnectFour::announce_winner(color_t winner) {
    QMessageBox * message = new QMessageBox(this);
    if(winner == my_color)
        message->setText("You Win!");
    else if(winner == NONE)
        message->setText("Tie Game!");
    else
        message->setText("You Lose!");
    message->exec();
    emit game_over();
}

// Checks if any player has won the game
void ConnectFour::check_for_win() {
    color_t winner;
    winner = check_horizontal_win();
    if(winner != NONE)
        announce_winner(winner);
    winner = check_vertical_win();
    if(winner != NONE)
        announce_winner(winner);
    winner = check_diagonal_down_right_win();
    if(winner != NONE)
        announce_winner(winner);
    winner = check_diagonal_up_right_win();
    if(winner != NONE)
        announce_winner(winner);
}

// checks for 4 in a row horizontally
ConnectFour::color_t ConnectFour::check_horizontal_win() {
    for(int i = 0; i < NUM_ROWS; i++) {
        color_t last_seen = NONE;
        int num_seen = 0;
        for(int j = 0; j < NUM_COLS; j++) {
            color_t color = space_grid[i][j];
            if(color == last_seen)
                num_seen++;
            else {
                num_seen = 1;
                last_seen = color;
            }
            if(num_seen == 4 && last_seen != NONE)
                return last_seen;
        }
    }
    return NONE;
}

// checks for 4 in a row vertically
ConnectFour::color_t ConnectFour::check_vertical_win() {
    for(int i = 0; i < NUM_COLS; i++) {
        color_t last_seen = NONE;
        int num_seen = 0;
        for(int j = 0; j < NUM_ROWS; j++) {
            color_t color = space_grid[j][i];
            if(color == last_seen)
                num_seen++;
            else {
                num_seen = 1;
                last_seen = color;
            }
            if(num_seen == 4 && last_seen != NONE)
                return last_seen;
        }
    }
    return NONE;
}

// Checks the grid for any 4 in a row on a down-right diagonal
ConnectFour::color_t ConnectFour::check_diagonal_down_right_win() {
    int row = 2; int col = 0;
    for(int diag = 0; diag < 6; diag++) {
        int i, j;
        color_t last_seen = NONE;
        int num_seen = 0;
        for(i = row, j = col; i < NUM_ROWS && j < NUM_COLS; i++, j++) {
            color_t color = space_grid[i][j];
            if(color == last_seen)
                num_seen++;
            else {
                num_seen = 1;
                last_seen = color;
            }
            if(num_seen == 4 && last_seen != NONE)
                return last_seen;
        }
        row--;
        if(row < 0) {
            row = 0;
            col++;
        }
    }
    return NONE;
}

// Checks the grid for any 4 in a row on an up-right diagonal
ConnectFour::color_t ConnectFour::check_diagonal_up_right_win() {
    int row = 3; int col = 0;
    for(int diag = 0; diag < 6; diag++) {
        int i, j;
        color_t last_seen = NONE;
        int num_seen = 0;
        for(i = row, j = col; i >= 0 && j < NUM_COLS; i--, j++) {
            color_t color = space_grid[i][j];
            if(color == last_seen)
                num_seen++;
            else {
                num_seen = 1;
                last_seen = color;
            }
            if(num_seen == 4 && last_seen != NONE)
                return last_seen;
        }
        row++;
        if(row == NUM_ROWS) {
            row = NUM_ROWS - 1;
            col++;
        }
    }
    return NONE;
}

const QString ConnectFour::colorToString(color_t color) {
    switch(color) {
    case RED:
        return "Red";
    case BLACK:
        return "Black";
    default:
        return "None";
    }
}

void ConnectFour::increment_turn() {
    if(player_turn == RED)
        player_turn = BLACK;
    else {
        player_turn = RED;
        turn_number++;
    }
    if(my_color == player_turn)
        chat->turnIndicator->setText(tr("Turn %1 Your move").arg(turn_number));
    else
        chat->turnIndicator->setText(tr("Turn %1 Opponent's move").arg(turn_number));
    chat->turnIndicator->setStyleSheet("Color : " + colorToString(player_turn));
    if(turn_number == 22) // Game has ended in a draw
        announce_winner(NONE);
}

void ConnectFour::square_clicked(int column_number) {
    if(player_turn == my_color)
        if(place_token(column_number)) {
            made_a_move = true;
            QString message = tr("M%1").arg(column_number);
            chat->sendMessage(message);
            check_for_win();
            increment_turn();
        }
}

void ConnectFour::apply_move(int col)
{
    qDebug() << "Move recieved in col " << col;
    if(made_a_move) {
        made_a_move = false;
        return;
    }
    if(player_turn != my_color) {
        place_token(col);
        check_for_win();
        increment_turn();
    }
}

// Returns true if a token was played
bool ConnectFour::place_token(int column_number) {
    for(int i = NUM_ROWS - 1; i >= 0; i--) {
        if(space_grid[i][column_number] == NONE) {
            space_grid[i][column_number] = player_turn;
            if(player_turn == RED) {
                QImage red_square(":/red_square.png");
                ((ConnectFourSquare *)grid_layout->itemAtPosition(i,column_number)->widget())->setPixmap(QPixmap::fromImage(red_square));
            } else {
                QImage black_square(":/black_square.png");
                ((ConnectFourSquare *)grid_layout->itemAtPosition(i,column_number)->widget())->setPixmap(QPixmap::fromImage(black_square));
            }
            return true;
        }
    }
    return false;
}

#include "connectfour.h"
#include "chatlog.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QMessageBox>
#include <QHBoxLayout>

const int NUM_ROWS = 6;
const int NUM_COLS = 7;

ConnectFour::ConnectFour(QWidget *parent)
    : QWidget(parent), player_turn(RED), turn_number(1)
{
    box_layout = new QHBoxLayout(this);

    initializeGrid();

    chat = new ChatLog();
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
    grid_layout = new QGridLayout();
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);
    this->setLayout(grid_layout);
    for(int i = 0; i < NUM_COLS; i++) {
        QPushButton * button = new QPushButton(tr("Column %1").arg(i + 1), this);
        connect(button, SIGNAL(clicked()), this, SLOT(column_button_pressed()));
        grid_layout->addWidget(button, 0, i);
    }
    QImage empty_cell(":/empty_square.png");
    if(empty_cell.isNull())
        exit(0);
    for(int i = 1; i <= NUM_ROWS; i++) {
        for(int j = 0; j < NUM_COLS; j++) {
            QLabel * label = new QLabel(this);
            label->setPixmap(QPixmap::fromImage(empty_cell));
            label->setFixedSize(QSize(100,100));
            grid_layout->addWidget(label, i, j);
        }
    }

    QWidget *grid_widget = new QWidget();
    grid_widget->setLayout(grid_layout);
    box_layout->addWidget(grid_widget);
}

void ConnectFour::announce_winner(color_t winner) {
    QMessageBox * message = new QMessageBox(this);
    if(winner == RED)
        message->setText("Red Player Wins!");
    else
        message->setText("Black Player Wins!");
    message->exec();
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

void ConnectFour::increment_turn() {
    if(player_turn == RED)
        player_turn = BLACK;
    else {
        player_turn = RED;
        turn_number++;
    }
}

void ConnectFour::column_button_pressed() {
    QString button_name = ((QPushButton *)sender())->text();
    int column_number = button_name[7].digitValue() - 1;
    for(int i = NUM_ROWS - 1; i >= 0; i--) {
        if(space_grid[i][column_number] == NONE) {
            space_grid[i][column_number] = player_turn;
            if(player_turn == RED) {
                QImage red_square(":/red_square.png");
                ((QLabel *)grid_layout->itemAtPosition(i+1,column_number)->widget())->setPixmap(QPixmap::fromImage(red_square));
            } else {
                QImage black_square(":black_square.png");
                ((QLabel *)grid_layout->itemAtPosition(i+1,column_number)->widget())->setPixmap(QPixmap::fromImage(black_square));
            }
            check_for_win();
            increment_turn();
            return;
        }
    }
    // if it makes it here, invalid move
}

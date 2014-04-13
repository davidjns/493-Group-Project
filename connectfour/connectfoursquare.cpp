#include "connectfoursquare.h"

ConnectFourSquare::ConnectFourSquare(int column_number_, QWidget *parent) :
    QLabel(parent), column_number(column_number_)
{
}

void ConnectFourSquare::mouseReleaseEvent(QMouseEvent *event) {
    emit clicked(column_number);
}

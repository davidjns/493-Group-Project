#ifndef CONNECTFOURSQUARE_H
#define CONNECTFOURSQUARE_H

#include <QLabel>

class ConnectFourSquare : public QLabel
{
    Q_OBJECT

    int column_number;

public:
    explicit ConnectFourSquare(int column_number_, QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void clicked(int);

};

#endif // CONNECTFOURSQUARE_H

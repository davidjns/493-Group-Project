#ifndef SERVER_H
#define SERVER_H

class QTcpSocket;

#include <QTcpServer>
#include <QMap>
#include <QSet>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);

private slots:
    void readyRead();
    void disconnected();
    void sendUserList();
    void newConnection();

protected:
    void incomingConnection(int socketfd);


private:
    QSet<QTcpSocket*> clients;
    QMap<QTcpSocket*, QString> users;
};

#endif // SERVER_H

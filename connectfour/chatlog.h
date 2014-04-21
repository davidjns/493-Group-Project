#ifndef CHATLOG_H
#define CHATLOG_H

class QFrame;
class QLabel;
class QPushButton;
class QLineEdit;
class QTcpSocket;
class QStackedWidget;
class QGridLayout;
class QStackedWidget;
class QListWidget;
class QTextEdit;
class QUdpSocket;
class QTcpServer;

class MySocket;

#include <QWidget>
#include <QHostAddress>

class ChatLog : public QWidget
{
    Q_OBJECT
public:
    ChatLog(QWidget *parent = 0);
    QUdpSocket *socketIn;
    QUdpSocket *socketOut;

    QLabel *turnIndicator;

    void sendMessage(QString message);
    void closeServers();
    quint16 getNumPlayers();

signals:
    void became_host();
    void player_joined();
    void move_recieved(int);

private slots:
    void on_hostButton_clicked();
    void on_joinButton_clicked();
    void on_sayButton_clicked();
    void connected();

    void processPendingDatagrams();

    void changePage();

private:
    QStackedWidget *stackedWidget;

    QHostAddress hostAddress;
    quint16 portNumber;
    QTcpServer *server;
    QTcpServer *server2;
    quint16 clientPortNumber;

    QWidget *loginPage;
    QFrame *loginFrame;
    QGridLayout *loginLayout;
    QPushButton *hostButton;
    QPushButton *joinButton;
    QLineEdit *serverLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *userLineEdit;
    QLabel *ipLabel;
    QLabel *failedLabel;

    QWidget *chatPage;
    QFrame *chatFrame;
    QGridLayout *chatLayout;
    QPushButton *sayButton;
    QLineEdit *sayLineEdit;
    QTextEdit *roomTextEdit;
    QTextEdit *clientsTextEdit;
    QListWidget *userListWidget;

    QString serverName;
    QString userName;

    bool host;

    void initializeLoginPage();
    void initializeChatPage();
    void displayErrorDialog();

    void playerConnected(QString playerName);
    qint16 numPlayers;

};

#endif // CHATLOG_H

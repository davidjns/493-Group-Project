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

class MySocket;

#include <QWidget>

class ChatLog : public QWidget
{
    Q_OBJECT
public:
    ChatLog(QWidget *parent = 0);

private slots:
    void on_hostButton_clicked();
    void on_joinButton_clicked();
    void on_sayButton_clicked();
    void connected();

    void processPendingDatagrams();

    void changePage();

private:
    QStackedWidget *stackedWidget;

    QUdpSocket *socketIn;
    QUdpSocket *socketOut;

    QWidget *loginPage;
    QFrame *loginFrame;
    QGridLayout *loginLayout;
    QPushButton *hostButton;
    QPushButton *joinButton;
    QLineEdit *serverLineEdit;
    QLineEdit *userLineEdit;

    QWidget *chatPage;
    QFrame *chatFrame;
    QGridLayout *chatLayout;
    QPushButton *sayButton;
    QLineEdit *sayLineEdit;
    QTextEdit *roomTextEdit;
    QListWidget *userListWidget;

    QString serverName;
    QString userName;

    void initializeLoginPage();
    void initializeChatPage();

};

#endif // CHATLOG_H
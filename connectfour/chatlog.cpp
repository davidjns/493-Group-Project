#include "chatlog.h"
#include "server.h"
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QTimer>
#include <QDateTime>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHBoxLayout>
#include <QDialog>

ChatLog::ChatLog(QWidget *parent) :
    QWidget(parent)
{
    host = false;
    this->setMinimumSize(300, 200);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->resize(1000, 1000);

    initializeLoginPage();
    initializeChatPage();

    stackedWidget->setCurrentWidget(loginPage);

    socketIn = new QUdpSocket(this);
    connect(socketIn, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    socketOut = new QUdpSocket(this);
    connect(socketIn, SIGNAL(connected()), this, SLOT(connected()));
}

void ChatLog::initializeLoginPage()
{
    loginPage = new QWidget();
    stackedWidget->addWidget(loginPage);

    loginFrame = new QFrame(loginPage);
    loginFrame->setFixedSize(250, 175);
    loginLayout = new QGridLayout(loginFrame);

    hostButton = new QPushButton("Host Game");
    connect(hostButton, SIGNAL(clicked()), this, SLOT(on_hostButton_clicked()));
    joinButton = new QPushButton("Join Game");
    connect(joinButton, SIGNAL(clicked()), this, SLOT(on_joinButton_clicked()));

    serverLineEdit = new QLineEdit();
    portLineEdit = new QLineEdit();
    userLineEdit = new QLineEdit();

    QLabel *label1 = new QLabel("User Name: ");
    QLabel *label2 = new QLabel("Server Name: ");
    QLabel *label3 = new QLabel("Port Number: ");

    loginLayout->addWidget(label1, 1, 1);
    loginLayout->addWidget(userLineEdit, 1, 2);
    loginLayout->addWidget(label2, 2, 1);
    loginLayout->addWidget(serverLineEdit, 2, 2);
    loginLayout->addWidget(label3, 3, 1);
    loginLayout->addWidget(portLineEdit, 3, 2);
    loginLayout->addWidget(joinButton, 4, 2);
    loginLayout->addWidget(hostButton, 5, 2);
}

void ChatLog::initializeChatPage()
{
    chatPage = new QWidget();
    stackedWidget->addWidget(chatPage);

    chatFrame = new QFrame(chatPage);
    chatFrame->setMinimumSize(250, 175);
    chatLayout = new QGridLayout(chatFrame);

    sayButton = new QPushButton("Send");
    connect(sayButton, SIGNAL(clicked()), this, SLOT(on_sayButton_clicked()));

    sayLineEdit = new QLineEdit();
    sayLineEdit->setFocus();
    roomTextEdit = new QTextEdit();
    roomTextEdit->setReadOnly(true);
    userListWidget = new QListWidget();

    QWidget *lowerChat = new QWidget();
    QHBoxLayout *lowerChatLayout = new QHBoxLayout();
    lowerChat->setLayout(lowerChatLayout);
    lowerChatLayout->addWidget(sayLineEdit);
    lowerChatLayout->addWidget(sayButton);

    chatLayout->addWidget(roomTextEdit, 1, 1);
    chatLayout->addWidget(lowerChat, 2, 1);
}

void ChatLog::on_hostButton_clicked()
{
    qDebug() << "here";
//    Server *server = new Server();
//    Server *server2 = new Server();
    QTcpServer *server = new QTcpServer();
    QTcpServer *server2 = new QTcpServer();

    QTcpSocket s;
    s.connectToHost("8.8.8.8", 53);
    if(s.waitForConnected())
    {
        hostAddress = s.localAddress();
        qDebug() << "my local address is: " << s.localAddress();
    }
    else
        qDebug() << "could not retrieve a host address.";

//    portNumber = portLineEdit->text().toInt();
    portNumber = 4200;

    bool success = server->listen(hostAddress, portNumber);
    bool success2 = server2->listen(hostAddress, portNumber+1);
    if(!success || !success2)
        qDebug("Could not listen to port 4200");
    else
    {
        qDebug() << "Server Ready";
        host = true;

        if(socketIn->bind(hostAddress, portNumber))
        {
            connected();
            qDebug() << "connected to port 4200";
        }
        else
        {
            //socketIn->connectToHost(hostAddress, portNumber);
            //connected();

            qDebug() << "failed to connect";
        }
    }

    emit became_host();

    QDialog *dialog = new QDialog();
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label = new QLabel("You have connected to server: " + s.localAddress().toString());
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->show();
}

void ChatLog::on_joinButton_clicked()
{
    serverName = serverLineEdit->text();

    hostAddress = QHostAddress(serverName);
    portNumber = 4200;
    //portNumber = portLineEdit->text().toInt();

    if(socketIn->bind(hostAddress, portNumber+1))
    {
        connected();
        qDebug() << "connected to port 4201";
    }
    else
    {
        socketIn->connectToHost(hostAddress, portNumber);
        connected();

        qDebug() << "failed to connect";
    }

    emit player_joined();
}

void ChatLog::on_sayButton_clicked()
{
    QString message = "C(" + (QTime::currentTime()).toString() + ") " + userName + ": " + sayLineEdit->text().trimmed() + "\r\n";
    sendMessage(message);

    sayLineEdit->clear();
    sayLineEdit->setFocus();
}

void ChatLog::sendMessage(QString message)
{
    if(!message.isEmpty())
    {
        qDebug() << "broadcasting datagram";
        QByteArray datagram;
        datagram.append(message);
        socketOut->writeDatagram(datagram.data(), datagram.size(), hostAddress, portNumber);
        socketOut->writeDatagram(datagram.data(), datagram.size(), hostAddress, portNumber+1);
    }
}

void ChatLog::processPendingDatagrams()
{
    while(socketIn->hasPendingDatagrams())
    {
        qDebug() << "processing datagrams";
        QByteArray datagram;
        datagram.resize(socketIn->pendingDatagramSize());
        socketIn->readDatagram(datagram.data(), datagram.size());

        //THIS IS WHEN WE HAVE THE DATAGRAM, PARSE IT ACCORDINGLY
        qDebug() << "Recieved datagram" << datagram.data();
        QString message = datagram.data();
        if(!message.isEmpty())
        {
            QChar messageType = message.at(0);
            message.remove(0, 1);

            if(messageType == QChar('C'))
            {
                roomTextEdit->append(message);
            }
            else if(messageType == QChar('M'))
            {
                emit move_recieved(message.toInt());
            }
            else if(messageType == QChar('P'))
            {
                roomTextEdit->append(message);
            }
        }
    }
}

void ChatLog::connected()
{
    qDebug() << "Connected";
    stackedWidget->setCurrentWidget(chatPage);
    resize(550, 275);

    userName = userLineEdit->text();

    sendMessage("P" + QString(userLineEdit->text() + " has connected!\n"));
}

void ChatLog::changePage()
{
    stackedWidget->setCurrentWidget(chatPage);
    resize(550, 275);
}

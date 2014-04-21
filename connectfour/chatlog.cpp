#include "chatlog.h"
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
#include <QAction>

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "stdio.h"

#define DEFAULT_PORT "27105";

ChatLog::ChatLog(QWidget *parent) :
    QWidget(parent)
{
    host = false;
    numPlayers = 0;
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
    //loginLayout->addWidget(label3, 3, 1);
    //loginLayout->addWidget(portLineEdit, 3, 2);
    loginLayout->addWidget(joinButton, 3, 2);
    loginLayout->addWidget(hostButton, 4, 2);
}

void ChatLog::initializeChatPage()
{
    chatPage = new QWidget();
    stackedWidget->addWidget(chatPage);

    chatFrame = new QFrame(chatPage);
    chatFrame->setMinimumSize(250, 200);
    chatLayout = new QGridLayout(chatFrame);

    sayButton = new QPushButton("Send");
    connect(sayButton, SIGNAL(clicked()), this, SLOT(on_sayButton_clicked()));

    sayLineEdit = new QLineEdit();
    sayLineEdit->setFocus();
    connect(sayLineEdit, SIGNAL(returnPressed()), this, SLOT(on_sayButton_clicked()));
    roomTextEdit = new QTextEdit();
    roomTextEdit->setReadOnly(true);
    roomTextEdit->setFixedWidth(275);
    clientsTextEdit = new QTextEdit();
    clientsTextEdit->setReadOnly(true);
    clientsTextEdit->setFixedWidth(100);
    clientsTextEdit->append("Players in Chat:");
    userListWidget = new QListWidget();

    QWidget *lowerChat = new QWidget();
    QHBoxLayout *lowerChatLayout = new QHBoxLayout();
    lowerChat->setLayout(lowerChatLayout);
    lowerChatLayout->addWidget(sayLineEdit);
    lowerChatLayout->addWidget(sayButton);

    QFont font;
    font.setPointSize(13);
    font.setBold(true);

    turnIndicator = new QLabel();
    turnIndicator->setFrameStyle(QFrame::Panel | QFrame::Plain);
    turnIndicator->setLineWidth(3);
    turnIndicator->setFont(font);
    ipLabel = new QLabel();
    ipLabel->setFont(font);\
    ipLabel->setFrameStyle(QFrame::Panel | QFrame::Plain);
    ipLabel->setLineWidth(3);

    chatLayout->addWidget(roomTextEdit, 1, 1);
    chatLayout->addWidget(clientsTextEdit, 1, 2);
    chatLayout->addWidget(lowerChat, 2, 1);
    chatLayout->addWidget(turnIndicator, 3, 1);
    chatLayout->addWidget(ipLabel, 4, 1);
}

void ChatLog::on_hostButton_clicked()
{
    server = new QTcpServer();
    server2 = new QTcpServer();
    //WinSock Stuff
//    struct addrInfo *result = NULL, *ptr = NULL, hints;
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//    hints.ai_flags = AI_PASSIVE;

//    // Resolve the local address and port to be used by the server
//    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//    if (iResult != 0) {
//        printf("getaddrinfo failed: %d\n", iResult);
//        WSACleanup();
//        return 1;
//    }

    //Qt Socket Stuff

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
            qDebug() << "failed to connect";
        }
    }

    emit became_host();
    ipLabel->setText("Server Address: " + s.localAddress().toString());
//    QFont font = ipLabel->font();
//    font.setPointSize(13);
//    font.setBold(true);
//    ipLabel->setFont(font);
    //displayAddressDialog(s.localAddress().toString());
}

void ChatLog::closeServers() {
    server->close();
    server2->close();
}

void ChatLog::on_joinButton_clicked()
{
    serverName = serverLineEdit->text();

    hostAddress = QHostAddress(serverName);
    portNumber = 4200;
    clientPortNumber = portNumber+1;
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
    ipLabel->setText("Server Address: " + serverName);
}

void ChatLog::on_sayButton_clicked()
{

    QString sayLine = sayLineEdit->text().trimmed();
    if(sayLine.isEmpty())
        return;

    QString message = "C(" + (QTime::currentTime()).toString() + ") " + userName + ": " + sayLine + "\r\n";
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
                playerConnected(message);
            }
            else if(messageType == QChar('S'))
            {
                if(!host)
                    playerConnected(message);
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

    sendMessage("C" + QString(userLineEdit->text() + " has connected!\n"));
    sendMessage("P" + QString(userLineEdit->text()));
}

void ChatLog::changePage()
{
    stackedWidget->setCurrentWidget(chatPage);
    resize(550, 275);
}

void ChatLog::playerConnected(QString playerName)
{
    numPlayers++;
    QString string = QString::number(numPlayers) + ". " + playerName;
    clientsTextEdit->append(string);

    if(host)
        sendMessage("S" + QString(userName));
}

void ChatLog::displayAddressDialog(QString address)
{
    //ipLabel->setText("You have connected to server: " + address);


//    QDialog *dialog = new QDialog();
//    QVBoxLayout *layout = new QVBoxLayout();
//    QLabel *label = new QLabel("You have connected to server: " + address);
//    layout->addWidget(label);
//    dialog->setLayout(layout);
//    dialog->show();
}

quint16 ChatLog::getNumPlayers() {
    return numPlayers;
}

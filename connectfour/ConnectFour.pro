#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T12:32:12
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConnectFour
TEMPLATE = app


SOURCES += main.cpp\
        connectfour.cpp \
    chatlog.cpp \
    server.cpp \
    connectfoursquare.cpp

HEADERS  += connectfour.h \
    chatlog.h \
    server.h \
    connectfoursquare.h

RESOURCES += \
    resources.qrc

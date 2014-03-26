#-------------------------------------------------
#
# Project created by QtCreator 2014-03-25T22:33:26
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebGames
TEMPLATE = app


SOURCES += main.cpp\
    lobbywindow.cpp \
    server.cpp \
    connection.cpp \
    client.cpp \
    peermanager.cpp

HEADERS  += \
    lobbywindow.h \
    server.h \
    connection.h \
    client.h \
    peermanager.h

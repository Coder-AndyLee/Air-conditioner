#-------------------------------------------------
#
# Project created by QtCreator 2015-05-28T21:51:58
#
#-------------------------------------------------

QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ui
TEMPLATE = app
QT += sql

SOURCES += main.cpp\
        mainengine.cpp \
    room.cpp \
    login.cpp

HEADERS  += mainengine.h \
    room.h \
    login.h

FORMS    += \
    mainengine.ui \
    login.ui

#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T17:45:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#INCLUDEPATH += /home/nemi/workspace/libs/opencv-2.4.10/release/include

LIBS += -L. -L/usr/local/cuda-6.5/lib64/ `pkg-config opencv --libs`

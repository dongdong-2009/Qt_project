#-------------------------------------------------
#
# Project created by QtCreator 2017-07-24T09:49:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SplitBlockFileCopy
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    blockfilecopy.cpp \
    workthread.cpp

HEADERS  += widget.h \
    blockfilecopy.h \
    workthread.h

FORMS    += widget.ui

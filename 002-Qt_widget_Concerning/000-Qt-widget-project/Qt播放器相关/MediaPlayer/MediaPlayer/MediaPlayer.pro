#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T19:52:38
#
#-------------------------------------------------

QT       += core gui \
        multimedia \
        multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediaPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mediaplayer.cpp \
    customslider.cpp

HEADERS  += mediaplayer.h \
    customslider.h

FORMS    += mediaplayer.ui

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    product.cpp \
    abstractfactory.cpp

HEADERS += \
    product.h \
    abstractfactory.h

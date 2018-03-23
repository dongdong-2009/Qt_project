TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    product.cpp \
    builder.cpp \
    director.cpp

HEADERS += \
    product.h \
    builder.h \
    director.h

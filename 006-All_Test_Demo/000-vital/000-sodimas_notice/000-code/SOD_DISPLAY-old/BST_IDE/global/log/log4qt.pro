#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T10:55:47
#
#-------------------------------------------------

QT       -= gui

TARGET = log4qt
TEMPLATE = lib

DEFINES += LOG4QT_LIBRARY

INCLUDEPATH += $$PWD/.. \
               $$PWD/
DEPENDPATH += $$PWD/..

SOURCES +=  $$PWD/appenderskeleton.cpp \
            $$PWD/basicconfigurator.cpp \
            $$PWD/consoleappender.cpp \
            $$PWD/dailyrollingfileappender.cpp \
            $$PWD/fileappender.cpp \
            $$PWD/helpers/classlogger.cpp \
            $$PWD/helpers/configuratorhelper.cpp \
            $$PWD/helpers/datetime.cpp \
            $$PWD/helpers/factory.cpp \
            $$PWD/helpers/initialisationhelper.cpp \
            $$PWD/helpers/logerror.cpp \
            $$PWD/helpers/logobject.cpp \
            $$PWD/helpers/logobjectptr.cpp \
            $$PWD/helpers/optionconverter.cpp \
            $$PWD/helpers/patternformatter.cpp \
            $$PWD/helpers/properties.cpp \
            $$PWD/hierarchy.cpp \
            $$PWD/layout.cpp \
            $$PWD/level.cpp \
            $$PWD/log4qt.cpp \
            $$PWD/logger.cpp \
            $$PWD/loggerrepository.cpp \
            $$PWD/loggingevent.cpp \
            $$PWD/logmanager.cpp \
            $$PWD/mdc.cpp \
            $$PWD/ndc.cpp \
            $$PWD/patternlayout.cpp \
            $$PWD/propertyconfigurator.cpp \
            $$PWD/rollingfileappender.cpp \
            $$PWD/simplelayout.cpp \
            $$PWD/spi/filter.cpp \
            $$PWD/ttcclayout.cpp \
            $$PWD/writerappender.cpp \
            $$PWD/varia/debugappender.cpp \
            $$PWD/varia/denyallfilter.cpp \
            $$PWD/varia/nullappender.cpp \
            $$PWD/varia/levelmatchfilter.cpp \
            $$PWD/varia/levelrangefilter.cpp \
            $$PWD/varia/listappender.cpp \
            $$PWD/varia/stringmatchfilter.cpp

HEADERS +=  $$PWD/appender.h \
            $$PWD/appenderskeleton.h \
            $$PWD/basicconfigurator.h \
            $$PWD/consoleappender.h \
            $$PWD/dailyrollingfileappender.h \
            $$PWD/fileappender.h \
            $$PWD/helpers/classlogger.h \
            $$PWD/helpers/configuratorhelper.h \
            $$PWD/helpers/datetime.h \
            $$PWD/helpers/factory.h \
            $$PWD/helpers/initialisationhelper.h \
            $$PWD/helpers/logerror.h \
            $$PWD/helpers/logobject.h \
            $$PWD/helpers/logobjectptr.h \
            $$PWD/helpers/optionconverter.h \
            $$PWD/helpers/patternformatter.h \
            $$PWD/helpers/properties.h \
            $$PWD/hierarchy.h \
            $$PWD/layout.h \
            $$PWD/level.h \
            $$PWD/log4qt.h \
            $$PWD/logger.h \
            $$PWD/loggerrepository.h \
            $$PWD/loggingevent.h \
            $$PWD/logmanager.h \
            $$PWD/mdc.h \
            $$PWD/ndc.h \
            $$PWD/patternlayout.h \
            $$PWD/propertyconfigurator.h \
            $$PWD/rollingfileappender.h \
            $$PWD/simplelayout.h \
            $$PWD/spi/filter.h \
            $$PWD/ttcclayout.h \
            $$PWD/writerappender.h \
            $$PWD/varia/debugappender.h \
            $$PWD/varia/denyallfilter.h \
            $$PWD/varia/nullappender.h \
            $$PWD/varia/levelmatchfilter.h \
            $$PWD/varia/levelrangefilter.h \
            $$PWD/varia/listappender.h \
            $$PWD/varia/stringmatchfilter.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEAA2E99E
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = log4qt.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


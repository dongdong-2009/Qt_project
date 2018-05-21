#include "dateandtime.h"
#include <QDateTime>
#include <QDebug>
#include <QDate>

DateAndTime::DateAndTime(QObject *parent) : QObject(parent)
{
    connect(&mTimer, &QTimer::timeout, this, &DateAndTime::sendDateAndTime);
    mTimer.start(1000);
    connect(&mFloor, &Floor::floorChanged, this, &DateAndTime::autoTest);
    mWeekString = QString();
}

QString DateAndTime::getTimeFormat(bool is12)
{
    QString timelocal = "";
    timelocal.clear();
    if (is12)
    {
        timelocal = QDateTime::currentDateTime().toString("hh:mm a");
        timelocal =  timelocal.split(" ").at(0);
    }
    else
    {
        timelocal = QDateTime::currentDateTime().toString("HH:mm");
    }
    //    qDebug()<< "timelocal = "<< timelocal;
    return timelocal;
}

void DateAndTime::autoTest(int pFloorNum, bool upArrowFlag)
{
    QString tmpFloor = QString::number(pFloorNum);
    emit sigAutoTestChanged(tmpFloor, upArrowFlag);
}

QString DateAndTime::getWeekDay()
{
    QString tmpString;
    tmpString = QDateTime::currentDateTime().toString("ddd");
    qDebug()<<__PRETTY_FUNCTION__<<__LINE__<<"tmpString = "<<tmpString;
    if (tmpString != mWeekString)
    {
        mWeekString = tmpString;
    }
    else
    {
//        qDebug()<<__PRETTY_FUNCTION__<<__LINE__<<"tmpString is same";
        return mWeekString;
    }
    int tmpWeek = 1;
    if (!QString::compare(tmpString, "Mon", Qt::CaseInsensitive))
    {
        tmpWeek = 1;
    }
    else if (!QString::compare(tmpString, "Tue", Qt::CaseInsensitive))
    {
        tmpWeek = 2;
    }
    else if (!QString::compare(tmpString, "Wed", Qt::CaseInsensitive))
    {
        tmpWeek = 3;
    }
    else if (!QString::compare(tmpString, "Thu", Qt::CaseInsensitive))
    {
        tmpWeek = 4;
    }
    else if (!QString::compare(tmpString, "Fri", Qt::CaseInsensitive))
    {
        tmpWeek = 5;
    }
    else if (!QString::compare(tmpString, "Sat", Qt::CaseInsensitive))
    {
        tmpWeek = 6;
    }
    else if (!QString::compare(tmpString, "Sun", Qt::CaseInsensitive))
    {
        tmpWeek = 7;
    }
    qDebug()<<__PRETTY_FUNCTION__<<__LINE__<<"tmpWeek = "<<tmpWeek;
    emit sigWeekChanged(tmpWeek);
    return mWeekString;
}

void DateAndTime::sendDateAndTime()
{
    mStringTime = getTimeFormat(false);
    emit signalTimeAndDate(mStringTime);
    mFloor.autoTestFloorChanage();
    getWeekDay();
}

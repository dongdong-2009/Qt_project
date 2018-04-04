#include "qmlscreena.h"
#include <QDebug>
#include <QTimerEvent>
#include <QDateTime>

QmlScreenA::QmlScreenA()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    qDebug()<<"this = "<<this;
}

QmlScreenA::~QmlScreenA()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
}

QColor QmlScreenA::color() const
{
    return m_currentColor;
}

void QmlScreenA::setColor(const QColor &color)
{
    m_currentColor = color;
    emit colorChanged(m_currentColor);
}

QColor QmlScreenA::timeColor() const
{
    QTime time = QTime::currentTime();
    int r = time.hour();
    int g = time.minute()*2;
    int b = time.second()*4;
    return QColor::fromRgb(r, g, b);
}

QmlScreenA::GenerateAlgorithm QmlScreenA::algorithm() const
{
    return m_algorithm;
}

void QmlScreenA::setAlgorithm(QmlScreenA::GenerateAlgorithm algorithm)
{
    m_algorithm = algorithm;
}

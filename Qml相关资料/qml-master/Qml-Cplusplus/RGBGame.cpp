#include <QTimerEvent>
#include <QDateTime>
#include "RGBGame.h"

RGBGame::RGBGame(QObject *parent)
    :QObject(parent)
    , m_algorithm(RandomRGB)
    , m_currentColor(Qt::black)
    , m_nColorTimer(0)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

RGBGame::~RGBGame()
{

}

QColor RGBGame::color() const
{
    return m_currentColor;
}

void RGBGame::setColor(const QColor &color)
{
    m_currentColor = color;
    emit colorChanged(m_currentColor);
}

QColor RGBGame::timeColor() const
{
    QTime time = QTime::currentTime();
    int r = time.hour();
    int g = time.minute()*2;
    int b = time.second()*4;
    return QColor::fromRgb(r, g, b);
}

GenerateAlgorithm RGBGame::algorithm()
{
    return m_algorithm;
}

void RGBGame::setAlgorithm(GenerateAlgorithm algorithm)
{
    m_algorithm = algorithm;
}

void RGBGame::start()
{
    if (0 == m_nColorTimer)
    {
        m_nColorTimer = startTimer(1000);
    }
}

void RGBGame::stop()
{
    if (0 < m_nColorTimer)
    {
        killTimer(m_nColorTimer);
        m_nColorTimer = 0;
    }
}

void RGBGame::timerEvent(QTimerEvent *e)
{
    if (m_nColorTimer == e->timerId())
    {
        switch (m_algorithm) {
        case RandomRGB:
            m_currentColor.setRgb(qrand()%255, qrand()%255,qrand()%255);
            break;
        case RandomRed:
            m_currentColor.setRed(qrand()%255);
            break;
        case RandomGreen:
            m_currentColor.setGreen(qrand()%255);
            break;
        case RandomBlue:
            m_currentColor.setBlue(qrand()%255);
            break;
        case LinearIncrease:
        {
            int r = m_currentColor.red() + 10;
            int g = m_currentColor.green() + 10;
            int b = m_currentColor.blue() + 10;
            m_currentColor.setRgb(r%255,g%255,b%255);
        }
            break;
        default:
            break;
        }
    }
    else
    {
        QObject::timerEvent(e);
    }
}









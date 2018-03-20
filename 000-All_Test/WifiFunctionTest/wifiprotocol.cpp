#include "wifiprotocol.h"
#include <QDebug>

WifiProtocol::WifiProtocol(QObject *parent) : QObject(parent)
{

}

quint8 WifiProtocol::frameVerify(quint8 *frame, int frameLength)
{
    qDebug()<< __PRETTY_FUNCTION__<<"frameLength = "<< frameLength;
    quint8 result = 0;
    if (frameLength <= 0)
    {
        qDebug()<< "Frame length is error!";
        return result;
    }
    for (int i = 0; i < frameLength; ++i)
    {
        result = result ^ frame[i];
    }
    return result & 0x7f;
}

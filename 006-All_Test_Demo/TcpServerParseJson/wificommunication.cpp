#include "wificommunication.h"
#include <QDebug>
//#include <QString>
//#include <QCoreApplication>

//#include <QJsonDocument>
//#include <QJsonArray>
//#include <QJsonValue>
//#include <QJsonParseError>
//#include <QVariantMap>

#define BVT_STX 0x80        /* 帧起始字符 */
#define BVT_ETX 0x81        /* 帧结束字符 */
#define BVT_ESC 0x1B        /* 转换字符 */

#define BVT_STX_AF 0xE7     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF 0xE8     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF 0x00     /* 转换字符转换后增加字符 */

WifiCommunication::WifiCommunication(QObject *parent) : QObject(parent)
{
    mMyTcpServer = new MyTcpServer();
    if (mMyTcpServer)
    {
        connect(mMyTcpServer, &MyTcpServer::myTcpServerRecvMsg, this, &WifiCommunication::parserJsonFormat);
        connect(mMyTcpServer, &MyTcpServer::myTcpServerRecvMsg, this, &WifiCommunication::sigMsgArrive);
    }
}

bool WifiCommunication::jsonFormatIsRight(const QByteArray &byteArray)
{
    bool flag = false;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError); // 转化为JSON文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) // 解析未发生错误
    {
        flag = true;
    }
    else
    {
        flag = false;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
    return flag;
}

//https://www.cnblogs.com/senior-engineer/p/5608985.html
void WifiCommunication::parserJsonFormat(const QByteArray &byteArray)
{
    if ((byteArray.isNull() || byteArray.isEmpty()))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mJsonString is empty!";
        return ;
    }
    if (!jsonFormatIsRight(byteArray))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the byteArray format is wrong";
        return ;
    }
    QJsonDocument qdoc = QJsonDocument::fromJson(byteArray);
    QJsonObject cmdqobj = qdoc.object();
    QJsonValue value0 = cmdqobj.value(QString("type"));
    QString type = value0.toString();
    qWarning() << type<<"\n";
    if (!QString::compare(type, "login", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {            
            login(cmdqobj);
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "heartbeat", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {

        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "getallparameter", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            getAllParameter();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "setparameter", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            setParameter(cmdqobj);
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "appupdateFinished", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            appUpdateFinished();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd is error";
    }
}

// 解析读取到的QByteArray的数据，需要对帧头和帧尾进行处理
quint32 WifiCommunication::ParseBuffer(QByteArray &buffer)
{
    if (!judgeArrayIsEmpty(buffer))
    {
        return false;
    }
    return true;
}

// 判断QByteArray的数据是否为空
bool WifiCommunication::judgeArrayIsEmpty(const QByteArray &buffer)
{
    bool flag = true;
    if (buffer.isNull() || buffer.isEmpty())
    {
        flag = true;
    }
    else
    {
        flag = false;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<< flag;
    return flag;
}

QByteArray WifiCommunication::sendJsonFrame(QJsonObject& msg)
{
    QJsonDocument document;
    document.setObject(msg);
    QByteArray allPara = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(allPara);
    return allPara;
}

void WifiCommunication::sltloginResult(bool flag)
{
    QJsonObject loginObj;
    QJsonObject info;
    loginObj.insert("type", "login");
    if(flag)
    {
        info.insert("result", true);
    }
    else
    {
        info.insert("result", false);
    }
    loginObj.insert("info", QJsonValue(info));
    QByteArray tmp =  sendJsonFrame(loginObj);
    writeMsgToClient(tmp, tmp.length());
    if (flag)
    {
        emit  sigDeviceConnected();
    }
}

int WifiCommunication::writeMsgToClient(QByteArray msg, int length)
{
    int len = -1;
    if (mMyTcpServer)
    {
        len = mMyTcpServer->writeMsgToClient(msg, length);
    }
    qDebug()<<__PRETTY_FUNCTION__<<"len = "<<len;
    return len;
}

void WifiCommunication::login(QJsonObject &map)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<map;
    QJsonValue infoVal = map["info"];
//    QJsonValue infoVal = cmdqobj.value(QString("info"));
    QJsonObject obj = infoVal.toObject();

    QJsonValue ssidval = obj["ssid"];
    QString ssid = ssidval.toString();
    QJsonValue passwordval = obj["password"];
    QString password = passwordval.toString();

    qDebug()<<__PRETTY_FUNCTION__<<"ssid = "<<ssid<<"passwd = "<<password;
    if(obj.contains(QString("ssid")) && obj.contains(QString("password")))
    {
//        QString ssid = map["ssid"].toString();
//        QString password = map["password"].toString();
        qDebug()<<__PRETTY_FUNCTION__<<"ssid = "<<ssid<<"passwd = "<<password;
        emit sigLogin(ssid, password);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"ssid passwd is lost";
    }
}

void WifiCommunication::getAllParameter()
{
    emit sigGetAllParametere();
}

void WifiCommunication::setParameter(QJsonObject &map)
{
    QVariantMap tmpMap =  map.toVariantMap();
    QMapIterator<QString, QVariant> it(tmpMap);
    while (it.hasNext())
    {
        it.next();
        emit sigParameterSetUp(it.key(), it.value());
    }
}

void WifiCommunication::appUpdateFinished()
{
    emit sigUpdateFinished();
}

void WifiCommunication::heatBeatFrame()
{

}

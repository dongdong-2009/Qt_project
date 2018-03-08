#include "tooltcp.h"

ToolTcpServer::ToolTcpServer(quint16 pPort, QObject *parent)
    :QTcpServer(parent)
{
    if(pPort <= 0)
        pPort = D_NMPORT;
    Open(QHostAddress::Any, pPort);
}

ToolTcpServer::~ToolTcpServer()
{
    Close();
}

bool ToolTcpServer::Open(QHostAddress pIP, quint16 pPort)
{
    Close();
    //>@����ϴ����еĳ��������ⲿ���򣬱���Mplayer���������ر���Display��û�йر�Mpalayer��
    //>@��ô������һ��Ӧ�ó�����Ȼû����ȫ�˳�������ռ�õĶ˿ں�û�б��ͷţ���ɴ˴μ����˿�ʧ�ܡ�
    if(listen(pIP, pPort))
       return true;
    IDE_DEBUG(GBKToUTF8(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
              ">>>>>>�����Ƿ��ϴ����еĳ����в�����>>>>>>\n"
              ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"));
    return false;
}

void ToolTcpServer::Close()
{
    if(isListening())
        close();
}

void ToolTcpServer::incomingConnection(int handle)
{
    ToolTcpClient* tmpSocket = new ToolTcpClient;
    if(!tmpSocket->setSocketDescriptor(handle))
    {
        tmpSocket->deleteLater();
        return;
    }
    tmpSocket->mConnectTime = QDateTime::currentDateTime();
    emit sNewSocket(tmpSocket);
}

/******************************************************
//>@
******************************************************/

ToolTcpClient::ToolTcpClient(uint pType, QObject *parent)
    :QTcpSocket(parent)
{
    mType = pType;
}

//>@ִ��deletelater�����������ǻ��һ��ʱ���Ż�����ִ��~ToolTcpClient()��������ݡ���˲�Ҫ��ToolTcpClientʹ������ָ��QPointer
ToolTcpClient::~ToolTcpClient()
{
    Close();
}

bool ToolTcpClient::isConnected()
{
    if(state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void ToolTcpClient::Close()
{
    disconnect(this, SIGNAL(connected()), this, SLOT(slot_Connected()));
    disconnect(this, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));

    if(state() != QAbstractSocket::UnconnectedState)
        disconnectFromHost();

    if(!(state() == QAbstractSocket::UnconnectedState || waitForDisconnected(1000)))
        IDE_DEBUG(QString("Socket[%1] can't be Closed!").arg(state()));

    if(isOpen())
        QIODevice::close();
}

bool ToolTcpClient::Open(QHostAddress pIP, quint16 pPort, quint32 pTimeout)
{
    if(state() == ConnectedState)
        return true;
    connect(this, SIGNAL(connected()), this, SLOT(slot_Connected()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));
    quint32 delayTime = pTimeout / 2;
    QtSleep(delayTime);
    connectToHost(pIP, pPort);
    quint32 i = 0;
    while(((i+=100) < delayTime) && (state() != ConnectedState))
        QtSleep(100);
    if(state() == ConnectedState)
        return true;
    return false;
}

void ToolTcpClient::Open(QHostAddress pIP, quint16 pPort)
{
    connect(this, SIGNAL(connected()), this, SLOT(slot_Connected()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));
    if((state() != ConnectedState) && (state() != ConnectingState))
        connectToHost(pIP, pPort);
}

int ToolTcpClient::GetConnectedTime()
{
    if(mConnectTime.isValid())
        return qAbs(QDateTime::currentDateTime().secsTo(mConnectTime));
    return -1;
}

void ToolTcpClient::slot_Connected()
{
    mConnectTime = QDateTime::currentDateTime();
    emit sConnState(true);
}

void ToolTcpClient::slot_Disconnected()
{
    emit sConnState(false);
}


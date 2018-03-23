#ifndef TOOLTCP_H
#define TOOLTCP_H

#include "globalfun.h"
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>

typedef QTcpSocket* TcpSocketPtr;
Q_DECLARE_METATYPE(TcpSocketPtr)

class ToolTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    ToolTcpServer(quint16 pPort = D_NMPORT, QObject *parent = 0);
    ~ToolTcpServer();

    bool Open(QHostAddress pIP, quint16 pPort);
    void Close();

    void incomingConnection(int handle);

signals:
    void sNewSocket(QTcpSocket *pSocket);
};

class ToolTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    ToolTcpClient(uint pType = 0, QObject *parent = 0);
    ~ToolTcpClient();

    bool Open(QHostAddress pIP, quint16 pPort, quint32 pTimeout);
    void Open(QHostAddress pIP, quint16 pPort);
    void Close();
    bool isConnected();

    int GetConnectedTime();

public slots:
    void slot_Connected();
    void slot_Disconnected();

signals:
    void sConnState(bool pState);

public:
    QDateTime       mConnectTime; //>@���ڼ�¼�ѽ������ӵ�ʱ��
    uint            mType;        //>@��ʾ�˿ͻ�������;��֡��ʽ(4���ֽ�):uint
};

#endif // NODETCP_H

#ifndef TOOLUDP_H
#define TOOLUDP_H

#include "global.h"
#include <QUdpSocket>
#include <QHostInfo>

//>@�����BST_IDE��BST_DISPLAY���ڱ����Ͻ��е��ԣ�������������ĺ궨�壬���򱾻��޷����������ϵ�DISPLAY
#define NOUDPLOCALFLITER  //>@# UDP�����˲����˳������ź�

class ToolUdp : public QIODevice
{
    Q_OBJECT
public:
    ToolUdp(QObject *parent = 0);
    ~ToolUdp();

    bool OpenUdp(quint16 pPort);
    void Close();
    bool isOpen();
    bool IsLocalIP(QHostAddress pIP);
    void UpdateBroadcastAddresses();

    qint64 readData(char *data, qint64 maxlen) {return -1;}
    qint64 writeData(const char *data, qint64 len) {return -1;}

    qint64 write(QByteArray data, QHostAddress host);
    qint64 write(const QByteArray &data);   //>@�㲥����
    qint64 reply(const QByteArray &data);
    qint64 echo();
    QByteArray readAll();

public:
    quint16             mPort;
    QUdpSocket*         mUdpSocket;
    QList<QHostAddress> mBroadcastIpAddrs;

    QByteArray          m_RevByteArray;
    QHostAddress        m_SenderHost; //>@��ǰ���ݵķ��ͷ���IP
    quint16             m_SenderPort; //>@��ǰ���ݵķ��ͷ���PORT
};

#endif // NODEUDP_H

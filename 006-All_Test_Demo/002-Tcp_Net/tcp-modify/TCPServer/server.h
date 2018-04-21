#ifndef SERVER_H
#define SERVER_H

//�ú�����Ҫ������ʵ��TCPЭ��ķ�����
#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include <QTextCodec>

class Server : public QTcpServer
{
    Q_OBJECT
public:
 //   explicit Server(QObject *parent = 0);
    Server(QObject *parent = 0,int port = 0);
    QList<TCPClientSocket *> tcpClientSocketList;//����һ��TCPClientSocket�б����ڴ洢�ͻ������ӵ�TCPClientSocket
    QTextCodec *codec;

signals:
    void updataServer(QString,int);


public slots:
    void updataClients(QString,int);
    void slotDisconnected(int);

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER_H

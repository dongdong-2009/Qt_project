#ifndef TCPMODULE_H
#define TCPMODULE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);

signals:

public slots:
};

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);

signals:

public slots:
};

#endif // TCPMODULE_H

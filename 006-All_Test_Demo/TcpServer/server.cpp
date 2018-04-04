#include "server.h"
#include <QDebug>

Server::Server(QObject *parent,int port)
    :QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    TcpClientSocket *tcpClientSocket = new TcpClientSocket(this);
    connect(tcpClientSocket, SIGNAL(updateClients(QString, int)), this, SLOT(updateClients(QString, int)));
    connect(tcpClientSocket, SIGNAL(disconnected(int)), this, SLOT(slotDisconnected(int)));

    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);
}

void Server::updateClients(QString msg, int length)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    qDebug()<<__PRETTY_FUNCTION__<<"this pointer = "<< this;
    emit updateServer(msg, length);
    for(int i = 0; i < tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->write(msg.toLatin1(), length) != length)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"length = "<<length;
            continue;
        }
    }
}

void Server::slotDisconnected(int descriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    for(int i = 0; i<tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"descriptor = "<<descriptor;
            tcpClientSocketList.removeAt(i);
            item->deleteLater();
            return;
        }
    }
    return;
}

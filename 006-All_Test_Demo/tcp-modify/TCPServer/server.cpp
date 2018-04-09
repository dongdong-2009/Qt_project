#include "server.h"

Server::Server(QObject *parent, int port)
    :QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
}
//���յ�����Ϣ�㲥��ȥ������Ⱥ��һ����
void Server::updataClients(QString msg, int length)
{
    emit updataServer(msg, length);

    for(int i=0; i<tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        codec = QTextCodec::codecForName("system");
        if(item->write(codec->fromUnicode( msg ), length) != length)
        {
            continue;
        }
    }
}
//�����client�Ͽ����ӣ�������Ҫ��tcpClientSocketList�������ClientSocket��remove��
void Server::slotDisconnected(int descriptor)
{
    for(int i = 0; i < tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            tcpClientSocketList.removeAt(i);
            continue;
        }
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    TCPClientSocket *tcpClientSocket = new TCPClientSocket(this);
    connect(tcpClientSocket, SIGNAL(updataClients(QString, int)),
            this, SLOT(updataClients(QString, int))); //������ִ�еĲۺ���
    connect(tcpClientSocket, SIGNAL(disconnected(int)),
            this, SLOT(slotDisconnected(int))); //�Ͽ����ӵ�ʱ��ִ�еĺ���

    tcpClientSocket->setSocketDescriptor(socketDescriptor); //����ӵ��׽���������ָ������socketDescriptor
    tcpClientSocketList.append(tcpClientSocket); //��tcpClientSocket�����׽����б��У��Ա���á�
}

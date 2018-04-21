#include "tcpclientsocket.h"

TCPClientSocket::TCPClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void TCPClientSocket::dataReceived()
{
    while(bytesAvailable() > 0)
    {
        int length = bytesAvailable();//��ȡ���ݳ���
        char buf[1024];
        read(buf, length);//�������ݣ���������buf ��

        QString msg = buf;
        emit updataClients(msg, length); //��������SIGNAL�ķ�ʽ����
    }
}

void TCPClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}

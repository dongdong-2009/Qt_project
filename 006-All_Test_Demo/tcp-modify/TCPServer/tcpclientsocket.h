#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>

//�ú�����Ҫ�����ǽ���TCP�׽��� ,���ڸ�Clientͨ��

class TCPClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TCPClientSocket(QObject *parent = 0);

signals:
    void updataClients(QString,int);
    void disconnected(int);

public slots:
    void dataReceived();
    void slotDisconnected();

};

#endif // TCPCLIENTSOCKET_H

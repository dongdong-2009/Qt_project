#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>


TCPClient::TCPClient(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("TCP Client");

    textBrowser = new QTextBrowser;
    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton(tr("����"));
    userNameLabel = new QLabel(tr("�û�����"));
    userNameLineEdit = new QLineEdit;
    serverIPLabel = new QLabel(tr("��������ַ��"));
    serverIPLineEdit = new QLineEdit;
    portLabel = new QLabel(tr("�˿ڣ�"));
    portLineEdit = new QLineEdit;
    enterBtn = new QPushButton(tr("����������"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textBrowser, 0, 0, 1, 2);
    mainLayout->addWidget(sendLineEdit, 1, 0);
    mainLayout->addWidget(sendBtn, 1, 1);
    mainLayout->addWidget(userNameLabel, 2, 0);
    mainLayout->addWidget(userNameLineEdit, 2, 1);
    mainLayout->addWidget(serverIPLabel, 3, 0);
    mainLayout->addWidget(serverIPLineEdit, 3, 1);
    mainLayout->addWidget(portLabel, 4, 0);
    mainLayout->addWidget(portLineEdit, 4, 1);
    mainLayout->addWidget(enterBtn, 5, 0, 1, 2);

    status = false; //���ڼ�¼��ǰ��״̬ true ��ʾ�Ѿ�����������
    port = 8080;
    portLineEdit->setText(QString::number(port));
    userNameLineEdit->setText(tr("������"));
    serverIPLineEdit->setText(tr("192.168.200.14"));

    serverIP = new QHostAddress(); //����һ��0.0.0.0

    connect(textBrowser, SIGNAL(cursorPositionChanged()), this, SLOT(autoScroll()));

    connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotEnter()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSend()));

    sendBtn->setEnabled(false);

}

TCPClient::~TCPClient()
{
    delete textBrowser;
    delete sendLineEdit;
    delete sendBtn;
    delete userNameLabel;
    delete userNameLineEdit;
    delete serverIPLabel;
    delete serverIPLineEdit;
    delete portLabel;
    delete portLineEdit;
    delete enterBtn;
    delete mainLayout;
}

void TCPClient::slotEnter()
{
    if(!status)
    {

        QString ip = serverIPLineEdit->text();
        if(!serverIP->setAddress(ip))// ����ĵ�ַ�Ƿ�Ϸ�
        {
            QMessageBox::information(this, "error", "server ip address error!");
            return;
        }

        if(userNameLineEdit->text() == "") //�û����Ƿ�Ϊ��
        {
            QMessageBox::information(this, "error", "user name error!");
            return;
        }
        //���ϵĴ��룬��Ҫ���ж��û������Ƿ�Ϸ���������ǽ������ӡ�

        userName = userNameLineEdit->text();

        tcpSocket = new QTcpSocket(this);//����һ��QTcpSocket�����ٽ���һЩ�źŲۣ��Ͽ������ӡ���������
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

        tcpSocket->connectToHost(*serverIP, port); //�������ӣ��ᷢ��connected�ź�
        status = true;
    }
    else
    {
        int length = 0;
        QString msg = userName + ":" + " Leave Char Room!";
        if((length = tcpSocket->write(msg.toLocal8Bit(),
                                      msg.toLocal8Bit().length())) != msg.toLocal8Bit().length())
        {
            return;
        }
        tcpSocket->disconnectFromHost(); //�Ͽ����ӣ��ᷢ��disconnected �ź�

        status = false;
    }
}


void TCPClient::slotConnected()
{
     enterBtn->setText(tr("�˳�������"));
     sendBtn->setEnabled(true);

     int length = 0;
     codec = QTextCodec::codecForName("system");
     QString msg = userName + ":" + " enter Char Room!";
     if((length = tcpSocket->write(msg.toLocal8Bit(), msg.toLocal8Bit().length())) != msg.toLocal8Bit().length())
     {
         return;
     }

}

void TCPClient::slotDisconnected()
{
    enterBtn->setText(tr("����������"));
    sendBtn->setEnabled(false);

}

void TCPClient::dataReceived()
{
    while(tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());

        tcpSocket->read(datagram.data(), datagram.size());

        QString msg = datagram.data();
        textBrowser->insertPlainText(msg.left(datagram.size()) + "\n");
    }
}

void TCPClient::slotSend()
{
    if(sendLineEdit->text() == "")
    {
        return;
    }

    QString msg = userName+":"+sendLineEdit->text();
 //   userNameLabel->setText(QString::number(msg.toLocal8Bit().length()));
    tcpSocket->write(msg.toLocal8Bit(), msg.toLocal8Bit().length());
    sendLineEdit->clear();
}

void TCPClient::autoScroll()
{
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);
}



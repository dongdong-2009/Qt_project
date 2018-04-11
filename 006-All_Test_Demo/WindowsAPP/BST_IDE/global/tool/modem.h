#ifndef MODEM_H
#define MODEM_H

#include "global.h"
#include "serialport.h"

#define SOH  0x01 //SOH: ��ͷ
#define EOT  0x04 //EOT: �����ļ�����
#define ACK  0x06 //ACK: ������Ӧ���磺���ݰ���ȷ����
#define NACK  0x15 //NACK: ��������Ӧ
#define CAN  0x18 //CAN: ȡ���ļ�����
#define XMC  0x43 //C: ASCII�ַ�C

class Modem : public QObject
{
    Q_OBJECT
public:
    Modem(QIODevice *pDev, QObject *parent = 0);
    ~Modem();

    virtual void Init(QIODevice *pDev);
    void Release();

    void Stop();
    QByteArray read(int pMaxNum, int pTimeout);

    bool SendIAP(int pTimeout);
    bool Send(QByteArray pArray, int pTimeout);
    bool SendFile(QString pFile, int pTimeout);
    bool SendEnd(int pTimeout);

    bool GetIAP(int pTimeout);
    QByteArray Get(bool &pEnd, int pTimeout);
    bool GetFile(QString pFile, int pTimeout);

signals:
    void sProgress(int pValue);
    void sFinished();

public:
    QPointer<QIODevice>       m_Device;

    char                     *m_ModemFlag;
    char                     *m_ModemAck;
    char                     *m_FrameHead;
    char                     *m_FrameEnd;
    char                     *m_FrameBuffer;
    char                     *m_FrameAck;
    int                       m_FrameSize;  //>@֡��С

    bool                      m_StopFlag;
    FRAME_STATE               m_FrameState;
};

#endif // MODEM_H

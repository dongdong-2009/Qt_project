#ifndef PHYSICALNETWORK_H
#define PHYSICALNETWORK_H

#include "physicalprotocol.h"
#include "tooltcp.h"
#include "tooludp.h"
#include <QHostAddress>

class PhysicalLANClient : public PhysicalHost
{
    Q_OBJECT
public:
    PhysicalLANClient(ToolTcpClient *pTcpClient, QObject *pParent = 0);
    PhysicalLANClient(QString pAddr=QString(), QString pBufferPath=QString(), QObject *pParent = 0);
    ~PhysicalLANClient();

    void SetDestoryFlag(bool pFlag) {mDestoryFlag = pFlag;}

    bool CanAutoConnect();

    void SetTcpClient(ToolTcpClient *pTcpClient, bool pSync = false, quint32 pSyncDelay = 100);  //pSync��ʾ��ǰ��SetTcpClient�����Ƿ���Ҫͬ��һ����������߽��棬һ��ֻ��PhysicalLANClient����TcpClientʱ�Ż�ʹ��true

    bool Sync(quint32 pTimeout = 0);
    void Async();
    void Async(bool pSync);
    bool isConnected();
    bool isSync(quint32 pTimeout = 0);
    int  GetSyncTime();

    qint64 write(const QByteArray &data);
    qint64 reply(QByteArray data);
    QByteArray readAll();

    void UpdateDevAddr();    //>@֧��10.200.19.1:8080����10.200.19.1���ָ�ʽ

    //>@�Ƚ�boot�Լ�map�ļ���������ǰ�����У�������
    bool Download(QString pThemeName, QString pThemePath=QString());
    bool Upload(QString pThemeName);

    bool DownloadPreference(QString pMdpName);
    bool UploadPreference(QString pMdpName);

signals:
    void sAutoConnect(bool pFlag);

public slots:
    void slot_ConnState(bool pFlag);
    void slot_ReadData();
    void slot_UpdateConnState();
    void slot_HBTimeout(quint32 pID);

public:
    QHostAddress                 mIP;
    quint16                      mPort;

    QString                      mCode;
    QString                      mPasswd;
    QString                      mLocation;

    bool                         mDestoryFlag;

    ToolTcpClient               *mTcpClient;
};

class PhysicalLANServer : public PhysicalProtocol
{
    Q_OBJECT
public:
    PhysicalLANServer(QObject *pParent = 0);
    ~PhysicalLANServer();

    virtual quint32 InitPara();
    virtual QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    virtual void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

    quint32 Monitor(quint32 pCount);

    bool isConnected();  //>@ָ�ͻ�������û�������ŵ�

    //>@�������ӣ����洢��mClients����������һ��PhysicalLANClient
    PhysicalLANClient* Connect(QHostAddress pAddr, quint16 pPort, quint32 pTimeout = 0);

    bool AddClient(PhysicalLANClient *pLanClient);
    PhysicalLANClient* AddClient(QHostAddress pAddr, quint16 pPort, bool &pNewConn);
    bool DelClient(QHostAddress pAddr, quint16 pPort);
    void DelClient(PhysicalLANClient *pLanClient);

signals:
    void sNetState(bool pFlag);
    void sAbortAll();

public slots:
    virtual void slot_Initialize();
    virtual void slot_InitNetwork();
    void slot_ReleaseNetwork();

    virtual void slot_NewSocket(QTcpSocket *pSocket);
    virtual void slot_ConnState(bool pFlag);

    void slot_ReadData();

public:
    bool                         mShowMessage;
    int                          mMonitorCount;

    QString                      mNetName;
    QHostAddress                 mIP;   //>@�����ķ�������IP
    bool                         mDHCP;
    NUM_INFO                     mTCPSPort;
    NUM_INFO                     mUDPPort;

    ToolUdp                     *mUdpDevice;
    ToolTcpServer               *mTcpServer;    //>@���ڽ���������������ע��Ӧ��
    QList<PhysicalLANClient*>    mClients;      //>@�ⲿ���������ͬ���οͻ���Ⱥ
};

class WifiOperate : public QThread
{
    Q_OBJECT
public:
    WifiOperate(QObject *parent = 0);
    ~WifiOperate();

    bool Open(QHostAddress pIpAddr, bool pForce=false);
    bool Close(bool pForce=false);

    void ForceQuit();
    void Wait(int pTimeout);
    void run();

public:
    int                        m_OperateType;
    QHostAddress               m_IpAddr;
};

/*
  רָ�����ϵ���ʾ���豸
    ������WIFI AP�Ĺ���
    �Զ����ӷ�����
*/
class PhysicalLANC : public PhysicalLANServer
{
    Q_OBJECT
public:
    PhysicalLANC(QObject *pParent = 0);
    ~PhysicalLANC();
    bool PrepareRelease();

    quint32 InitPara();
    QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

    bool SetValue(QString pValue);

    void OpenWIFI(bool pForce=false);
    void CloseWIFI(bool pForce=false);

    quint32 Monitor(quint32 pCount);

    void Connected();
    void Disconnected();

    //>@��д��������PhysicalUdp��Ϊ�ӻ�ʱ�Ĳ���
    qint64 write(const QByteArray &data);
    qint64 reply(QByteArray data);
    QByteArray readAll();

public slots:
    void slot_InitNetwork();

    void slot_Connect();
    void slot_RouterConnState(bool pFlag);

    void slot_ConnState(bool pFlag);

public:
    //>@WIFIӦ��
#ifdef SUPPORT_MUTEX
    QMutex                     *m_WifiMutex;
#else
    bool                        m_WifiMutex;
#endif
    WifiOperate                 m_WifiOperate;
    bool                        m_WifiOpened;
    bool                        m_WifiBootON;   //>@�Ƿ񿪻�����
    QHostAddress                m_WifiIpAddr;

    PhysicalLANClient*          m_RouterClient;      //>@��������Router��������
    WaitInfo                    m_RouterWait;
    QHostAddress                m_RouterIpAddr;
    NUM_INFO                    m_RouterPort;
    NUM_INFO                    m_RouterTimeout; //>@�Զ�������ʱ����
};

/*
  רָС���������������豸����������
    ������ƶˣ���Ϊ�ͻ���
    �������������������ȴ��ƶ˵Ŀͻ�������
         �������������ȴ���������ʾ��NAT����������
         �����ͻ���Ⱥ����������ͬ���ε���ʾ����ָһЩ�Ǳ����ʾ���������������豸��
         UDP��ͬ����ɨ�裬����Ӷ�̬���ӵ��豸
  ע�����
    1�����ͬһ���豸ͬʱ��LANC/LANR����ôֻ����һ���ط���IP��ַ��Ҳ��
            <LANC>
                <UDP Para="num" List="60000;65000">60001</UDP>
                <TCPS Para="num" List="60000;65000">60002</TCPS>
                <IP Para="string">192.168.1.1</IP>
            </LANC>
            <LANR>
                <UDP Para="num" List="60000;65000">60011</UDP>
                <TCPS Para="num" List="60000;65000">60012</TCPS>
            </LANR>
    2��
*/
class PhysicalLANR : public PhysicalLANServer
{
    Q_OBJECT
public:
    PhysicalLANR(QObject *parent = 0);
    ~PhysicalLANR();

    //>@ʹ��UDP����ɨ��ͬ�����豸
    QStringList Scan(QString pContent, quint32 pTimeout);
    QList<PhysicalLANClient*> ScanDevices(quint32 pTimeout);  //>@���ش˴����ӵ�����

    quint32 Monitor(quint32 pCount);

    QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

public slots:
    void slot_InitNetwork();

    void slot_Connect();
    void slot_ServerConnState(bool pFlag);

    void slot_Init(QVariant);

public:
    //>@��������
    PhysicalLANClient          *m_ServerClient;       //>@�����ͻ���
    WaitInfo                    m_ServerWait;
    QHostAddress                m_ServerIpAddr;
    NUM_INFO                    m_ServerPort;
    NUM_INFO                    m_ServerTimeout; //>@�Զ�������ʱ����
};

#endif // PHYSICALNETWORK_H

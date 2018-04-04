#include "physicalnet.h"
#include "logicbase.h"
#include "deviceinc.h"

PhysicalLANClient::PhysicalLANClient(ToolTcpClient *pTcpClient, QObject *pParent):
    PhysicalHost(QString(), QString(), pParent)
{
    mTcpClient = 0;
    connect(this, SIGNAL(sAutoConnect(bool)), this, SLOT(slot_ConnState(bool)), Qt::QueuedConnection);
    mDestoryFlag = true;
    mPort = D_NMPORT;
    mDevType = TYPE_NET;
    SetTcpClient(pTcpClient);
}

PhysicalLANClient::PhysicalLANClient(QString pAddr, QString pBufferPath, QObject *parent):
    PhysicalHost(pAddr, pBufferPath, parent)
{
    mTcpClient = 0;
    connect(this, SIGNAL(sAutoConnect(bool)), this, SLOT(slot_ConnState(bool)), Qt::QueuedConnection);
    mDestoryFlag = true;
    mPort = D_NMPORT;
    mDevType = TYPE_NET;
}

PhysicalLANClient::~PhysicalLANClient()
{
    if(mDestoryFlag)
        Async();
}

bool PhysicalLANClient::CanAutoConnect()
{
    return (mHostFlag && mAutoConnFlag);  //>@���ΪHost������ʹ�����Զ�����
}

void PhysicalLANClient::SetTcpClient(ToolTcpClient *pTcpClient, bool pSyncParUI, quint32 pSyncDelay)
{
    if(mTcpClient)
    {
        if(mTcpClient == pTcpClient)
            return;
        mTcpClient->Close();
        mTcpClient->deleteLater();
        mTcpClient = 0;
    }
    mTcpClient = pTcpClient;
    if(mTcpClient)
    {
        if(mTcpClient->isConnected())
        {
            mIP = mTcpClient->peerAddress();
            mPort = mTcpClient->peerPort();
        }

        disconnect(mTcpClient, SIGNAL(readyRead()), 0, 0);
        disconnect(mTcpClient, SIGNAL(sConnState(bool)), 0, 0);

        connect(mTcpClient, SIGNAL(readyRead()), this, SLOT(slot_ReadData()));
        connect(mTcpClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ConnState(bool)));

        SetCurDev(P_TCP, mTcpClient);

        if(pSyncParUI)
        {
            if(pSyncDelay == 0)
                slot_UpdateConnState();
            else
                QTimer::singleShot(pSyncDelay, this, SLOT(slot_UpdateConnState()));
        }
    }
}

void PhysicalLANClient::slot_UpdateConnState()
{
    bool tmpFlag = isConnected();
    if(tmpFlag)
        Connected();
    else
        Disconnected();
    emit sConnState(tmpFlag);
}

bool PhysicalLANClient::isConnected()
{
    if(mTcpClient)
        return mTcpClient->isConnected();//>@�˴�����ʹ��m_Connected��־λ���˱�־λ����protocol�е��ж�
    return false;
}

bool PhysicalLANClient::Sync(quint32 pTimeout)
{
    if(!mTcpClient)
    {
        mTcpClient = new ToolTcpClient;
        connect(mTcpClient, SIGNAL(readyRead()), this, SLOT(slot_ReadData()));
        connect(mTcpClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ConnState(bool)));
        SetCurDev(P_TCP, mTcpClient);
    }
    if(!isConnected())
    {
        if(mPort == 0)
            mPort = D_NMPORT;
        SetHostFlag(true);
        SeteAutoConnect(true);
        if(pTimeout == 0)
            mTcpClient->Open(mIP, mPort);
        else
            return mTcpClient->Open(mIP, mPort, pTimeout);
    }
    return true;
}

void PhysicalLANClient::Async(bool pSync)
{
    if(pSync)
        PhysicalHost::Async();
    if(mTcpClient)
    {
        disconnect(mTcpClient, SIGNAL(readyRead()), 0, 0);
        disconnect(mTcpClient, SIGNAL(sConnState(bool)), 0, 0);
        mTcpClient->Close();
        mTcpClient->deleteLater();
        mTcpClient = 0;

        IDE_DEBUG(QString("%1:%2 Async!").arg(mIP.toString()).arg(mPort));
    }
}

void PhysicalLANClient::Async()
{
    PhysicalHost::Async();
    if(mTcpClient)
    {
        disconnect(mTcpClient, SIGNAL(readyRead()), 0, 0);
        disconnect(mTcpClient, SIGNAL(sConnState(bool)), 0, 0);
        mTcpClient->Close();
        mTcpClient->deleteLater();
        mTcpClient = 0;

        IDE_DEBUG(QString("%1:%2 Async!").arg(mIP.toString()).arg(mPort));
    }
}

bool PhysicalLANClient::isSync(quint32 pTimeout)
{
    int i = 0;
    while(!isConnected() && ((i+=100) < pTimeout))
    {
        QtSleep(100);
    }
    return isConnected();
}

int PhysicalLANClient::GetSyncTime()
{
    if(mTcpClient)
        return mTcpClient->GetConnectedTime();
    return -1;
}

void PhysicalLANClient::slot_ConnState(bool pFlag)
{
    if(pFlag)
    {
        if(!m_Connected)   //>@���ظ����ͽ������ӵ��ź�
        {
            Connected();
            emit sConnState(pFlag);
        }
    }
    else
    {
        if(!mAutoConnState)
        {
            mAutoConnState = true;
            if(mHostFlag)  //>@�������ӱ��˵�Socket���Զ�����
            {
                mAutoConnLog = true;
                m_RetransFile = true;
                bool tmpFlag = false;
                //>@�ٴ��ж��Ƿ�����Ѿ��Ͽ�������
                if(mTcpClient)
                    tmpFlag = mTcpClient->isConnected();

                int tmpFreqBk = m_HBFreq[2].mNum;
                if(!tmpFlag)  //>@���������У������޸�m_Connected��ֵ
                {
                    //>@���������Ĺ�������Ҫ��������
                    m_HBFreq[2].mNum = 0;
                    //>@
                    for(int i=0;i<3 && !tmpFlag && m_Connected;i++)
                        tmpFlag = Sync(300);
                }
                if(!tmpFlag && CanAutoConnect())
                {
                    mAutoConnFlag = false;//>@����Զ�����ʧ�ܣ���λ�Զ�������־λ

                    if(m_Connected)   //>@ֻ����ִ��һ��Disconnected
                    {
                        Disconnected();
                        emit sConnState(pFlag);
                    }
                }
                else
                {
                    m_HBCount[2] = 0;
                    m_HBFreq[2].mNum = tmpFreqBk;
                    m_HBWait[2].update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                    emit sHB(m_HBFreq[2].mNum);
                }
            }
            else                //>@���������ɵı������ӵ�Socket
            {
                //>@����Socket����֧���Զ����������Եȴ�
                bool tmpFlag = false;
                //>@�ȴ������Ĺ�������Ҫ��������������������ӣ�������Socket�ᷢ����������num�ġ�
                m_HBFreq[1].mNum = 0;
                for(int i=0;i<3 && !tmpFlag;i++)
                    tmpFlag = isSync(300);

                if(!tmpFlag)
                {
                    if(m_Connected)   //>@ֻ����ִ��һ��Disconnected
                    {
                        Disconnected();
                        emit sConnState(pFlag);
                    }
                }
            }
            mAutoConnState = false;
        }
    }
}

void PhysicalLANClient::slot_HBTimeout(quint32 pID)
{
    if(pID == 1)            //>@���һ��ʱ��û�н��յ��ⲿAPP�����������ź�
    {
        Async(false);  //>@�˴���û��ȷ��һ���Ͽ������ˣ���˲���ִ��disconnected����
        emit sAutoConnect(false);
    }
    else if(pID == 2)       //>@���һ��ʱ��û�н��յ���ʾ����Ӧ�������ź�(�����ʱ�����m_TcpSocket��ȡ�����ӵĵ�ʵ�����Ѿ��Ͽ������⡣)
    {
        //>@���֮ǰ���Զ�����������ֱ�ӶϿ����ӣ��������Զ�����һ��
        if(!mAutoConnLog)
        {
            emit sAutoConnect(false);
        }
        else
        {
            Async();
            emit sConnState(false);
        }
    }
}

qint64 PhysicalLANClient::write(const QByteArray &data)
{
    if(isConnected())
    {
        return mTcpClient->write(data);
    }
    return -1;
}

qint64 PhysicalLANClient::reply(QByteArray data)
{
    if(isConnected())
    {
        return mTcpClient->write(data);
    }
    return -1;
}

QByteArray PhysicalLANClient::readAll()
{
    if(isConnected())
    {
        return mTcpClient->readAll();
    }
    return QByteArray();
}

void PhysicalLANClient::slot_ReadData()
{
    ParseFrameBuffer(P_TCP, VAR_CAST<QIODevice *>(sender()), readAll());
}

void PhysicalLANClient::UpdateDevAddr()
{
    if(isSync())
    {
        mIP = mTcpClient->peerAddress();
        mPort = mTcpClient->peerPort();
    }
    mDevAddr = QString("%1:%2:%3").arg(mIP.toString()).arg(mPort).arg(mCode);
}

bool PhysicalLANClient::Download(QString pThemeName, QString pThemePath)   //>@�˴���ThemeName������·��
{
    if(!isSync())
        return false;
    if(pThemeName.isEmpty())
        return false;
    mIsBusy = true;
    //>@���ص���ǰ���̴������OutPutĿ¼��
    QString tmpSrcPath = pThemePath + pThemeName;
    if(GetDir(tmpSrcPath, mBufferPath))
    {
        mIsBusy = false;
        return true;
    }
    mIsBusy = false;
    return false;
}

//>@���ϵͳ�Ѿ�����ͬ�����⣬�򸲸ǣ�����ֱ���ϴ�
bool PhysicalLANClient::Upload(QString pThemeName)
{
    if(!isSync())
        return false;
    return false;
}

bool PhysicalLANClient::DownloadPreference(QString pMdpName)
{
    if(!isSync())
        return false;
    return false;
}

bool PhysicalLANClient::UploadPreference(QString pMdpName)
{
    if(pMdpName.isEmpty())
        return false;
    if(!isSync())
        return false;
    return false;
}

PhysicalLANServer::PhysicalLANServer(QObject *parent):
    PhysicalProtocol(parent)
{
    mShowMessage = true;

    mMonitorWait.SetTimeout(10);

#ifdef WINDOWS
    mNetName = GBKToUTF8("��������");
    mIP = QHostAddress("10.200.23.158");
#else
    mNetName = QString("eth0");
    mIP = QHostAddress("192.168.0.10");
#endif

    mTCPSPort.setnum(D_NMPORT, 60000, 65000);
    mUDPPort.setnum(D_CMDPORT, 60000, 65000);

    mUdpDevice = 0;
    mTcpServer = 0;
    mDHCP = false;

    PhysicalProtocol::Connected();
}

PhysicalLANServer::~PhysicalLANServer()
{
    slot_ReleaseNetwork();

    QString tmpCMD = QString("ifconfig eth0 down &");
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
}

quint32 PhysicalLANServer::InitPara()
{
    quint32 tmpRet = PhysicalProtocol::InitPara();
#ifdef WINDOWS
    SetNetwork(mNetName, false, mIP, QHostAddress("255.255.0.0"), QHostAddress("10.200.0.254"), QHostAddress("10.200.0.1"));
#else
    QString tmpCMD;
    if(mDHCP)
    {
        QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Physical/LANC/udhcpd.conf");
        SetDHCP(tmpDHCPFile, mIP.toString());

        tmpCMD = QString("ifconfig %1 %2 up | route add default dev %1 | udhcpd -f %3 &").arg(mNetName).arg(mIP.toString()).arg(tmpDHCPFile);
    }
    else
    {
        tmpCMD = QString("ifconfig %1 %2 up &").arg(mNetName).arg(mIP.toString());
    }
    system(tmpCMD.toLatin1().data());
#endif
    return tmpRet;
}

QHBoxLayout* PhysicalLANServer::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = 0;
    if(!pName.compare("TCPS", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mTCPSPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, mTCPSPort.mNum, mTCPSPort.mMinNum, mTCPSPort.mMaxNum);
    }
    else if(!pName.compare("UDP", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mUDPPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, mUDPPort.mNum, mUDPPort.mMinNum, mUDPPort.mMaxNum);
    }
    else if(!pName.compare("IP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        mIP = QHostAddress(tmpValue);
        if(!mIP.isNull())
        {
            tmpHBoxLayout = CreateTextEdit(pName, tmpValue);
        }
    }
    else if(!pName.compare("DHCP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        mDHCP = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("NetName", Qt::CaseInsensitive))
    {
        mNetName = pElement.text();
        tmpHBoxLayout = CreateTextEdit(pName, mNetName);
    }
    return tmpHBoxLayout;
}

void PhysicalLANServer::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    if(!pName.compare("TCPS", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mTCPSPort.mNum, tmpComponent->value(), mTCPSPort.mMinNum, mTCPSPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("UDP", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mUDPPort.mNum, tmpComponent->value(), mUDPPort.mMinNum, mUDPPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("IP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = mIP.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                mIP = QHostAddress(tmpValue);
                QString tmpCMD;
                if(mDHCP)
                {
                    QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Physical/LANC/udhcpd.conf");
                    SetDHCP(tmpDHCPFile, tmpValue);

                    tmpCMD = QString("ifconfig eth0 down | ifconfig eth0 %1 up | udhcpd -f % &").arg(mIP.toString()).arg(tmpDHCPFile);
                }
                else
                {
                    tmpCMD = QString("ifconfig eth0 down | ifconfig eth0 %1 up &").arg(mIP.toString());
                }
                IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
                system(tmpCMD.toLatin1().data());
#endif
            }
        }
    }
    else if(!pName.compare("DHCP", Qt::CaseInsensitive))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, mDHCP, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!pName.compare("NetName", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, pElement, mNetName, tmpComponent->text()))
            {
            }
            else
            {
            }
        }
    }
}

quint32 PhysicalLANServer::Monitor(quint32 pCount)
{
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient)
        {
            tmpTcpClient->Monitor(pCount);
        }
    }
}

void PhysicalLANServer::slot_Initialize()
{
    PhysicalProtocol::slot_Initialize();
    QTimer::singleShot(1000, this, SLOT(slot_InitNetwork()));
}

void PhysicalLANServer::slot_ReleaseNetwork()
{
    if(mUdpDevice)
    {
        mUdpDevice->deleteLater();
        mUdpDevice = 0;
    }
    if(mTcpServer)
    {
        mTcpServer->deleteLater();
        mTcpServer = 0;
    }
}

void PhysicalLANServer::slot_InitNetwork()
{
    if(!mUdpDevice)
    {
        mUdpDevice = new ToolUdp;
        mUdpDevice->OpenUdp(mUDPPort.mNum);
        connect(mUdpDevice, SIGNAL(readyRead()), this, SLOT(slot_ReadData()), Qt::QueuedConnection);
    }
    if(!mTcpServer)
    {
        mTcpServer = new ToolTcpServer(mTCPSPort.mNum);
        connect(mTcpServer, SIGNAL(sNewSocket(QTcpSocket*)), this, SLOT(slot_NewSocket(QTcpSocket*)));
    }
    if(m_DevManager)
        mMonitorWait.update(mMonitorCount, m_DevManager->m_DevMonitorFreq);
}

bool PhysicalLANServer::isConnected()
{
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient && tmpTcpClient->isConnected())
        {
            IDE_DEBUG(QString("%1:%2 is connected!").arg(tmpTcpClient->mIP.toString()).arg(tmpTcpClient->mPort));
            return true;
        }
    }
    return false;
}

void PhysicalLANServer::slot_NewSocket(QTcpSocket *pSocket)
{
    bool pNewConn = true;
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        //>@��Ϊ�Ƿ������ˣ�����ֻҪ�Ƚ�IP��ַ����(Ĭ��һ��IPֻ������һ��)
        if(tmpTcpClient && (tmpTcpClient->mIP == pSocket->peerAddress()))
        {
            pNewConn = false;
            break;
        }
    }
    if(pNewConn)  //>@�����ǰ��������û��ĳ�����ӣ�������
    {
        tmpTcpClient = new PhysicalLANClient;
        tmpTcpClient->SetDeviceManager(m_DevManager);
        connect(tmpTcpClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ConnState(bool)));
        mClients.append(tmpTcpClient);
        tmpTcpClient->slot_Initialize();
        tmpTcpClient->SetHostFlag(false);
    }
    tmpTcpClient->SetTcpClient(VAR_CAST<ToolTcpClient*>(pSocket), true, 0);  //>@ִ�к���������״̬�жϲ�����sConnState�ź�
}

void PhysicalLANServer::slot_ConnState(bool pFlag)
{
    PhysicalLANClient *tmpTcpClient = VAR_CAST<PhysicalLANClient*>(sender());
    if(tmpTcpClient)
    {
        if(pFlag)
        {
            Connected();
            if(mShowMessage) //>@��Msg����ʾһά��
            {
                emit sInfo(QString("%1:%2 Connected!").arg(tmpTcpClient->mIP.toString()).arg(tmpTcpClient->mPort), 2000);
            }
        }
        else
        {
            Disconnected();
            if(mShowMessage)  //>@��Msg����ʾһά��
            {
                emit sInfo(QString("%1:%2 Disconnected!").arg(tmpTcpClient->mIP.toString()).arg(tmpTcpClient->mPort), 2000);
            }
        }
        emit sNetState(pFlag);
    }
}

bool PhysicalLANServer::AddClient(PhysicalLANClient *pLanClient)
{
    if(!mClients.contains(pLanClient))
        mClients.append(pLanClient);
    return true;
}

PhysicalLANClient* PhysicalLANServer::AddClient(QHostAddress pIP, quint16 pPort, bool &pNewConn)
{
    pNewConn = true;
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient && (tmpTcpClient->mIP == pIP) && (tmpTcpClient->mPort == pPort))
        {
            pNewConn = false;
            break;
        }
    }
    if(pNewConn)  //>@�����ǰ��������û��ĳ�����ӣ�������
    {
        tmpTcpClient = new PhysicalLANClient;
        tmpTcpClient->SetDeviceManager(m_DevManager);
        tmpTcpClient->mIP = pIP;
        tmpTcpClient->mPort = pPort;
        tmpTcpClient->slot_Initialize();
        mClients.append(tmpTcpClient);
    }
    return tmpTcpClient;
}

void PhysicalLANServer::DelClient(PhysicalLANClient *pLanClient)
{
    if(pLanClient)
    {
        if(mClients.contains(pLanClient))
        {
            mClients.removeOne(pLanClient);
        }
        pLanClient->deleteLater();
    }
}

PhysicalLANClient* PhysicalLANServer::Connect(QHostAddress pIP, quint16 pPort, quint32 pTimeout)
{
    PhysicalLANClient *tmpTcpClient = new PhysicalLANClient;
    tmpTcpClient->SetDeviceManager(m_DevManager);
    tmpTcpClient->mIP = pIP;
    tmpTcpClient->mPort = pPort;
    tmpTcpClient->slot_Initialize();
    tmpTcpClient->Sync(pTimeout);
    return tmpTcpClient;
}

bool PhysicalLANServer::DelClient(QHostAddress pIP, quint16 pPort)
{
    bool find = false;
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient)
        {
            if(tmpTcpClient->mIP == pIP && tmpTcpClient->mPort == pPort)
            {
                find = true;
                break;
            }
        }
    }
    if(find)
        DelClient(tmpTcpClient);
    return find;
}

void PhysicalLANServer::slot_ReadData()
{
    ParseFrameBuffer(P_UDP, VAR_CAST<QIODevice*>(sender()), readAll());
}

WifiOperate::WifiOperate(QObject *parent) :
    QThread(parent)
{
    m_OperateType = -1;
}

WifiOperate::~WifiOperate()
{
    ForceQuit();
    wait(1000);
}

void WifiOperate::ForceQuit()
{
    quit();
}

void WifiOperate::Wait(int pTimeout)
{
    quint32 i=0;
    while((i+=10) < pTimeout)
        msleep(10);
}

bool WifiOperate::Open(QHostAddress pIpAddr, bool pForce)
{
    static bool busy = false;
    if(busy)
        return false;
    busy = true;
    if(isRunning())
    {
        if(pForce)
        {
            ForceQuit();
            QtSleep(1000);
        }
        else
        {
            IDE_TRACE();
            busy = false;
            return false;
        }
    }
    m_IpAddr = pIpAddr;
    m_OperateType = 1;
    start(LowestPriority);
    busy = false;
}

bool WifiOperate::Close(bool pForce)
{
    static bool busy = false;
    if(busy)
        return false;
    busy = true;
    if(isRunning())
    {
        if(pForce)
        {
            ForceQuit();
            QtSleep(1000);
        }
        else
        {
            IDE_TRACE();
            busy = false;
            return false;
        }
    }
    m_OperateType = 2;
    start(LowestPriority);
    busy = false;
}

void WifiOperate::run()
{
    switch(m_OperateType)
    {
        case 1:  //>@open
        {
            QString tmpCMD = QString("ifconfig ra0 %1 up | route add default dev ra0 | udhcpd -f /usr/bst/SYSTEM/Middleware/WIFI/udhcpd.conf &").arg(m_IpAddr.toString());
            IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
            system(tmpCMD.toLatin1().data());
#endif
            break;
        }
        case 2:   //>@close
        {
            QString tmpCMD = QString("ifconfig ra0 down | killall udhcpd &");
            IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
            system(tmpCMD.toLatin1().data());
#endif
            break;
        }
        default:
        {
            break;
        }
    }
}

PhysicalLANC::PhysicalLANC(QObject *parent):
    PhysicalLANServer(parent)
{
    mDevType = P_LANC;

    m_RouterClient = 0;
    mMonitorWait.SetTimeout(0);  //>@Ĭ�ϲ��Զ���������
    m_RouterTimeout.setnum(0, 0, 60000);
    m_RouterPort.setnum(D_NMPORT, 60000, 65000);

    m_WifiIpAddr = QHostAddress("192.168.1.10");
    m_WifiBootON = false;
    mShowMessage = !m_WifiBootON;

    m_WifiOpened = false;
#ifdef SUPPORT_MUTEX
    m_WifiMutex = new QMutex(QMutex::Recursive);
#else
    m_WifiMutex = false;
#endif

#ifdef DISPLAY
    SYSTEM_CMD::SetWIFISSID(m_DevManager->mDevCode);
#endif
}

PhysicalLANC::~PhysicalLANC()
{
    CloseWIFI();

#ifdef SUPPORT_MUTEX
    if(m_WifiMutex)
        delete m_WifiMutex;
#else
    m_WifiMutex = false;
#endif
}

quint32 PhysicalLANC::InitPara()
{
    quint32 tmpRet = PhysicalLANServer::InitPara();
    if(m_WifiBootON)
        OpenWIFI();
    return tmpRet;
}

QHBoxLayout* PhysicalLANC::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = PhysicalLANServer::InitSubPara(pName, pElement);
    if(tmpHBoxLayout)
        return tmpHBoxLayout;
    if(!pName.compare("WIFION", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_WifiBootON = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        mShowMessage = !m_WifiBootON;
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("WIFIIP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_WifiIpAddr = QHostAddress(tmpValue);
        tmpHBoxLayout = CreateTextEdit(pName, tmpValue);

        //QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Middleware/WIFI/udhcpd.conf");
        //SetDHCP(tmpDHCPFile, tmpValue);
    }
    else if(!pName.compare("RouterIP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_RouterIpAddr = QHostAddress(tmpValue);
        tmpHBoxLayout = CreateTextEdit(pName, tmpValue);
    }
    else if(!pName.compare("RouterPort", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_RouterPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_RouterPort.mNum, m_RouterPort.mMinNum, m_RouterPort.mMaxNum);
    }
    else if(!pName.compare("RouterTimeout"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_RouterTimeout.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_RouterTimeout.mNum, m_RouterTimeout.mMinNum, m_RouterTimeout.mMaxNum);
        if(m_RouterTimeout.mNum)
        {
            m_RouterWait.SetTimeout(m_RouterTimeout.mNum);
            m_RouterWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
        }
    }
    return tmpHBoxLayout;
}

void PhysicalLANC::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    PhysicalLANServer::UpdateSubPara(pName, pObject, pElement);
    if(!pName.compare("WIFION", Qt::CaseInsensitive))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, m_WifiBootON, tmpComponent->isChecked()))
            {
            }
            else
            {
                mShowMessage = !m_WifiBootON;
            }
        }
    }
    else if(!pName.compare("WIFIIP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = m_WifiIpAddr.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                m_WifiIpAddr = QHostAddress(tmpValue);

                QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Middleware/WIFI/udhcpd.conf");
                SetDHCP(tmpDHCPFile, tmpValue);
            }
        }
    }
    else if(!pName.compare("RouterIP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = m_RouterIpAddr.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                m_RouterIpAddr = QHostAddress(tmpValue);
            }
        }
    }
    else if(!pName.compare("RouterPort", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_RouterPort.mNum, tmpComponent->value(), m_RouterPort.mMinNum, m_RouterPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("RouterTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_RouterTimeout.mNum, tmpComponent->value(), m_RouterTimeout.mMinNum, m_RouterTimeout.mMaxNum))
            {
            }
            else
            {
                if(m_RouterTimeout.mNum)
                {
                    m_RouterWait.SetTimeout(m_RouterTimeout.mNum);
                    m_RouterWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
            }
        }
    }
}

quint32 PhysicalLANC::Monitor(quint32 pCount)
{
    PhysicalLANServer::Monitor(pCount);

    if(m_RouterClient)
        m_RouterClient->Monitor(pCount);

    if(m_RouterTimeout.mNum && !m_RouterIpAddr.isNull() && m_RouterPort.mNum && m_RouterWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
    {
        emit sConnect();
    }
}

void PhysicalLANC::slot_InitNetwork()
{
    PhysicalLANServer::slot_InitNetwork();

#if (defined(IDE) || defined(DISPLAY))
    emit sInfo(QString("WIFI : %1").arg(m_DevManager->mDevCode), 1000, DM_NET);
#endif

    //>@��һ�ν����������������
    QTimer::singleShot(1000, this, SLOT(slot_Connect())); //>@������ʱ�������ӣ���������ڽ���û����ʾʱ�������ӣ���ʱ��û��WIFIͼ����ʾ��
}

void PhysicalLANC::slot_Connect()
{
    if(!m_RouterClient)
    {
        m_RouterClient = new PhysicalLANClient(new ToolTcpClient, m_DevManager);
        connect(m_RouterClient, SIGNAL(sConnState(bool)), this, SLOT(slot_RouterConnState(bool)));
        m_RouterClient->mIP = m_RouterIpAddr;
        m_RouterClient->mPort = m_RouterPort.mNum;
        m_RouterClient->SeteAutoConnect(true);
        m_RouterClient->slot_Initialize();
    }
    if(!m_RouterClient->mTcpClient)
        m_RouterClient->SetTcpClient(new ToolTcpClient);
    if(!m_RouterClient->isConnected())
    {
        slot_RouterConnState(false);
        m_RouterClient->Sync();
    }
    if(m_RouterTimeout.mNum)
        m_RouterWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
}

void PhysicalLANC::slot_RouterConnState(bool pFlag)
{
    if(pFlag)
    {
#ifdef DISPLAY
        emit sShowIcon("router");
#endif
    }
    else
    {
#ifdef DISPLAY
        emit sHideIcon("router");
#endif
    }
}

void PhysicalLANC::Connected()
{
    //PhysicalLANServer::Connected();  //>@�Ѿ��ڴ�������ʱǿ������Ϊ�������ˡ�
#ifndef SERVER
#ifdef DISPLAY   //>@�������Ӻ���ʾͼ��
    if(m_LCD)
        m_LCD->slot_SrnSaveMode(false);
    emit sShowIcon("net");
#ifndef SUPPORT_WIDGET
    emit sSetDevices(false, DM_NET);
#endif
#endif
#endif
}

void PhysicalLANC::Disconnected()
{
#ifndef SERVER
#ifdef DISPLAY  //>@�Ͽ����Ӻ󣬲�����������ʾͼ��
    //>@�鿴��ǰ�Ƿ�����Ч����
    if(!isConnected())
    {
        emit sHideIcon("net");
#ifndef SUPPORT_WIDGET
        emit sSetDevices(true, DM_NET);
#endif
        //>@�����CANOPEN�豸������Ҫ��ѯ��ǰ��ʲôģʽ
        PhysicalCOM *tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
        if(tmpCOM)
        {
            tmpCOM->SetCanopenMode(tmpCOM->mCanopenMode);
        }
    }
#endif
#endif
}

void PhysicalLANC::slot_ConnState(bool pFlag)
{
    PhysicalLANServer::slot_ConnState(pFlag);
    if(!pFlag && mShowMessage)
    {
        emit sInfo(QString("WIFI : %1").arg(m_DevManager->mDevCode), MSG_PERMANENT, DM_NET);
    }
}

bool PhysicalLANC::SetValue(QString pValue)
{
#ifndef PART_TEST
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpCmdList = tmpList.at(i).split(':');
        if(tmpCmdList.count()!=2)
            continue;
        QString tmpName = tmpCmdList.at(0);
        QString tmpContent = tmpCmdList.at(1);
        if(!tmpName.compare("wifi", Qt::CaseInsensitive))  //>@��������
        {
            if(!tmpContent.compare("open", Qt::CaseInsensitive))
            {
#ifdef DISPLAY
                //>@��ѯ��ǰ�Ƿ���COM�豸������У�����COM�豸����WIFI
                PhysicalCOM *tmpCOM = 0;
            #ifdef IDE
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #elif DISPLAY
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #endif
                if(tmpCOM)
                    tmpCOM->OpenWIFI();
                else
                    OpenWIFI();
#endif
            }
            else if(!tmpContent.compare("close", Qt::CaseInsensitive))
            {
#ifdef DISPLAY
#ifndef SODIMAS
                //>@��ѯ��ǰ�Ƿ���COM�豸������У�����COM�豸����WIFI
                PhysicalCOM *tmpCOM = 0;
            #ifdef IDE
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #elif DISPLAY
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #endif
                if(tmpCOM)
                    tmpCOM->CloseWIFI();
                else
                    CloseWIFI();
#endif
#endif
            }
        }
    }
#endif
    return true;
}

bool PhysicalLANC::PrepareRelease()
{
    CloseWIFI(true);

    QString tmpCMD = QString("ifconfig ra0 down | killall udhcpd &");
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
}

void PhysicalLANC::OpenWIFI(bool pForce)
{
#ifdef SUPPORT_MUTEX
    if(!m_WifiMutex->tryLock())
#else
    if(m_WifiMutex)
#endif
    {
        IDE_DEBUG(QString("Can't Open WIFI for mutex locked!"));
        return;
    }
#ifndef SUPPORT_MUTEX
    m_WifiMutex = true;
#endif

    if(!pForce && m_WifiOpened)
    {
#ifdef SUPPORT_MUTEX
        m_WifiMutex->unlock();
#else
        m_WifiMutex = false;
#endif
        return;
    }

    //>@����Ѿ�wifi�򿪣�������ʾCODE
    //>@��Msg����ʾһά��
#if (defined(IDE) || defined(DISPLAY))
    if(!m_WifiBootON)
        emit sInfo(QString("WIFI : %1").arg(m_DevManager->mDevCode), MSG_PERMANENT, DM_NET);
#endif

    m_WifiOpened = true;
#if 0
    QString tmpCMD = QString("ifconfig ra0 %1 up | route add default dev ra0 | udhcpd -f /usr/bst/SYSTEM/Middleware/WIFI/udhcpd.conf &").arg(m_IpAddr.toString());
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
#else
    m_WifiOperate.Open(m_WifiIpAddr, true);
#endif

#ifdef SUPPORT_MUTEX
    m_WifiMutex->unlock();
#else
    m_WifiMutex = false;
#endif
}

void PhysicalLANC::CloseWIFI(bool pForce)
{
    if(!pForce && m_WifiBootON)
    {
        return;
    }
#ifdef SUPPORT_MUTEX
    if(!m_WifiMutex->tryLock())
#else
    if(m_WifiMutex)
#endif
    {
        IDE_DEBUG(QString("Can't Close WIFI for mutex locked!"));
        return;
    }
#ifndef SUPPORT_MUTEX
    m_WifiMutex = true;
#endif

    if(!m_WifiOpened)
    {
#ifdef SUPPORT_MUTEX
        m_WifiMutex->unlock();
#else
        m_WifiMutex = false;
#endif
        return;
    }

#ifndef SERVER
#ifdef DISPLAY  //>@ȷ���Ͽ����Ӻ�ʹͼ����ʧ��
    emit sHideIcon("net");
    m_Connected = false;
#endif
#endif

    //>@��Msg����ʾһά��
    emit sInfo(QString(), MSG_TEMPORARY, DM_NET);
    emit sInfo(QString("WIFI closed!"), 1000);

#if 0
    QString tmpCMD = QString("ifconfig ra0 down | killall udhcpd &");
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
#else
    m_WifiOperate.Close(true);
#endif
    m_WifiOpened = false;

#ifdef SUPPORT_MUTEX
    m_WifiMutex->unlock();
#else
    m_WifiMutex = false;
#endif
}

qint64 PhysicalLANC::write(const QByteArray &data)
{
    if(mUdpDevice)
    {
        return mUdpDevice->write(data);
    }
    return -1;
}

qint64 PhysicalLANC::reply(QByteArray data)
{
    if(mUdpDevice)
    {
        return mUdpDevice->reply(data);
    }
    return -1;
}

QByteArray PhysicalLANC::readAll()
{
    if(mUdpDevice)
    {
        return mUdpDevice->readAll();
    }
    return QByteArray();
}

PhysicalLANR::PhysicalLANR(QObject *parent):
    PhysicalLANServer(parent)
{
    mDevType = P_LANR;
    mMonitorWait.SetTimeout(0);  //>@Ĭ�ϲ��Զ���������

    m_ServerClient = 0;
    m_ServerTimeout.setnum(0, 0, 60000);
    m_ServerPort.setnum(D_NMPORT, 60000, 65000);
}

PhysicalLANR::~PhysicalLANR()
{
}

QHBoxLayout* PhysicalLANR::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = PhysicalLANServer::InitSubPara(pName, pElement);
    if(tmpHBoxLayout)
        return tmpHBoxLayout;
    if(!pName.compare("ServerIP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_ServerIpAddr = QHostAddress(tmpValue);
        tmpHBoxLayout = CreateTextEdit(pName, tmpValue);
    }
    else if(!pName.compare("ServerPort", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_ServerPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_ServerPort.mNum, m_ServerPort.mMinNum, m_ServerPort.mMaxNum);
    }
    else if(!pName.compare("ServerTimeout"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_ServerTimeout.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_ServerTimeout.mNum, m_ServerTimeout.mMinNum, m_ServerTimeout.mMaxNum);
        if(m_ServerTimeout.mNum)
        {
            m_ServerWait.SetTimeout(m_ServerTimeout.mNum);
            m_ServerWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
        }
    }
    return tmpHBoxLayout;
}

void PhysicalLANR::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    PhysicalLANServer::UpdateSubPara(pName, pObject, pElement);
    if(!pName.compare("ServerIP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = m_ServerIpAddr.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                m_ServerIpAddr = QHostAddress(tmpValue);
            }
        }
    }
    else if(!pName.compare("ServerPort", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_ServerPort.mNum, tmpComponent->value(), m_ServerPort.mMinNum, m_ServerPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("ServerTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_ServerTimeout.mNum, tmpComponent->value(), m_ServerTimeout.mMinNum, m_ServerTimeout.mMaxNum))
            {
            }
            else
            {
                if(m_ServerTimeout.mNum)
                {
                    m_ServerWait.SetTimeout(m_ServerTimeout.mNum);
                    m_ServerWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
            }
        }
    }
}

void PhysicalLANR::slot_InitNetwork()
{
    PhysicalLANServer::slot_InitNetwork();

    //>@��һ�ν����������������
    QTimer::singleShot(1000, this, SLOT(slot_Connect())); //>@������ʱ�������ӣ���������ڽ���û����ʾʱ�������ӣ���ʱ��û��WIFIͼ����ʾ��
}

void PhysicalLANR::slot_Connect()
{
    if(!m_ServerClient)
    {
        m_ServerClient = new PhysicalLANClient(new ToolTcpClient, m_DevManager);
        connect(m_ServerClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ServerConnState(bool)));
        m_ServerClient->mIP = m_ServerIpAddr;
        m_ServerClient->mPort = m_ServerPort.mNum;
        m_ServerClient->SeteAutoConnect(true);
        m_ServerClient->slot_Initialize();
    }
    if(!m_ServerClient->mTcpClient)
        m_ServerClient->SetTcpClient(new ToolTcpClient);
    if(!m_ServerClient->isConnected())
    {
        slot_ServerConnState(false);
        m_ServerClient->Sync();
    }
    if(m_ServerTimeout.mNum)
        m_ServerWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
}

void PhysicalLANR::slot_ServerConnState(bool pFlag)
{
    if(pFlag)
    {
#ifdef DISPLAY
        emit sShowIcon("server");
#endif
    }
    else
    {
#ifdef DISPLAY
        emit sHideIcon("server");
#endif
    }
}

quint32 PhysicalLANR::Monitor(quint32 pCount)
{
    PhysicalLANServer::Monitor(pCount);

    if(m_ServerClient)
        m_ServerClient->Monitor(pCount);

    if(m_ServerTimeout.mNum && !m_ServerIpAddr.isNull() && m_ServerPort.mNum && m_ServerWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
    {
        emit sConnect();
    }
}

QStringList PhysicalLANR::Scan(QString pContent, quint32 pTimeout)
{
    PH_Type tmpType = m_CurDevType;
    QIODevice* tmpDev = m_CurTransDev.data();
    SetCurDev(P_UDP, mUdpDevice);
    QStringList tmpList = PhysicalProtocol::Scan(pContent, pTimeout);
    SetCurDev(tmpType, tmpDev);
    return tmpList;
}

//>@���������ӵ��豸�ڵ�
QList<PhysicalLANClient*> PhysicalLANR::ScanDevices(quint32 pTimeout)
{
    QList<PhysicalLANClient*> tmpDevList;
    QStringList tmpList = Scan(QString(""), pTimeout);
    QString tmpDevStr;
    foreach(tmpDevStr, tmpList)
    {
        QHostAddress tmpAddr;
        QString tmpCode;
        quint16 tmpPort;
        if(!ParseIP(tmpDevStr, tmpAddr, tmpPort, tmpCode))
            continue;
        if(tmpCode.isEmpty())
            continue;
        //>@����һ���ڵ�
        bool tmpNewConn = false;
        PhysicalLANClient* tmpTcpClient = AddClient(tmpAddr, D_NMPORT, tmpNewConn);
        if(!tmpTcpClient)
            continue;
        tmpTcpClient->mCode = tmpCode;
        //>@����˽ڵ�֮ǰΪδ����״̬������Ĵ˽ڵ�״̬Ϊ����״̬
        if(tmpTcpClient->mDevState == S_OFF)
            tmpTcpClient->mDevState == S_ON;
        //>@
        if(tmpNewConn)
            tmpDevList.append(tmpTcpClient);
    }
    return tmpDevList;
}

void PhysicalLANR::slot_Init(QVariant pPara)
{
    PhysicalLANClient *tmpClient = VAR_CAST<PhysicalLANClient*>(sender());
    if(tmpClient)
    {
        QStringList tmpParaList = pPara.toStringList();
        if(tmpParaList.count() >= 3)
        {
            tmpClient->mCode = tmpParaList.at(0);
            tmpClient->mPasswd = tmpParaList.at(1);
            tmpClient->mLocation = tmpParaList.at(2);
        }
    }
}


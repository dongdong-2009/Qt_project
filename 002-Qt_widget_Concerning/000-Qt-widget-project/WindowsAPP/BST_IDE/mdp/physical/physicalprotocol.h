#ifndef PHYSICALPROTOCOL_H
#define PHYSICALPROTOCOL_H

#include "physicalbase.h"
#ifdef SURRPORT_ZIP
#include "zipfun.h"
#endif
#include <QHostInfo>
#include <QMutex>
#include "tooludp.h"
#include "tooltcp.h"
#include "serialport.h"

#define D_FRAMEMINLEN       5

#define D_SETFRAME_TIMEOUT  2000

#define D_PACKAGE_SIZE      10240
#define D_PACKAGE_TIMEOUT   500   //>@����1�����ݵ�ʱ��(ms)

#define D_CONTENTSPLIT      QString(';')

inline qint64 GetTransTimeout(qint64 pSize) { return qMax((qint64)(pSize * D_PACKAGE_TIMEOUT / D_PACKAGE_SIZE), (qint64)(5000)); }

class PhysicalProtocol;
class PhysicalPlayer;

#ifdef SUPPORT_SCENE
class GraphicsMedia;
#elif SUPPORT_WIDGET
class WidgetMedia;
#endif

class FRAME_STRUCT : public QObject
{
    Q_OBJECT
public:
    FRAME_STRUCT()
    {
        mDevType = P_INVALID;
        mFlag = Flag_Normal;
        mType = Type_NONE;
        mCmd = 0;
        mAddr = 0;
        mLength = 0;
        mCRC = 0;
    }
    void ClearData()
    {
        mData.clear();
        mLength = 0;
    }
    void SetDev(PH_Type pType, QIODevice* pDev)
    {
        mDevType = pType;
        mDev = pDev;
    }
    bool IsFromHost() {return (mFlag==Flag_Normal);}
public:
    FRAME_FLAG                  mFlag;  //>@���ӻ�
    FRAME_TYPE                  mType;
    quint8                      mCmd;
    quint16                     mAddr; //>@֡������Ҳ�������Ϊ���ļ�/�Ĵ���)��ַ
    quint16                     mLength;  //>@һ�����ڽ������ݼ���
    QByteArray                  mData;  //>@����С��ͨ��mData�������
    quint8                      mCRC;
    quint32                     mError; //>@ִ֡�еĴ�������

    PH_Type                     mDevType;
    QPointer<QIODevice>         mDev;   //>@���ʹ�֡���豸
} ;

typedef enum {RU_NONE, RU_FLRFUNC} RU_TYPE;     //>@��ʾ������ܸ���

inline RU_TYPE GetReuseType(QString pType)
{
    if(pType.compare("Flr_Func", Qt::CaseInsensitive) == 0)
        return RU_FLRFUNC;
    return RU_NONE;
}

inline QString GetReuseTypeString(RU_TYPE pType)
{
    if(RU_FLRFUNC == pType)
        return QString("Flr_Func");
    return QString("None");
}

typedef enum {ID_NONE, ID_FLRLOGO, ID_AUTORUN} IDLE_RUN;     //>@��״̬���Զ����Ŷ���

inline IDLE_RUN GetIdleRunType(QString pType)
{
    if(pType.compare("Flr_Logo", Qt::CaseInsensitive) == 0)
        return ID_FLRLOGO;
    else if(pType.compare("AutoRun", Qt::CaseInsensitive) == 0)
        return ID_AUTORUN;
    return ID_NONE;
}

inline QString GetIdleRunTypeString(IDLE_RUN pType)
{
    if(ID_FLRLOGO == pType)
        return QString("Flr_Logo");
    else if(ID_AUTORUN == pType)
        return QString("AutoRun");
    return QString("None");
}

typedef struct{
    IDLE_RUN            mIdleRunType;
    int                 mIdleTimeout, mMinIdleTimeout, mMaxIdleTimeout;
    int                 mIdleFreq, mMinIdleFreq, mMaxIdleFreq;
} IDLE_INFO;

class ReadFileOp :public QObject
{
    Q_OBJECT
public:
    ReadFileOp(QObject *parent = 0);
    ~ReadFileOp();

    void unbind();
    bool bind(QString pSrc);
    void setpos(qint64 pPos);
    bool read(qint64 maxlen);
    int progress();

    QByteArray getmd5() {return QByteArray();}

public:
    bool        mEnable;
    QFile       mFile;
    qint64      mSize;
    qint64      mPos;
    qint64      mSurplus; //>@ʣ���ֽ���
    QByteArray  mCurPack;  //>@��ǰ�����İ�����
};

class WriteFileOp :public QObject
{
    Q_OBJECT
public:
    WriteFileOp(QObject *parent = 0);
    ~WriteFileOp();

    void unbind();
    bool bind(QString pSrc, qint64 pSize);
    bool write(QByteArray pByteArray, qint64 pPos=-1); //>@pPos���Ϊ��������ʾֱ��׷��
    bool repair();  //>@��������ļ��ָ�
    int progress();

    QByteArray getmd5() {return QByteArray();}

public:
    bool        mEnable;
    QFile       mFile;
    qint64      mSize;
    qint64      mPos;
    qint64      mSurplus; //>@ʣ���ֽ���
};

typedef enum{
    Flag_GetSnapShot = 0x01,
    Flag_GetCamera = 0x02,
    Flag_SendSnapShot = 0x10,
    Flag_SendCamera = 0x20
}STREAM_TYPE;

enum {
    REG_ADDR_PH = 0,
    REG_ADDR_LG = 10000,
    REG_ADDR_COM = 20000
};

class ProtocolState{
public:
    ProtocolState()
    {
        for(int i=0;i<3;i++)
        {
            mType[i] = 0;
            mState[i] = Unstart;
        }
    }

    int         mType[3];  //>@Ŀǰ֧��3�㣬0���ʾ���һ�㣬1����ʾ����ָ�2��ʾָ���Լ��ļ�֡�����
                           //>@mType���Ϊ0��ʾ������
    TaskState   mState[3];
};

/*++++++++========================================================================
PhysicalProtocol  :
    1��֧��˫���䣬����ʱ���ݽ��յ���֡���ͷ������ݡ�
                  ����ʱ�����β�ȷ���������ͣ�0x80Ϊ����0xc0Ϊ�ӣ�
    2��Э���շ�������
          ����һ��������У��Ǹ�FIFO��
            ����ǰ���͵�֡���У��������Ҫ��Ӧ������Ҫ���У������յ���Ӧʱ�鿴��֡�����е��Ƿ�һ��
            �������յ��ӻ�����Ӧʱ������Ҫ�����κ���Ӧ
          �ӣ�����Ҫ���壬������Ӧλѡ���Ƿ���Ӧ����
    3���ļ�����
          ͨ��4��ָ���޸��ļ�����״̬��
          ͨ��filetest�޸Ĵ������ļ�ָ�룬�����ļ�����ָ��״̬ȷ����ǰ�ļ������״̬��
            �����ļ�ָ��Ϊ�գ�����յ��ļ���д����ȫ��ʧ�ܡ�
            �����һ�δ���ʧ�ܺ�ϣ���˴ο��Զϵ���������ô������ʹ��filetest�޸Ĵ������ļ�ָ�룬Ȼ����ͨ���޸�֡�������ﵽ��Ŀ�ġ�
    4��
//++++++++========================================================================*/

class PhysicalProtocol : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalProtocol(QObject *parent);
    ~PhysicalProtocol();

    virtual quint32 InitPara();
    virtual QHBoxLayout* InitSubPara(QString pName, QDomElement pElement){}
    virtual void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement){}

    void UpdateBtnRange(QString pValue);

    virtual quint32 Monitor(quint32 pCount);

    virtual void Connected();
    virtual void Disconnected();
    virtual bool isConnected();
    virtual bool CanAutoConnect() {return false;}

    bool CanAbort();
    void WaitForLeisure(quint32 pTimeout);

    //>@�����QtWait�����˶��߼��Ĺ���
    quint32 ProtocolWait(int &pFlag, int pEqlFlag, unsigned int msec);

    void SetCurDev(PH_Type pType, QIODevice* pDev);

    void SeteAutoConnect(bool pFlag) {mAutoConnFlag = pFlag;}
    void SetHostFlag(bool pFlag)    {mHostFlag = pFlag;}

    quint32 SendData(FRAME_STRUCT *pFrame);

    quint32 SendFrame(FRAME_STRUCT *pFrame);
    quint32 ReplyFrame(FRAME_STRUCT *pFrame);

    virtual quint32 ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pFrameBuffer);

    quint32 ParseFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseHeartbeatFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseUrgencyFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseFloorFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseButtonFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseDisBtnFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseScandevFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseConfFrame(FRAME_STRUCT *pFrame);//>@��⵽һ���µ�����ָ��
    virtual quint32 ParseFileFrame(FRAME_STRUCT *pFrame, bool pEmitPrg=true);
    virtual quint32 ParseStreamFrame(FRAME_STRUCT *pFrame);

    //>@File
    void SetMaxBlock(qint64 pSize) { m_MaxBlockSize = pSize; }

    quint32 SendFileData(FRAME_STRUCT* pPreFrame, bool pEmitPrg=false);
    quint32 SendFileMD5(FRAME_STRUCT* pPreFrame);
    quint32 SendFileStop(FRAME_STRUCT* pPreFrame);
    quint32 ReplyFileStop(FRAME_STRUCT* pPreFrame);

    quint32 ReplyReTrans(FRAME_STRUCT* pFrame, quint16 pAddr);
    quint32 ReplyOneTrans(FRAME_STRUCT* pFrame, quint16 pAddr);

    //>@������
    quint32 SendStream(STREAM_TYPE pType);
    quint32 GetStream(STREAM_TYPE pType);

    //>@����
    quint32 SendHBFrame(quint32 pTimeout, bool pAck = true);  //>@��������
    quint32 ReplyHBFrame(FRAME_STRUCT *pFrame, QByteArray pContent);

    bool HB(quint32 pTimeout, bool pWait=false);  //>@ʹ���������ɣ�����ʾ����һ�����͡����pTimeoutΪ0����ʾ�ر�����

    bool WaitHB(quint32 pTimeout);

    //>@Conf����
    quint32 SendConfFrame(QString pInstruc);  //>@��������ָ��
    bool SendConfFrame(QString pInstruc, int pTimeout);
    quint32 ReplyConfFrame(SetCmdInfo pInfo, FRAME_STRUCT* pFrame);  //>@��Ҫ���CMDָ��֡����

    quint32 SendConfFrame(SetCmdInfo pInfo);

    void SendFileTest(QString pCover, QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void GetFileTest(QString pCover, QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void StartGetFile(quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void ListPath(QString pPath, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);

    //>@ָ��壺Զ�̽�ѹ��ѹ��
    void Zip(QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void Unzip(QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);

    //>@�ǻ����ѹ��ѹ����isLocal��ʾ�Ƿ�Ϊ���ؽ�ѹ��ѹ��
    bool Zip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal);
    bool UnZip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal);

    bool Set(QString pContent, qint64 pTimeout=-1);
    bool Get(QString pContent, qint64 pTimeout=-1);
    virtual void Bootup();
    void Msg(QString pMessage)    { SendConfFrame(QString("Msg#%1").arg(pMessage)); }
    void Cmd(QString pMessage)    { SendConfFrame(QString("Cmd#%1").arg(pMessage)); }

    //>@���öԷ��ļĴ���
    bool WriteReg(quint16 pRegAddr, QByteArray pContent, bool pNeedAck=false);
    QByteArray ReadReg(quint16 pRegAddr, quint32 pTimeout);

    //>@�������ؼĴ���
    /* 0~10000��ַ��Ϊ�����豸������ַ�����㷽����PH_Type*100+n��Ҳ�����֧��100�������豸
     * 10000~20000��ַ��Ϊ�߼��豸������ַ�����㷽����10000+LG_Type*100+n
     * 20000~30000��ַ��Ϊ�������������ַ�����㷽����20000+COM_Type*100+n
    */
    virtual QByteArray SetRegister(quint16 pAddr, QByteArray pContent);
    virtual QByteArray GetRegister(quint16 pAddr);

    quint32 ParseRegFrame(FRAME_STRUCT *pFrame);

    //>@����ָ�����
    bool StopTask(); //>@ֹͣ��ǰ���񣬰����ļ������Լ���������

    void ClearConfQueue();
    void PushConfQueue(SetCmdInfo pInfo);
    quint32 ProcConfQueue(SetCmdInfo &pInfo, bool pSuccess);

    bool StartConfMonitor(qint64 pTime);
    void StopConfMonitor();

    bool GetDevInfo(QString pName, QString pPasswd, quint64 pLife);

    //>@����ָ��
    bool SendFile(QString pCover, QString pSrc, QString pDst);
    bool GetFile(QString pCover, QString pSrc, QString pDst);
    bool SendDir(QString pSrc, QString pDir);
    bool GetDir(QString pSrc, QString pDir);

    bool GetDesktop(QString pSrc, QString pDir, qint64 pTime=30000) {}

    //>@
    quint32 SendScanFrame(QString pContent);
    virtual QStringList Scan(QString pContent, quint32 pTimeout);

    void SendFloorFrame(quint32 pFloorNum, quint8 pFunc, quint32 pResvFunc);
    void SendFloorFrame(quint8 pExFunc, quint32 pFloorNum, ARROWSTATE pArrowState, quint8 pRadioFun, quint8 pAudio);
    void ReplyFloorFrame(quint8 pTouch, quint8 pSwitch, quint8 pFun0);
    void SendButtonFrame(QVariant pBtnList, quint32 pDisablity);
    void ReplyButtonFrame(QVariant pBtnList, quint32 pDisablity);

public slots:
    void UpdatePara(QObject* pObject);
    void slot_Initialize();
    void slot_StandbyCallback(bool pFlag);

    void slot_ConfMonitorTimeout();

    virtual void slot_Init(QVariant) {}

    //>@�����APP��ָ���������������Display����ʾһ��ʱ����û���յ���Ч����
    virtual void slot_HBTimeout(quint32 pID);
    virtual void slot_HB(quint32 pTimeout);  //>@��������

    virtual void slot_Connect() {}      //>@���Խ�������
    virtual void slot_Disconnect() {}   //>@���ԶϿ�����

    void slot_BtnCallEvent(QVariant pPara);

    void slot_Zip(FRAME_STRUCT* pFrame);
    void slot_Unzip(FRAME_STRUCT* pFrame);

    void slot_ZipTaskEnd();
    void slot_ZipTaskErr();
    void slot_ZipTaskProgress(int ratio);

signals:
    void sInit(QVariant);

    void sConnect();
    void sDisconnect();

    void sHBTimeout(quint32 pID);
    void sBootup();

    void sTouchOutputChange(quint8 pValue);
    void sSwitchOutputChange(quint8 pValue);

    void sFloorEvent(quint32 pFloorNum);
    void sArrowEvent(quint32 pArrowState);
    void sRadioFunEvent(quint32 pFundex);
    void sMediaEvent(quint32 pMediaNum);
    void sAudioEvent(quint32 pAudioNum);
    void sButtonEvent(QVariant pBtnList, BUTTONSTATE pState);
    void sDisablityEvent(bool pDisablity);

    void sZip(FRAME_STRUCT* pFrame);
    void sUnzip(FRAME_STRUCT* pFrame);

    void sZip(QString pSrc, QString pDir, bool pCover, bool pIngore);
    void sUnzip(QString pSrc, QString pDir, bool pCover, bool pIngore);

    void sSet(QString pContent);
    void sGet(QString pContent);

    void sMessage(QString pMessage, int pDelay=5000);

    void sSendFile(bool pState);
    void sGetFile(bool pState);

    void sSendDir(bool pState);
    void sGetDir(bool pState);

    void sDownLoadThemeZip(bool pState);
    void sUpLoadThemeZip(bool pState);

    void sHB(quint32 pTimeout);

public:
#ifdef SUPPORT_MUTEX
    QMutex                 *m_ParseDataMutex;
#else
    bool                    m_ParseDataMutex;
#endif

    QPointer<QIODevice>     m_CurTransDev;    //>@��ǰ���յ��豸
    PH_Type                 m_CurDevType;

    quint32                 m_MonitorCount;

    TaskState               m_HBState;
    quint32                 m_HBOverflow[3];   //>@���������ĳ�ʱ����

    //>@��0����ʾ�ͻ�����APP����������Ƶ�ʣ�
    //>@��1����ʾ�ͻ������APP��������������Ƶ��
    //>@��2����ʾHost���������ʾ����Ӧ��������
    NUM_INFO                m_HBFreq[3];
    WaitInfo                m_HBWait[3];
    quint32                 m_HBCount[3];      //>@���͵��������������������m_HBOverflowʱ��ʽ����

    bool                    m_Elevator;
    NUM_INFO                m_BtnBaseNum;

    QByteArray              m_DataBuffer;
    FRAME_STATE             m_FrameState;

    //>@ָ���״̬����
    FRAME_STRUCT           *m_CurRevFrame;      //>@��ǰ���յ�֡
    FRAME_STRUCT           *m_CurSendFrame;     //>@��ǰ���͵�֡

    ProtocolState           m_CurTaskStatus;

    bool                    m_Connected;

    qint64                  m_WaitTimeout;  //>@���ڳ�ʱ�ȴ���ȫ�ֱ���

    SetCmdInfo              m_RcvScanInfo;

    bool                    mHostFlag;    //>@�Ƿ�Ϊ�������ӷ�������Socket
    bool                    mAutoConnFlag, mAutoConnState;//>@�Զ�����
    bool                    mAutoConnLog;  //>@ָʾ�Ƿ������Զ����������������ܽ��յ�����ʱ��λ��

    //>@���ݽӿڲ���
    bool                    m_DebugMode;        //>@�Ƿ�������ģʽ
    RU_TYPE                 m_ReuseType;
    IDLE_INFO               m_IdleInfo;

    QString                 m_BtnRange;
    QList<int>              m_ValidBtns;

    NUM_INFO                m_FlrStationNum;

    //>@�ļ�����
    qint64                  m_MaxBlockSize;
    ReadFileOp             *m_ReadFileOp;
    WriteFileOp            *m_WriteFileOp;
    TaskState               m_FileTransState;

    TaskState               m_SetTaskState;
    TaskState               m_GetTaskState;

    TaskState               m_ReadRegState;
    QByteArray              m_ReadRegBuffer;

    bool                    m_RetransFile; //>@�Ƿ��ظ���ǰ���ļ����������

    //>@����ָ��
    SetCmdInfo              m_SendConfInfo, m_RcvConfInfo;
    QQueue<SetCmdInfo>      m_ConfQueue;  //>@������������ָ���
    TaskState               m_ConfState;
    QTimer                  m_ConfMonitorTimer;   //>@���ĳ��setcmdinfo�Ƿ�ʱ

    TaskState               m_SendDirTaskState;
    TaskState               m_GetDirTaskState;

    TaskState               m_ZipTaskState;
    //>@��������ָ��
    quint32                 m_QueueProgress;

    //>@��ʽ����
    QPixmap                 m_SnapShotPix;
    TaskState               m_SnapShotState;

    //>@ScanDevice
    QStringList             m_DevAddrList;      //>@�豸ɨ���б� IP��PORT��CODE ���� IP:CODE
    bool                    m_LoopFlag;         //>@����ɨ���豸ʱ�ȴ���ʶ�������ڽ������ɨ��Ĺ������ͷŴ˶����bug

    QHash<QString, QString> m_DevInfoList;
    TaskState               m_DevInfoTaskState;

    QStringList             m_LsFileList;
    TaskState               m_LsTaskState;

    bool                    m_StopTaskFlag;

    //>@������������


    QPointer<PhysicalPlayer>     mPlayer;
#ifdef SUPPORT_SCENE
    QPointer<GraphicsMedia>      mMedia;
#elif SUPPORT_WIDGET
    QPointer<WidgetMedia>        mMedia;
#endif
};


typedef enum{
    TYPE_LOCAL = 0,     //>@���ػ��壬Ҳ��������������
    TYPE_NET,           //>@�����豸
    TYPE_STORAGE,       //>@�洢�豸
    TYPE_COM            //>@�����豸
}DEVICETYPE;

inline DEVICETYPE GetDeviceType(QString pDevType)
{
    if(pDevType.compare("NET", Qt::CaseInsensitive) == 0)
        return TYPE_NET;
    else if(pDevType.compare("STORAGE", Qt::CaseInsensitive) == 0)
        return TYPE_STORAGE;
    else if(pDevType.compare("COM", Qt::CaseInsensitive) == 0)
        return TYPE_COM;
    return TYPE_LOCAL;
}

typedef enum{
    TYPE_TREE = 1,
    TYPE_LIST,
    TYPE_TABLE,
    TYPE_NONE
}DEVCANTYPE;

class DevContainer
{
public:
    DevContainer()
    {
        mItem = 0;
        mType = TYPE_NONE;
    }
public:
    void*            mItem;       //>@�豸����ָ��
    DEVCANTYPE       mType;       //>@�豸��������
};

typedef struct{
    DEVICETYPE      mType;
    QString         mPath;
} RC_CAN;

#define D_DELFLAG       10001

class PhysicalHost : public PhysicalProtocol
{
    Q_OBJECT
public:
    PhysicalHost(QString pAddr=QString(), QString pBufferPath=QString(), QObject *parent=0);
    ~PhysicalHost();

    virtual bool isValid();

    void SetDevCan(QTreeWidgetItem *pDevItem, DEVICESTATE pState=S_OFF);
    void SetDevCan(QListWidgetItem *pDevItem, DEVICESTATE pState=S_OFF);
    void SetDevCan(QTableWidgetItem *pDevItem, DEVICESTATE pState=S_OFF);

    virtual bool Scan();        //>@ɨ�軺��·���µĹ���
    virtual void Clear() {}
    QList<RC_CAN> ScanDirectiory(QString pPath);

    virtual bool isSync(quint32 pTimeout = 0);
    virtual bool Sync() {return false;}
    bool Sync(quint32 pTimeout);
    virtual void Async() { Disconnected(); }
    bool Async(quint32 pTimeout);

    virtual void Connected();
    virtual void Disconnected();

    virtual void ChangeDevAddr(QString pDevAddr);  //>@�������޸��豸���ͣ��������޸��豸��ַ�������޸�IP
    virtual void ChangeBuffer(QString pBufferPath);

    virtual void OpenLocation(QString pLocation) {}  //>@��ĳ��·��

    virtual bool DeleteTheme(QString pThemeName) {return true;}
    virtual bool SetRunningTheme(QString pThemeName) {return true;}
    virtual QString GetRunningTheme() {return QString();}

signals:
    void sConnState(bool pState);

public:
    DevContainer     mDevCan;           //>@�豸����
    DEVICETYPE       mDevType;          //>@�豸����

    QString          mDevNickname;
    QString          mDevAddr;
    QString          mBufferPath;

    bool             mIsBusy;
};


#endif // PHYSICALPROTOCOL_H

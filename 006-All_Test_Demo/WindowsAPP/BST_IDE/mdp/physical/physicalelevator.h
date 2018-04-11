#ifndef PHYSICALELEVATOR_H
#define PHYSICALELEVATOR_H

#include "physicalprotocol.h"
#include "serialport.h"

class PhysicalLANC;
class LogicThemeManager;

/*
js:
    function abc()
    {
    }
    function abc_result()
    {
    }
ini:
    [TestName]
    xx = x.x.x.x.x   x.x.x.x.x
*/
typedef struct{
    QString mFuncScript;
    QString mProcScript;
} JS_TEST_CASE;

typedef struct{
    QList<QByteArray> mFrameList;
} INI_TEST_CASE;

class ElevatorTestcaseParse : public QObject
{
    Q_OBJECT
public:
    ElevatorTestcaseParse(QString pFile);
    ~ElevatorTestcaseParse();

    void ClearJsTestcaseList();
    void ClearIniTestcaseList();
    bool SetTestFile(QString pFile);

public:
    bool                            mInvalid;
    QFileInfo                       mTestCaseFile;
    QHash<QString, JS_TEST_CASE*>   mJsTestcaseList;
    QHash<QString, INI_TEST_CASE*>  mIniTestcaseList;
};

/*
ini:
    [ErrorDescription]
    origin = x.x.x.x.x   x.x.x.x.x
    parsed = x.x.x.x.x   x.x.x.x.x
*/
#define D_FRAMEERROR_ORIGIN     QString("origin")
#define D_FRAMEERROR_PARSED     QString("parsed")
typedef struct{
    QList<QByteArray> mOriginFrameList;  //>@���͵�֡�б�
    QList<QByteArray> mParsedFrameList;  //>@�������֡�б����ڱȶ��Ƿ���ȷ
} INI_FRAME_ERROR;

class ElevatorFrameErrorParse : public QObject
{
    Q_OBJECT
public:
    ElevatorFrameErrorParse(QString pFile);
    ~ElevatorFrameErrorParse();

    void ClearIniFrameErrorList();
    bool SetFrameErrorFile(QString pFile);

public:
    bool                                mInvalid;
    QFileInfo                           mFrameErrorFile;
    QHash<QString, INI_FRAME_ERROR*>    mIniFrameErrorList;
};

class FLOOR_LOGO_INFO
{
public:
    FLOOR_LOGO_INFO()
    {
        clear();
    }
    void clear()
    {
        mFloorOrLogo = 0;
        mFloor = INVALIDRC;
        mLogo = INVALIDRC;
    }
    void SetFloor(quint32 pFloor)
    {
        mFloorOrLogo = 1;
        mFloor = pFloor;
    }
    void SetLogo(quint32 pLogo)
    {
        mFloorOrLogo = 2;
        mLogo = pLogo;
    }
    bool isFloor()
    {
        return (mFloorOrLogo == 1);
    }
    bool isLogo()
    {
        return (mFloorOrLogo == 2);
    }
    FLOOR_LOGO_INFO & operator=(const FLOOR_LOGO_INFO &s)
    {
        mFloorOrLogo = s.mFloorOrLogo;
        mFloor = s.mFloor;
        mLogo = s.mLogo;
        return *this;
    }
    quint32  mFloorOrLogo;  //>@0:null 1:floor 2:logo
    quint32  mFloor;
    quint32  mLogo;
};

/*
    ʵ��Э�鷢����֧��ͨ�����Խű����ɣ��Լ�Э�������֧�ִ����⣩�Ĺ���
    ������ԭ��
        ����֡�Ի�ȡĳһ������״̬��Ȼ���ȡ��Ӧ�����֡�б�����ȷ�����֡���бȶԡ�
*/
class PhysicalElevator : public PhysicalProtocol
{
    Q_OBJECT
public:
    PhysicalElevator(QObject *parent = 0);
    ~PhysicalElevator();

    //>@���Խű�
    bool InitTestcase(QString pFile);
    //>@�������
    bool InitFrameError(QString pFile);

    void GotoTest(bool pEnable, bool pNormal);
    void OpenWIFI();
    void CloseWIFI();

    virtual void Clear();

    void SetSimulate(bool pEnable) { mSimulateMode = pEnable; }
    void SetAutoTest(bool pEnable, bool pNormal);
    void SetTemporaryTest(bool pEnable, bool pNormal);

    QByteArray GetSavedFrame();

public slots:
    void slot_Initialize();
    void slot_NetState(bool pFlag);
    void slot_DebugTimeout();

public:
    bool                         mTemporaryTest;
    bool                         mSimulateMode;
    ElevatorTestcaseParse       *mTestcaseParse;
    ElevatorFrameErrorParse     *mFrameErrorParse;

    quint32                      mMonitorCount;   //>@

    bool                         mAutoTestEnable;
    NUM_INFO                     mAutoTestFreq;
    INI_TEST_CASE               *mTestcase;
    quint32                      mTestFrameNum;

    bool                         mDebugMonitorEn;  //>@��WIFI��������ʱ����Ϊfalse��
    int                          mDebugTimeoutCount;
    NUM_INFO                     mDebugTimeout;
    NUM_INFO                     mDebugFrame;

    bool                         mEnvSaved;
    FLOOR_LOGO_INFO              mFlrLogoInfoSaved;
    quint8                       mCheckFunSaved;

    bool                         mHasArrow;

    quint8  baiwei, shiwei, gewei, checkfun, raidofunc, audionum, extendfunc;

    QPointer<PhysicalLANC>       mLANC;
};

class PhysicalCOMHost : public PhysicalHost
{
    Q_OBJECT
public:
    PhysicalCOMHost(QString pAddr, QString pBufferPath=QString(), QObject *parent=0);
    ~PhysicalCOMHost();

    bool Scan();        //>@ɨ�軺��·���µĹ���

    bool isSync();
    bool Sync();
    void Async();

    bool Download(QString pThemeName, QString pThemePath=QString());
    bool Upload(QString pThemeName);

public:

};

class MonachThread : public QThread
{
    Q_OBJECT
public:
    MonachThread(BaseSerialport *qesp)
        :QThread()
    {
        mQesp = qesp;
        mRun = true;
    }
    ~MonachThread()
    {
        Stop();
    }
    void Stop()
    {
        mRun = false;
        //mQesp->Stop();
    }

    quint32 ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pFrameBuffer);

    BaseSerialport*       mQesp;
    bool                    mRun;
    QByteArray              mBuffer;

protected:
    virtual void run()
    {
        while (mRun)
        {
            //LOCK_MUTEX();
            if(mQesp->isOpen())
            {
                if(mQesp->bytesAvailable()>0)
                {
                    ParseFrameBuffer(P_INVALID, mQesp, mQesp->readAll());
                }
            }
            //UNLOCK_MUTEX();
            msleep(1);
        }
        mRun = true;
    }
};

class PhysicalCOM : public PhysicalElevator
{
    Q_OBJECT
public:
    PhysicalCOM(QObject *parent = 0);
    ~PhysicalCOM();

    bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);
    bool SetValue(QString pValue);

    void SetCanopenMode(int pMode);

    QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

    QString GetInformation()
    {
#ifndef IOS
        if(!mSerialPort)
            return QString("%1").arg(mDevName);
        return QString("%1>%2  %3").arg(mDevName).arg(mSerialPort->portName()).arg(mSerialPort->baudRate());
#endif
    }

    qint64 write(QByteArray data);
    qint64 reply(QByteArray data);

    void Bootup();
    quint32 ParseScandevFrame(FRAME_STRUCT *pFrame);
    quint32 ParseFloorFrame(FRAME_STRUCT *pFrame);

    bool PrepareRelease();
    bool ProcNoArrow(bool pForce=false);

    quint32 Monitor(quint32 pCount);

    bool WriteReg(quint16 pRegAddr, QByteArray pContent);
    QByteArray ReadReg(quint16 pRegAddr, quint32 pTimeout);

public slots:
    void slot_Initialize();
    void slot_DataEnable();

public:
    int                     mCanopenMode;
#ifndef IOS
    SerialPort             *mSerialPort;
#endif
    QList<QByteArray>       mFrameList;

    QString                 mInterBoardVersion;

    QString                 mPortName;
    BaudRateType            mPortBaud;
    ParityType              mPortParity;

    FLOOR_LOGO_INFO         mFloorLogoInfo;

    bool                    mDataEnable;  //>@����ʹ��

    int                     mNoArrowMode; //>@  0:���� 1���ȴ�����  2���л�ģʽ
    QString                 mNoArrowType;
    NUM_INFO                mNoArrowTimeout;
    int                     mNoArrowTimeoutCount;
    NUM_INFO                mNoArrowFreq;
    int                     mNoArrowFreqCount;
    bool                    mNoArrowFreqChange;

    bool                    m_IBMonitorEnable;
    WaitInfo                m_IBMonitorWait;

    bool                    m_Bootup;

private:
    bool                    isBusy;
};

#define D_AO_GONG1       40
#define D_AO_GONG2       41
#define D_AO_GONG3       42
#define D_AO_BEEP3       43
#define D_AO_DOOROPEN    44
#define D_AO_DOORCLOSE   45

class PhysicalSAF : public PhysicalElevator
{
    Q_OBJECT
public:
    PhysicalSAF(QObject *parent = 0);
    ~PhysicalSAF();

    bool open(OpenMode mode);
    void close();
    quint32 Monitor(quint32 pCount);

    bool Frame51();
    quint32 ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pByteArray);

    void SetFloor(quint8 pSAF, quint8 pTalk);
    void SetFloorEmpty();
    void SetAudio(quint8 pNum);
    void SetRadioFunc(quint8 pNum);
    void SetArrow(quint8 pEnable, quint8 pUp, quint8 pDown);
    void RollArrow(bool pEnable);

    QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

    bool SetValue(QString pValue);
    void Clear();

public slots:
    void slot_Initialize();

public:
    QFile                 *mFile;
    iniParse              *mAutoGenFrame;
    QByteArray             mReadByteArray;

    int                    mFrame51Mode; //>@  0:���� 1���ȴ�����  2���л�ģʽ
    int                    mFrame51FreqCount;
    NUM_INFO               mFrame51Freq;
    int                    mFrame51TimeoutCount;
    NUM_INFO               mFrame51Timeout;
    QString                mFrame51Type;

    bool                   mOpenMsgEnable;

    quint8                 realcheckfun, indicatecheckfun;     //>@����ָ���¼�ͷ������������

    quint8                 old_saf, old_flm, old_fld;

    QList<quint8>          audiolist;  //>@��֧��һ֡˳�򲥷Ŷ����Ƶ
    quint32                floortalkcount; //>@¥��ı�ʱ���㣬ָʾ��ǰ¥�㹲�����˼���talk
};

#endif // PHYSICALELEVATOR_H







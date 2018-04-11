#ifndef DEVFUN_H
#define DEVFUN_H

#include "bstui.h"

class DeviceManager;
#ifdef IDE
    class DeviceManager;
    class DeviceItem;
#endif

#ifdef SUPPORT_SCENE
class GraphicsScene;
#elif SUPPORT_WIDGET
class UiWidget;
#endif

#define D_PARASPLIT    '>'

#define D_MAXTIMEOUT     0x7fffffff

class DeviceMonitor;

class WaitInfo
{
public:
    WaitInfo()
    {
        mFreq = 1000;
        mNext = 1;   //>@�˳�ʼֵ��Ϊ0��ʹ����һ���ֻغ��ִ��Monitor�е����ݣ���Ϊ1������ִ��һ��Monitor�е����ݡ�
    }
    void SetTimeout(quint32 pValue);
    bool isValid(quint32 pCurCount, quint32 pFreq);
    void update(quint32 pCurCount, quint32 pFreq);  //>@���³�ʼ��
public:
    quint32  mFreq;        //>@���Ƶ��
    quint32  mNext;        //>@�´μ�����ڵļ���ֵ
};

//>@ɾ��������߼��ڵ�ʱ��Ҫ��ɾ������ڵ㣬��ɾ���߼��ڵ� �� ��Ҫ����ΪLG�豸��PHDeviceDeleted����s

//>@���Ŀؼ����ؼ����Զ������źŸ���XML��
//>@ִ�����̣����Ŀؼ����ؼ��Զ��ж��߼����Ƿ���Ը��ģ�������������XML�����򲻸���XML���ָ��ؼ�Ϊ��һ��״̬����ʱ�������ٴν����Զ��ж��߼����Ƿ���Ը��ģ���������ʲô�����ˡ�
//bool SetButton(ButtonList *pComponent, QDomElement &pElement);
//>@���ؼ�����ʱ��ͨ���������������£�������XML��
class LogicThemeManager;
class PhysicalLCD;
class IODeviceBase : public QIODevice
{
    Q_OBJECT
public:
    IODeviceBase(QObject *parent = 0);
    ~IODeviceBase();

    bool SetDeviceManager(QObject *pManager);

    //>@================================================================
    virtual bool isSequential() const { return true; }
    virtual bool open(OpenMode mode){ return true; }
    virtual void close(){}

    virtual qint64 pos() const  {return 0;}
    virtual qint64 size() const  {return 0;}
    virtual bool seek(qint64 pos)  {return true;}
    virtual bool atEnd() const  {return true;}
    virtual bool reset()  {return true;}

    virtual qint64 bytesAvailable() const  {return 0;}
    virtual qint64 bytesToWrite() const  {return 0;}

    virtual bool canReadLine() const  {return true;}

    virtual qint64 write(QByteArray data, QString pAddr) {return 0;} //>@��ĳһ��ַ���豸��������
    virtual qint64 reply(QByteArray data) {return QIODevice::write(data);}
    virtual inline qint64 write(const QByteArray &data) {return (QIODevice::write(data));}
    virtual QByteArray readAll(){return (QIODevice::readAll());}

    virtual bool waitForReadyRead(int msecs)  {return true;}
    virtual bool waitForBytesWritten(int msecs)  {return true;}

    virtual qint64 readData(char *data, qint64 maxlen)  {return 0;}
    virtual qint64 readLineData(char *data, qint64 maxlen)  {return 0;}
    virtual qint64 writeData(const char *data, qint64 len)  {return 0;}

    virtual bool isOpen()const{return (QIODevice::isOpen());}

    //>@================================================================
    //>@�ļ���������
    virtual bool copy(QString pSrc, QString pDst)  {return CopyFile(pSrc, pDst);}
    virtual bool cut(QString pSrc, QString pDst)  {return CutFile(pSrc, pDst);}
    virtual bool del(QString pPath)  {return DelFile(pPath);}

    virtual bool mkdir(QString pPath)  {return CreatPath(pPath);}
    virtual bool copydir(QString pSrc, QString pDst, bool subfolder) {return CopyFolder(pSrc, pDst, subfolder);}
    virtual bool movedir(QString pSrc, QString pDst)  {return MoveFolder(pSrc, pDst);}
    virtual bool deldir(QString pDir)  {return DelFolder(pDir);}
    virtual bool cleardir(QString pDir)  {return ClearFolder(pDir);}

    //>@================================================================
    bool SetElementText(QDomElement pElement, QString pValue);

    //>@================================================================
    virtual quint32 InitPara();  //>@���ش���Ĳ�������
    bool SetPara(QString pParaName, QString pParaValue, bool pSyncUI=true);
    bool SetPara(QDomElement pElement, QString pParaValue, bool pSyncUI=true);
    bool SetPara(QDomElement &pElement, xmlParse *pParse); //>@�޸�pElementΪ���Element
    QString GetPara(QString pParaName);
    QString GetPara(QDomElement pElement);
    bool GetPara(QDomElement &pElement, xmlParse *pParse);

    virtual bool WriteReg(quint16 pRegAddr, QByteArray pContent) {return true;}
    virtual QByteArray ReadReg(quint16 pRegAddr, quint32 pTimeout) {return QByteArray();}

    virtual bool PrepareRelease() {return true;}

    //>@================================================================
    //>@��ȡ���ڵ��е���Ҫ��Ϣ
    virtual QString GetInformation() { return QString(); }
    virtual bool SetValue(QString pValue) {return false;}  //>@ִ�з�������xmlparser�е�Element��ֵ������RTC���ڴ�ֵ�д��ָ���б�
    virtual QString GetValue(QString pValue) { return QString(); }
    //>@================================================================
    bool SaveMdp();

    virtual bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);

    virtual quint32 Monitor(quint32 pCount);

#ifdef SUPPORT_SCENE
    GraphicsScene *GetThemeScene();
#elif SUPPORT_WIDGET
    UiWidget *GetThemeScene();
#endif

public slots:
    virtual void UpdatePara(QObject* pObject);
    virtual void slot_Initialize();    //>@�������߼��豸������ȫ�����غú�ִ�д˺�����   //>@��ʼ����Ϻ���
    virtual void slot_UiShowed() {}
signals:
    void sOperateTheme(AREA_OPERATE pOperate, QVariant pPara);
    void sInfo(QString pMsg, int pDelay=3000, E_DEVMODE pMode=DM_NORMAL);
    void sWarn(QString pContent, int pDelay=3000, E_DEVMODE pMode=DM_NORMAL);
    void sError(QString pContent, int pDelay=3000, E_DEVMODE pMode=DM_NORMAL);
    void sProgress(quint32 pValue);
    void sProgress(quint32 pPrg, quint32 pExtraPrg);
    void sProgress(QString pMsg, quint32 pValue);
    void sProgress(QString pMsg, quint32 pValue, quint32 pExtraPrg);

    void sBeep(QString);  //>@���Ͳ���beep�ź�
    void sIcon(QString);  //>@������ʾͼ���ź�
    void sShowIcon(QString);  //>@������ʾͼ���ź�
    void sHideIcon(QString);  //>@��������ͼ���ź�
    void sFeedLCD();      //>@��������

    void sSetDevices(bool pEnable, E_DEVMODE pDevMode);

public:
    DeviceManager               *m_DevManager;
#ifdef IDE
    DeviceItem                  *m_DeviceItem;
    void DeleteDevItem();
#endif

    QPointer<LogicThemeManager>  mThemeManager;
    QPointer<PhysicalLCD>        m_LCD;

    QDomElement                  m_ParaElement;
    Groupbox                    *m_SettingGroup;

    QString                      mDevName;

    WaitInfo                     mMonitorWait;

    bool                         mEnable;           //>@�豸ʹ�ܿ���
    E_DEVMODE                    mDevMode;

    bool                         mInitialized;

public:
    QFrame* CreateLine();
    QHBoxLayout* CreateButton(QString pName, QStringList pBtnListTag);
    QHBoxLayout* CreateCheck(QString pName, QStringList pParaList, QStringList pSelectParaList);
    QHBoxLayout* CreateEnum(QString pName, QStringList pParaList, QString pSelectPara);
    QHBoxLayout* CreateBool(QString pName, QString pPara);
    QHBoxLayout* CreateNum(QString pName, int pValue, int pMinValue, int pMaxValue);
    QHBoxLayout* CreateENetAddrEdit(QString pName, QString pIP);
    QHBoxLayout* CreateDateTimeEdit(QString pName, QDateTime pDateTime);
    QHBoxLayout* CreateTextEdit(QString pName, QString pValue);

    bool ModifyButton(ButtonList *pComponent, QDomElement &pElement, QStringList &pDstValue, QStringList pNewValue);
    bool ModifyCheck(CheckList *pComponent, QDomElement &pElement, QStringList &pDstValue, QStringList pNewValue);
    bool ModifyEnum(ComboBox *pComponent, QDomElement &pElement, QString &pDstValue, QString pNewValue);
    bool ModifyBool(Switch *pComponent, QDomElement &pElement, bool &pDstValue, bool pNewValue);
    bool ModifyNum(SpinEdit *pComponent, QDomElement &pElement, int &pDstValue, int pNewValue, int pMinValue=-1, int pMaxValue=-1);
    bool ModifyENetAddrEdit(ENetAddrEdit *pComponent, QDomElement &pElement, ENET_ADDR &pDstValue, ENET_ADDR pNewValue);
    bool ModifyDateTimeEdit(DateTimeEdit *pComponent, QDomElement &pElement, QDateTime &pDstValue, QDateTime pNewValue);
    bool ModifyTextEdit(TextEdit *pComponent, QDomElement &pElement, QString &pDstValue, QString pNewValue);
};

class MiddlewareDevice;
class PhysicalDevice;
class LogicDevice;

class DevManager : public BstUiManager
{
public:
    DevManager();
    ~DevManager();

    virtual bool SetMonitor(quint32 pFreq);
    virtual quint32 Monitor(quint32 pCount) {return 0;}

public:
    DeviceMonitor                          *m_DevMonitor;
    int                                     m_DevMonitorFreq;

    QPixmap                                 m_TranparentPix;

    QHash<MW_Type, MiddlewareDevice*>       m_MWDevList;
    QHash<PH_Type, PhysicalDevice*>         m_PHDevList;
    QHash<LG_Type, LogicDevice*>            m_LGDevList;
};

class DeviceMonitor : QThread
{
    Q_OBJECT
    friend class DevManager;
public:
    DeviceMonitor(DevManager *qesp);
    ~DeviceMonitor();

    void ForceQuit();
    void Wait(int pTimeout);

    void run();

public:
    quint32                 m_Count;
    bool                    m_Quit;
    DevManager             *m_DevManager;
};

#endif // DEVFUN_H

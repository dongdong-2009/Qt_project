#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "global.h"
#include "physicalinc.h"
#include "logicinc.h"
#include "middlewareinc.h"
#include "bootlogo.h"
#ifdef IMX6X
#include "rencode.h"
#endif

#define D_RENCODE   (D_FSRCPATH + D_BOOTDIR + QString("rencode.png"))

#define D_PROGRESSDELAYTIME 10

#ifndef WINDOWS
void setShutDownSignal( int signalId );
void handleShutDown( int signalId );
#endif

/*
    0��ͨ��MDP������豸��
    1��ͨ���鿴DEV�ļ���ȷ��UDISK��TFlash���Ȳ���豸�Ƿ���ڣ�ֻ������豸����豸��
    2��ͨ���鿴/devĿ¼��ȷ���Ƿ����key����LED�ȷ��Ȳ���豸��ֻ������豸����豸��
    3�������豸����������ӳ���������߼��豸���ɼ��ء�ɾ��ĳ���豸���߼�Э����������豸
    4������ϵͳ��Ϣ��Devinfo�е���Ϣ������CPU��MEMռ���ʵȣ���
    5���������ò˵���MDP����
    6���ָ��������ã�������ʽ��TFlash������TFlash����UDisk���з����ȣ�
    7���豸���ƣ�����LED�������رա���ѯ״̬�ȣ�
*/
class DeviceManager : public QWidget, public DevManager
{
    Q_OBJECT
public:
    DeviceManager(QWidget *parent=0);
    DeviceManager(QRect pRect, QWidget *parent=0);
    ~DeviceManager();

    void InitEnvirHash();

    bool OpenBootMdp(QString pPath);
    void CloseBootMdp();
    bool SaveBootMdp();

    void ReleaseDevices();

    QString GetVersion(QString pName);

    bool AddMWDevice(MW_Type pMWType, QDomElement pMWTypeElement, bool pCover=false);
    bool DelMWDevice(MW_Type pMWType);
    bool AddPHDevice(PH_Type pPHType, QDomElement pPHElement, bool pCover=false);
    bool DelPHDevice(PH_Type pPHType);
    bool AddLGDevice(LG_Type pLGType, QDomElement pPHElement, bool pCover=false);
    bool DelLGDevice(LG_Type pLGType);

    quint32 Monitor(quint32 pCount);

    PhysicalDevice* GetPHDev(PH_Type pPHType);
    LogicDevice*    GetLGDev(LG_Type pLGType);

    void PrepareRelease();
    bool SetCmd(QString pCmd);

    bool eventFilter(QObject *o, QEvent *e);
    void paintEvent(QPaintEvent *e);

    void PlayBeep(QString pBeepName)  {emit sBeep(pBeepName);}
    void ShowIcon(QString pIconName)  {emit sIcon(pIconName);}

signals:
    void sProgress(QString pMsg, quint32 pPrg);
    void sProgress(QString pMsg, quint32 pPrg, quint32 pExtPrg);
    void sBeep(QString pBeepName);

    void sIcon(QString pIconName);
    void sShowIcon(QString);  //>@������ʾͼ���ź�
    void sHideIcon(QString);  //>@��������ͼ���ź�

    void sSetMenu();

public slots:
    void slot_InitDevices();
    void slot_Initialized();
    void slot_UiShowed();
    void slot_StartMonitor();

    void SetDevicesEnable(bool pFlag, E_DEVMODE pDevMode);

    void slot_Info(QString pMsg);
    void slot_Progress(quint32 pValue);
    void slot_Progress(QString pMsg, quint32 pValue);

protected:
    void _Initialize();

public:
    bool                                    m_DevFlag;
    E_DEVMODE                               m_DevMode;

    bool                                    m_DevSetBusy;

    QString                                 m_APPVersion, m_SYSVersion;

    QHash<QString, QString>                 m_EnvirHash;

    QString                                 mDevCode;  //>@�豸һά��
    QString                                 mPasswd;
    QString                                 mLocation;

    QPixmap                                 mRencodePix;
    mdpManager                             *mMdpManager;
    BootLogo                               *mBootLogo;
    bool                                    mPaintEnable;
};



#endif // DEVICEMANAGER_H

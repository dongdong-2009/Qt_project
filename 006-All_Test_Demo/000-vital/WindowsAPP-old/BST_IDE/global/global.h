#ifndef GLOBAL_H
#define GLOBAL_H

#include "globalfun.h"
#include "logfun.h"

class bstproManager;
class uiManager;
class mdpManager;
class stcManager;
class rcManager;
class cfgManager;
class mapManager;
class sevManager;

class IMAGEITEMINFO;
class Global;

#define D_MENU_NEW          tr("&New")
#define D_MENU_OPEN         tr("&Open")
#define D_MENU_CLOSE        tr("&Close")
#define D_MENU_ADD          tr("&Add")
#define D_MENU_DELETE       tr("&Delete")
#define D_MENU_CLEAR        tr("&Clear")
#define D_MENU_UPDATE       tr("&Update")
#define D_MENU_PERFERENCE   tr("&Preference")
#define D_MENU_OPTIONS      tr("&Options")
#define D_MENU_VIEW         tr("&View")

#define D_MENU_REFRESH      tr("&Refresh")
#define D_MENU_EXPLORER     tr("&Explorer")
#define D_MENU_SYNC         tr("&Sync")
#define D_MENU_ASYNC        tr("&ASync")
#define D_MENU_REGULAR      tr("&Regular")
#define D_MENU_DOWNLOAD     tr("&Download")
#define D_MENU_UPLOAD       tr("&Upload")
#define D_MENU_RUNNING      tr("&Set as Running")

extern Global GlobalPara;

void Logger(MSG_LEVEL pLevel, QString pMsg);
void Logger(QString Key = QString(), QStringList tmplist = QStringList());

void GetApplicationDirectory();

bool GetScreenInfo();
bool InitLoggerInfo();
bool GetBootConfigure();
bool InitializeLinguist();

#ifdef DISPLAY
    class DeviceManager;
#endif
class BootParaGen;

class Global : public QObject
{
    Q_OBJECT
public:
    explicit Global(QObject *parent=0);
    ~Global();

    QRect m_DeskRect;
    QRect m_ScreenRect;
    int   m_ScreenNum;

    QString m_SoftwareDir;          //>@Ӧ�ó���Ŀ¼

    //>@��־���

    QHash<QString, LOGTIP_INFO*> m_LogTipGroup;

    //>@QML���
#ifdef SURRPORT_QML
    QDeclarativeEngine *m_QMLEngine;
#endif

    //>@BOOT.XML��ȫ�ֱ���
    QHash<QString, QDomElement>         NewCategoryGroup;               //>@�½�form��֧�ֵ����ͼ���
    QHash<QString, SolutionCategory>    SolutionCategoryGroup;          //>@֧�ֵ���Ŀ����
    QHash<QString, FileCategory>        FileCategoryGroup;              //>@֧�ֵ��ļ�����

    QHash<QString, ComCategory*>        m_ComCategoryGroup;             //>@���õĽ������

    QHash<QString, QDomElement>         m_ConfigInfoGroup;
    QHash<MW_Type, QDomElement>         m_MWonfigInfoGroup;             //>@�м��
    QHash<PH_Type, QDomElement>         m_PHonfigInfoGroup;
    QHash<LG_Type, QDomElement>         m_LGonfigInfoGroup;

    QHash<QString, QDomElement>         m_TestCaseGroup;                //>@

    IMAGEITEMINFO *m_ActiveImageItemInfo;

#ifdef DISPLAY
    DeviceManager *m_DeviceManager;
    BootParaGen   *m_BootParse;
#endif
#if (defined(IDE) || defined(PRODUCT))
    QMainWindow   *m_MainWindow;

    QToolBar      *m_ProjectBar;
    QTabWidget    *m_TabPageManager;
    QToolBar      *m_SqlToolBar;
    QToolBar      *m_DockViewToolBar;

    //״̬���б�ǩ
    QStatusBar    *statusBar;
    QLabel        *docEditStatus,
                  *keyboardStatus,
                  *searchStatus;
    QProgressBar  *progressStatus;
    QTextEdit     *m_OutputMsgList;
#endif

public slots:
    void showMessage(QString pShowMsg);
    void outputMessage(QString pShowMsg);
    void updateProgress(int pProgress);
    void updateProgress(QString pShowMsg, int pProgress);
};

/*
    XX.bstpro
        xx.ui
        xx.map
        xx.stc
        xx.sto
*/

class bstproManager : public xmlParse
{
    Q_OBJECT
public:
    bstproManager(QString fileName);

    ~bstproManager(){}

    static const QString BstProSuffix;
    static const QString BstProFileType;

    bool NewPro();
    bool OpenPro();
    bool ClosePro();
    bool DelPro();
    bool SavePro();
    bool RenamePro(QString FileName);

    bool AddFile(QString pFileName);
    bool AddFile(QString pKey, QString pFileName);
    bool DelFile(QString pKey);
    QString GetFile(QString pKey);
};

/*
    <Desktop>
        <Rect>
            <Horizontal>0,0,800,600</Horizontal>
            <Vertical>0,0,600,800</Vertical>
        </Rect>
        <StateMachine>
            <Common EffectType="Image" EffectPara="">
                <RC></RC>
            </Common>
            <Change EffectType="Roll" EffectPara="">
                <RC></RC>
            </Change>
        </StateMachine>
    </Desktop>

    �������Զ������㷨�� �������⣬���ھ���ͼ��Ԫ�ص�������뱣֤�����ͬ���޸�����ʱ��Ҫʵʱ�ж��Ƿ��߳�������������Ŀ��
    1�������Զ�����ԭ��
        �����������������Ͻ����궼Ϊ0�����ʾ��δ�Զ����ֹ��������һ���Զ����֣����򲻶���������Զ�����
            ��������������ŷţ����ȼ�ȡ����COM_TYPE�Ĵ�С
        �����ڲ���������ж����ŷţ�Ҳ�������һ�����ŵڶ��е�ԭ�����ȼ�ȡ����COM_TYPE�Ĵ�С
            ͨ����ά�����¼
            һ����������������ÿһ�ж���ƽ�������϶��������Ϊ���������+1����
            ���з��棬ÿ��������Լ����ڵ�����Ҫ����
        �����������ڲ������е���������������С��С���������Ĵ�С���ڴ���С��С���򲻱䣬�����������յ������и߶�ʹ����С�߶ȣ����ʹ��������

    2�������Զ�����ԭ��
        �Ӳ�����������Զ����֣���ȫ���û������Ϸ����ã����һ����ƽ���ʱ����ƺ�棬���������
        �����ڲ���������ж����ŷţ�Ҳ�������һ�����ŵڶ��е�ԭ�����ȼ�ȡ����COM_TYPE�Ĵ�С
        �����������ڲ������е���������������С��С���������Ĵ�С���ڴ���С��С���򲻱䣬�����������յ������и߶�ʹ����С�߶ȣ����ʹ����С���

    3���϶������ͬ��ԭ��
        �������򣬲���Ҫ�����κμ��
        �����������Ҫʵʱ������Ȼ��߸߶ȣ��鿴�����ڷ�λ�Ƿ��Ѿ�������ǰ����������棬����ǣ�����ʾע�⣬���ִ���޸ģ���������ǰ����ĺ����л�����ʹ֮������Ӧһ�ֽ���

    4�������ʼ����������ԭ��
        ������鿴�Լ����е��ӿؼ��Ƿ��������⣬����ǣ����Զ��������ӿؼ�������С��
        Ȼ��鿴�����Ƿ��������⣬����ǣ��������������

*/
inline QRectF ToSceneRect(QRectF pRect)
{
#ifdef IDE
    return QRectF(pRect.left() + StartPosX, pRect.top() + StartPosY, pRect.width(), pRect.height());
#elif DISPLAY
    return pRect;
#endif
}

inline QRectF FromSceneRect(QRectF pRect)
{
#ifdef IDE
    return QRectF(pRect.left() - StartPosX, pRect.top() - StartPosY, pRect.width(), pRect.height());
#elif DISPLAY
    return pRect;
#endif
}

#define D_RCINVALID      QString("Invalid")
#define D_EFFECTTYPE     QString("EffectType")
#define D_EFFECTPARA     QString("EffectPara")

class RC_MAP
{
public:
    RC_MAP(){}
    bool operator==(const RC_MAP &s)
    {
        if(mSrc.compare(s.mSrc))
            return false;
        if(mMap.compare(s.mMap))
            return false;
        return true;
    }
    bool operator!=(const RC_MAP &s)  { return !operator==(s); }
    RC_MAP & operator=(const RC_MAP &s)
    {
        mSrc = s.mSrc;
        mMap = s.mMap;
        return *this;
    }
public:
    QString mSrc;
    QString mMap;
};

class QUiManager : public xmlParse
{
    Q_OBJECT
public:
    QUiManager(QString pFileName);
    void GetComponentGroup(QDomElement pElement);

    QRect GetRect(COM_TYPE pType);
    void ModifyRect(COM_TYPE pType, QRect pRect);

public:
    QHash<COM_TYPE, QDomElement>  m_ComElementGroup;
};

class uiManager : public xmlParse
{
    Q_OBJECT
public:
    uiManager(QString fileName);

    static const QString UiSuffix;
    static const QString UiFileType;

    //>@����ui�ļ�
    bool NewUI();
    bool OpenUI();
    bool CloseUI();
    bool DelUI();
    bool SaveUI();
    bool RenameUI(QString FileName);

    bool isValid();

    QString GetSolution();
    bool SetSolution(QString pSolution);
    DIRECTION GetDirectionPermisson();
    bool SetDirectionPermisson(DIRECTION pDirection);
    bool canSwitch() { return (m_DirectionPermisson == HVBoth); }

    //>@�����ļ��е�Ԫ��
    void GetComponentGroup();

    //>@�����ļ���ȫ·��
    FILEPATH_TYPE GetRcFileType(QString pFileName);
    FILEPATH_TYPE RegulatRcFile(QString& pFileName);
    //>@�鿴�Ƿ�Ϊ���·��
    bool IsRelativePath(QString pFileName);
    //>@������Դ��Ŀ��·��
    bool RegularFile(COM_TYPE pComType, QString pRcName, QString &pRcFile, QString pDstPath, QSize pSize);

    bool GetComRect(int pLayout, QDomElement pComElement, QRect &pHRect, QRect &pVRect);
    QRect GetComRect(int pLayout, QDomElement pComElement, DIRECTION pDirection);
    bool ModifyComRect(int pLayout, QDomElement pComElement, QRect pRect, DIRECTION pDirection, bool pForceNew = false);

    QDomNodeList GetComStateList(QDomElement pComElement);
    QDomElement GetComState(QDomElement pComElement, QString pStateName, bool pCreateFlag = false); //>@pCreateFlag������������Զ�����
    bool ModifyEffectType(QDomElement pStateElement, QString pEffectType);
    QDomElement ModifyComState(QDomElement pComElement, QString pStateName, QString pEffectType, QString pEffectPara);

    bool ModifyEffectPara(QDomElement pStateElement, QString pEffectPara);

    //>@��ȡĳ������ĸ�QDomElement��ͬʱ�鿴�Ƿ�Ϊ��������Դ·����������д�����·��
    bool GetParentElement(QDomElement &pParentElement, QString pComPath);
    //>@��ȡ����ĸ����
    QDomElement GetParComDom(QDomElement pComElement);
    QDomElement GetParComDom(QString pComPath);

    bool AddMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
    bool DelMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
    QList<RC_MAP> GetMapList(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);
    QHash<QString, QString> GetMapHash(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);

    //>@���һ���ؼ���Dom��Դ
    QDomElement AddComDom(COM_TYPE pComType, QRect pRect, DIRECTION pDirection);
    //>@��Dom��ɾ��һ���ؼ���Dom��Դ
    bool DeleteComDom(COM_TYPE pComType);

    //>@����Դȫ������Ϊ���·�������洢��UI��Ӧ���ļ�����
    bool SaveRelativeComDom(QDomElement pComElement);

public:
    //>@xx.ui�ļ��е������subui��control����DomԪ��(xx.ui)
    QHash<COM_TYPE, QDomElement>  m_ComElementGroup;
    DIRECTION                     m_DirectionPermisson;
};

class mdpManager : public xmlParse
{
    Q_OBJECT
public:
    mdpManager(QString fileName);

    //>@�����ļ�
    bool NewMDP();
    bool OpenMDP();
    bool CloseMDP();
    bool DelMDP();
    bool SaveMDP();
    bool RenameMDP(QString FileName);

    //>@�����ļ��е�Ԫ��
    bool GetCfgparaGroup();

    bool SetMonitor(quint32 pFreq);
    quint32 GetMonitor();

    bool SetPassword(QString pPasswd);
    QString GetPassword();

    bool SetLocation(QString pLocation);
    QString GetLocation();

    bool ShowLog(bool pEnable);
    bool CanShowLog();

    bool SetPattern(QString pPattern);
    QString GetPattern();

    QString GetVersion();

    QDomElement AddDeviceDom(QString pPath, QDomElement pInitElement);
    void DelDeviceDom(QString pPath);
    void DelDeviceDom(MW_Type pType, QString pPath);
    void DelDeviceDom(PH_Type pType, QString pPath);
    void DelDeviceDom(LG_Type pType, QString pPath);
    QDomElement CreateParaDom(QDomElement pInitElement);
    bool ClrMDP();
    bool ClrDomCldNodes(QDomElement pElement);
    bool DelDom(QDomElement pElement);

public:
    static const QString MdpSuffix;
    static const QString MdpFileType;

    QHash<QString, QDomElement> m_CfgparaElementGroup;
    QHash<MW_Type, QDomElement> m_MWElementGroup;
    QHash<PH_Type, QDomElement> m_PHElementGroup;
    QHash<LG_Type, QDomElement> m_LGElementGroup;
};

class devManager : public xmlParse
{
    Q_OBJECT
public:
    devManager(QString fileName);

    ~devManager(){}

    static const QString DevSuffix;
    static const QString DevFileType;

    //>@����ui�ļ�
    bool NewDEV();
    bool OpenDEV();
    bool CloseDEV();
    bool DelDEV();
    bool SaveDEV();
    bool RenameDEV(QString FileName);

    //>@�����ļ��е�Ԫ��
    bool GetDevGroup() {return true;}

    bool SetCode(QString pCode);
    QString GetCode();

public:
    QHash<PH_Type, QDomElement>   m_DevInfoGroup;
};

/*
    <Device>  //>@�豸��ۼ��ӿڶ��壬���豸ƽ��Ϊ�ο���
        <Top img="">
            <Chip>
                <Core nickname="" desc="" rect="10,10,300,300"/>   //>@��оƬ
                <Power nickname="" desc=""/> //>@��ԴоƬ
                <Interface nickname="" desc=""/>  //>@�ӿ�оƬ
            </Chip>
            <Input>
                <Key num="" />  //>@num�����д��Ĭ��Ϊ1�����������͸���numȷ�������Ϊ8λ����Ϊchar����
                <Dial num="" />
                <COM0 num="" />
                <USB0 num="2" subtype="host" />
                <USB1 num="1" subtype="device" />
            </Input>
            <Output>
                <LCD rect="10,10,300,300" />
                <LED num="1"/>
            </Output>
        </Top>
        <Bottom/>
        <Front/>
        <Behind/>
        <Left/>
        <Right/>
    </Device>
    <AutoTest>  :�Զ�����
        <Path loop="0" >
            <Step delay="" loop="" waitusr=""(�Ƿ�ȴ��û�����:0-���ȴ���1-���������ȴ����룬2-������ť�ȴ�OK��ť�����ڲ���ͬ����)>
                <Input>
                    <Key>1,0,0,0</Key>
                    <COM0>0x80,0x01,...</COM0>
                </Input>
                <Output> //>@ָӦ����ʲô�������
                </Output>
            </Step>
        </Path>
    </AutoTest>
    <ModuleTest>:ģ����ԣ���Ҫ���ڲ����û����ĵ���ع��ܵ㣬ͨ����ק�ұ����в���ģ����ɾ����ģ�飬ScriptEngine�����в����������Ÿ�js�ļ���
        <BalancingLoad Desc="ƽ��ϵͳ���أ�ʹ֮��������">
            <CPU Para="num" List="50;70" Desc="��CPUռ����С�ڴ����õ�ֵʱ��Ϊϵͳ����������"></CPU>
            <MEM Para="num" List="50;70"></MEM>
            <TestCase Para="file">js/cpu.js</TestCase> ���� <TestCase Para="string">alert("hello")</TestCase>
        </BalancingLoad>
    </ModuleTest>
    <ProtocolTest>  :Э�����
        <Generator>
            <SAF>
                <SF Para="bool" Desc="����Ϊ�ߵ͵�ƽ">false</SF>
                <TALK Para="bool">false</TALK>
                <SGO Para="bool">false</SGO>
                <FLD Para="bool">false</FLD>
                <FLM Para="bool">false</FLM>
                <SAF Para="num" List="1;70" Desc="����֡����Ϊxx"></SAF>
                <QuickLaunch Para="button" List="a;b;c;d"></QuickLaunch>
            </SAF>
        </Generator>
        <Analyzer>
            <BST3GP>
                <Hundread Para="num" List="32;126">32</Hundread>
                <Ten Para="num" List="32;126">32</Ten>
                <Single Para="num" List="32;126">32</Single>
                <CheckFun Para="check" List="ArrowUP;ArrowDown;Run"></CheckFun>
                <RadioFun Para="num" List="1;127">false</RadioFun>
                <Audio Para="num" List="1;127">false</Audio>
                <ExtendFun Para="check" List="Floor;Arrow;Function;Audio"></ExtendFun>
                <QuickLaunch Para="button" List="a;b;c;d"></QuickLaunch>
            </BST3GP>
        </Analyzer>
    </ProtocolTest>
    <Integrity>xxx.js</Integrity>
*/

class stcManager : public xmlParse
{
    Q_OBJECT
public:
    stcManager(QString fileName);
    ~stcManager(){}

    static const QString StcSuffix;
    static const QString StcFileType;

    //>@����ui�ļ�
    bool NewSTC();
    bool OpenSTC();
    bool CloseSTC();
    bool DelSTC();
    bool SaveSTC();
    bool RenameSTC(QString FileName);

    //>@�����ļ��е�Ԫ��
    bool GetStcGroup();

public:
    QHash<QString, QDomElement> m_ModuleCaseGroup;
    QDomElement                 m_GeneratorElement;
    QList<QDomElement>          m_AnalyzerGroup;
};

class rcManager : public xmlParse
{
    Q_OBJECT
public:
    rcManager(QString fileName);

    ~rcManager(){}

    static const QString RcSuffix;
    static const QString RcFileType;

    //>@����ui�ļ�
    bool NewRC();
    bool OpenRC();
    bool CloseRC();
    bool DelRC();
    bool SaveRC();
    bool RenameRC(QString FileName);

    //>@�����ļ��е�Ԫ��
    bool GetRcGroup();
};

class cfgManager : public xmlParse
{
    Q_OBJECT
public:
    cfgManager(QString fileName);

    ~cfgManager(){}

    static const QString BootFileName;
    static const QString CfgFileType;

    bool getNewCategory();
    bool getSolutionCategory();
    bool getFileCategory();

    void getEffectAndRcTypes(QDomElement pElement, ComCategory *pComCategory);
    bool getComponetCategory();

    //>@ /Device/RMII
    bool getCfgparaGroup();

    bool getTestcaseGroup();
};

class cmdManager : public xmlParse
{
    Q_OBJECT
public:
    cmdManager();                   //>@����XML��ʽ������
    cmdManager(QString fileName);   //>@����XML�ļ�

    ~cmdManager(){}

public:
    //>@����XML�ļ�
    static const QString CmdSuffix;
    static const QString CmdFileType;

    //>@����ui�ļ�
    bool NewCMD();
    bool OpenCMD();
    bool CloseCMD();
    bool DelCMD();
    bool SaveCMD();
    bool RenameCMD(QString FileName);

    //>@�����ļ��е�Ԫ��
    bool GetCMDGroup();

    QHash<QString, QDomElement> m_CfgItemGroup;
};

class MEDIA_INFO
{
public:
    MEDIA_INFO()
    {
    }
    ~MEDIA_INFO()
    {
    }
    QString mFuncMap;
    QString mImage;
    QString mAudio;
    QString mVideo;
    QString mLogo;
    QString mText;
    MEDIA_INFO & operator=(const MEDIA_INFO &s)
    {
        mFuncMap = s.mFuncMap;
        mImage = s.mImage;
        mAudio = s.mAudio;
        mVideo = s.mVideo;
        mLogo = s.mLogo;
        mText = s.mText;
        return *this;
    }
    bool operator==(const MEDIA_INFO &s)
    {
        if(mFuncMap != s.mFuncMap)
            return false;
        if(mImage != s.mImage)
            return false;
        if(mAudio != s.mAudio)
            return false;
        if(mVideo != s.mVideo)
            return false;
        if(mLogo != s.mLogo)
            return false;
        if(mText != s.mText)
            return false;
        return true;
    }
};

typedef struct{
    QString    mEventValue;
    MEDIA_INFO mMediaInfo;
} MAPITEM_INFO;

class mapManager : public xmlParse
{
    Q_OBJECT
public:
    mapManager(QString fileName);

    ~mapManager(){}

    static const QString MapSuffix;
    static const QString MapFileType;

    QHash<QString, QDomElement> m_CfgparaElementGroup;
    QMultiHash<QString, MAPITEM_INFO*> m_MapItemInfoGroup;

    //>@����map�ļ�
    bool NewMAP();
    bool OpenMAP();
    bool CloseMAP();
    bool DelMAP();
    bool SaveMAP();
    bool RenameMAP(QString FileName);

    bool SetUIPath(QString UiPath);
    QString GetUIPath();
    bool GetCfgparaGroup();

    QDomElement GetMapElement(MAP_EVENT pTypeFilter);
    bool SetMapElement(MAP_EVENT pType, QString pEvent, QString pActions);

    bool ClrMap();
    bool ClrDomCldNodes(QDomElement pElement);
    bool DelDom(QDomElement pElement);
    bool AddDomNewNode(DomInfo pDomInfo);

    bool MapItemToElement(MAPITEM_INFO *pMapItemInfo, QDomElement &pParentElement);
    MAPITEM_INFO * ElementToMapItem(QDomElement pElement);

    bool GetMapItemGroup();
    bool SaveMapItemGroup();

    bool AddMapItem(QString pEventName, QString pEventValue, MEDIA_INFO mMediaInfo);
    bool DelMapItem(QString pEventName, QString pEventValue);
    bool ModifyMapItem(QString pEventName, QString pEventValue, MEDIA_INFO mMediaInfo);
    MAPITEM_INFO *GetMapItem(QString pEventName, QString pEventValue);
};

/*>@
    �����豸��ǰ׺��
        ��һ���ַ���
            C-��ʾcop
            L-��ʾlop
            H-��ʾ����
            S-��ʾ�����
            E-��ʾ���ݿ�����
            D-��ʾ�Ż�������
        �ڶ����ַ���
            5-���ػ�
            6-������ͼƬ��
            7-�����ͼƬ��
            8-��������Ƶ��
            9-�������Ƶ��
        �������ַ���
            CODE��һά�룩
*/
class ElevatorNameParse
{
    ElevatorNameParse(QString pName)
    {
        *this = pName;
    }
    bool operator==(const ElevatorNameParse &s)
    {
        for(int i=0;i<2;i++)
        {
            if(mType[i] != s.mType[i])
                return false;
        }
        if(mCode != s.mCode)
            return false;
        return true;
    }
    bool operator!=(const ElevatorNameParse &s)  { return !operator==(s); }

    ElevatorNameParse & operator=(const ElevatorNameParse &s)
    {
        for(int i=0;i<2;i++)
            mType[i] = s.mType[i];
        mCode = s.mCode;
        return *this;
    }
    ElevatorNameParse & operator=(QString s)
    {
        if(s.count() > 2)
        {
            mType[0] = s.at(0).toLatin1();
            mType[1] = s.at(1).toLatin1();
            mCode = s.mid(2).toLatin1();
        }
        return *this;
    }
public:
    quint8  mType[2];
    QByteArray  mCode;
};

class sevManager : public xmlParse
{
    Q_OBJECT
public:
    sevManager(QString fileName);
    ~sevManager(){}

    static const QString SevSuffix;
    static const QString SevFileType;

    //>@����sev�ļ�
    bool NewSev();
    bool OpenSev();
    bool CloseSev();
    bool DelSev();
    bool SaveSev();
    bool RenameSev(QString FileName);

    bool GetElevatorGroup();

public:
    QHash<QString, QDomElement> m_ElevatorElementGroup;
};

class devInfoManager : public xmlParse
{
    Q_OBJECT
public:
    devInfoManager(QString fileName);
    ~devInfoManager(){}

    static const QString DevInfoSuffix;
    static const QString DevInfoFileType;

    //>@����ui�ļ�
    bool NewDevInfo();
    bool OpenDevInfo();
    bool CloseDevInfo();
    bool DelDevInfo();
    bool SaveDevInfo();
    bool RenameDevInfo(QString FileName);
};

#define D_FONTFAMILY  QString("family")
#define D_FONTSIZE    QString("size")
#define D_FONTCOLOR   QString("color")

class FONT_INFO
{
public:
    FONT_INFO()
    {
        mFont = QFont("Arial", 11, QFont::DemiBold, false/*italic*/);
        mColor = Qt::black;
        mValid = true;
    }
    FONT_INFO(QFont pFont, QColor pColor)
    {
        mFont = pFont;
        mColor = pColor;
        mValid = true;
    }
    FONT_INFO(QString pPara)
    {
        *this = pPara;
    }
    FONT_INFO & operator=(const FONT_INFO &s)
    {
        mFont = s.mFont;
        mColor = s.mColor;
        mValid = s.mValid;
        return *this;
    }
    bool isValid()
    {
        return mValid;
    }
    FONT_INFO & operator=(QString pPara)  /*family:Arial;size:11;color:0,0,0,0*/
    {
        if(pPara.isEmpty())
            mValid = false;
        mValid = true;
        QStringList tmpParaList = pPara.split(';');
        for(int i=0;i<tmpParaList.count();i++)
        {
            QString tmpString = tmpParaList.at(i);
            QStringList tmpChildParaList = tmpString.split(':');
            if(tmpChildParaList.count() != 2)
            {
                IDE_TRACE();
                mValid = false;
                continue;
            }
            QString tmpChildString = tmpChildParaList.at(0);
            if(tmpChildString.compare(D_FONTFAMILY, Qt::CaseInsensitive) == 0)
            {
                mFont.setFamily(tmpChildParaList.at(1));
            }
            else if(tmpChildString.compare(D_FONTSIZE, Qt::CaseInsensitive) == 0)
            {
                mFont.setPointSize(tmpChildParaList.at(1).toInt());
            }
            else if(tmpChildString.compare(D_FONTCOLOR, Qt::CaseInsensitive) == 0)
            {
                QString tmpColorString = tmpChildParaList.at(1);
                QStringList tmpColorItems = tmpColorString.split(',');
                if(tmpColorItems.count() == 3)
                    mColor.setRgb(tmpColorItems.at(0).toInt(),
                                  tmpColorItems.at(1).toInt(),
                                  tmpColorItems.at(2).toInt());
                else if(tmpColorItems.count() == 4)
                    mColor.setRgb(tmpColorItems.at(0).toInt(),
                                  tmpColorItems.at(1).toInt(),
                                  tmpColorItems.at(2).toInt(),
                                  tmpColorItems.at(3).toInt());
                else
                {
                    IDE_TRACE();
                    mValid = false;
                }
            }
        }
        return *this;
    }
    QString toString()
    {
        if(mValid == false)
            return QString();
        return QString("%1:%2;%3:%4;%5:%6,%7,%8,%9")
                      .arg(D_FONTFAMILY).arg(mFont.family())
                      .arg(D_FONTSIZE).arg(mFont.pointSize())
                      .arg(D_FONTCOLOR).arg(mColor.red()).arg(mColor.green()).arg(mColor.blue()).arg(mColor.alpha());
    }
    bool operator==(const FONT_INFO &s)
    {
        if(mFont != s.mFont)
            return false;
        if(mColor != s.mColor)
            return false;
        if(mValid != s.mValid)
            return false;
        return true;
    }
    QFont  mFont;
    QColor mColor;
    bool   mValid;
};

class fontManager : public xmlParse
{
    Q_OBJECT
public:
    fontManager(QString fileName);
    ~fontManager(){}

    static const QString FontSuffix;
    static const QString FontFileType;

    //>@����ui�ļ�
    bool NewFont();
    bool OpenFont();
    bool CloseFont();
    bool DelFont();
    bool SaveFont();
    bool RenameFont(QString FileName);

    bool GetParagraphGroup();

    bool AddParagraph(QString pDesc, QString pString, FONT_INFO pInfo);
    bool ModifyParagraph(QString pDesc, QString pString, FONT_INFO pInfo);
    bool DelParagraph(QString pDesc);

    bool GetParagraph(QString pDesc, QString &pString, FONT_INFO &pInfo);

    QHash<QString, QDomElement> m_ParagraphGroup;
};

class updateManager : public xmlParse
{
    Q_OBJECT
public:
    updateManager(QString fileName);
    ~updateManager(){}

    static const QString UpdateSuffix;
    static const QString UpdateFileType;

    //>@����ui�ļ�
    bool OpenUpdate();
    bool CloseUpdate();
    bool SaveUpdate();

    QString GetVersion(QString pName);
};

/*Burn�ļ����壺
    <?xml version='1.0' encoding='UTF-8'?>
    <root>
        <FileType>BST BURN File</FileType>
        <Code>BST</Code>
        <HardwareCheck>
            <LostMaterial>
                <���ɿ�>U1;U2</���ɿ�>
            </LostMaterial>
            <ShortCircuit>
                <���ɿ� type="�麸">U1;U2</���ɿ�>
            </ShortCircuit>
            <PowerSuface></PowerSuface>
        </HardwareCheck>
        <UsbBurn>
            <Program1>true</Program1>
            <Program2>false</Program2>
        </UsbBurn>
    </root>
*/

class burnManager : public xmlParse
{
    Q_OBJECT
public:
    burnManager(QString fileName);
    ~burnManager(){}

    static const QString BurnSuffix;
    static const QString BurnFileType;

    //>@����Burn�ļ�
    bool NewBurn();
    bool OpenBurn();
    bool CloseBurn();
    bool DelBurn();
    bool SaveBurn();
    bool RenameBurn(QString FileName);
};

#endif // GLOBAL_H


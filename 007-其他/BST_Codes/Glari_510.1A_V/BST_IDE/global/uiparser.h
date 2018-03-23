#ifndef UIPARSER_H
#define UIPARSER_H

#include "xmlparser.h"

//>@---------------UI��������-------------------

UI_TYPE GetUiType(QString pUiName);
QString GetUiName(UI_TYPE pUiType);
QString GetUiPath(UI_TYPE pType);
UI_TYPE GetParentUiType(QString pUiPath);
UI_TYPE GetParentUiType(UI_TYPE pType);
QString GetCurUiName(QString pUiPath);
QString GetParentUiPath(QString pUiPath);
QString GetUiPath(QString pUiName);
UI_TYPE GetCurUiType(QString pUiPath);

/*
    �������Զ������㷨�� �������⣬���ھ���ͼ��Ԫ�ص�������뱣֤�����ͬ���޸�����ʱ��Ҫʵʱ�ж��Ƿ��߳�������������Ŀ��
    1�������Զ�����ԭ��
        �����������������Ͻ����궼Ϊ0�����ʾ��δ�Զ����ֹ��������һ���Զ����֣����򲻶���������Զ�����
            ��������������ŷţ����ȼ�ȡ����UI_TYPE�Ĵ�С
        �����ڲ���������ж����ŷţ�Ҳ�������һ�����ŵڶ��е�ԭ�����ȼ�ȡ����UI_TYPE�Ĵ�С
            ͨ����ά�����¼
            һ����������������ÿһ�ж���ƽ�������϶��������Ϊ���������+1����
            ���з��棬ÿ��������Լ����ڵ�����Ҫ����
        �����������ڲ������е���������������С��С���������Ĵ�С���ڴ���С��С���򲻱䣬�����������յ������и߶�ʹ����С�߶ȣ����ʹ��������

    2�������Զ�����ԭ��
        �Ӳ�����������Զ����֣���ȫ���û������Ϸ����ã����һ����ƽ���ʱ����ƺ�棬���������
        �����ڲ���������ж����ŷţ�Ҳ�������һ�����ŵڶ��е�ԭ�����ȼ�ȡ����UI_TYPE�Ĵ�С
        �����������ڲ������е���������������С��С���������Ĵ�С���ڴ���С��С���򲻱䣬�����������յ������и߶�ʹ����С�߶ȣ����ʹ����С���

    3���϶������ͬ��ԭ��
        �������򣬲���Ҫ�����κμ��
        �����������Ҫʵʱ������Ȼ��߸߶ȣ��鿴�����ڷ�λ�Ƿ��Ѿ�������ǰ����������棬����ǣ�����ʾע�⣬���ִ���޸ģ���������ǰ����ĺ����л�����ʹ֮������Ӧһ�ֽ���

    4�������ʼ����������ԭ��
        ������鿴�Լ����е��ӿؼ��Ƿ��������⣬����ǣ����Զ��������ӿؼ�������С��
        Ȼ��鿴�����Ƿ��������⣬����ǣ��������������
*/

#define D_INVALID       QString("Invalid")
#define D_TYPE          QString("Type")
#define D_PARA          QString("Para")

#define PARA_CURVESHAPE  QString("AnimateType")  //>@��Ч��������
#define PARA_FRAMERANGE  QString("FrameRange")   //>@��Ч֡Ƶ��
#define PARA_LOOPCOUNT   QString("LoopCount")    //>@ѭ������
#define PARA_INTERVAL    QString("Interval")     //>@��Чʱ��
#define PARA_DIRECTION   QString("Direction")    //>@��Ч����
#define PARA_ORIENTATION QString("Orientation")  //>@��Ч������
#define PARA_AMOUNT      QString("Amount")       //>@����
#define PARA_DUTYRATIO   QString("DutyRatio")    //>@ռ�ձ�
#define PARA_AUTOPLAY    QString("AutoPlay")
#define PARA_RCTYPE      QString("RcType")       //>@��Դ���ͣ�Image/Text

#define PARA_LAYOUT      QString("Layout")       //>@����
#define PARA_ROW         QString("Row")          //>@������������
#define PARA_COLUMN      QString("Column")       //>@������������
#define PARA_RSPACE      QString("RowSpace")     //>@������Ԫ�غ�����
#define PARA_CSPACE      QString("ColumnSpace")  //>@������Ԫ��������

#define PARA_STYLE       QString("Style")        //>@������ʽ
#define PARA_ALIGN       QString("Align")
#define PARA_PATTERN     QString("Pattern")

//>@3D
#define PARA_ANGLEDIFF   QString("AngleDiff")       //>@һ����ת�Ƕ�

typedef int         ( *_StrToEnum )  ( QString );
typedef QVariant	( *_StrToValue )  ( int, QString );

typedef QString     ( *_EnumToStr )  ( int );
typedef QString     ( *_ValueToStr )  ( int, QVariant );

typedef int         ( *_StrToInt )  ( QString );

typedef bool     	( *_ParsePara )  ( QString, QString, QHash<int, QVariant> );
typedef QString  	( *_MergePara )  ( int, QVariant);

class AREA_ANIMATE;
typedef bool     	( *_ParseAnimatePara )  ( QString, QString, AREA_ANIMATE* );

#define D_INVALID_NUM       (0x7f)

#define D_PREFIX              QString("Prefix")
#define D_INITVALUE           QString("Init")
#define D_DEFAULT_PREFIX      QString("RC")

#define D_MAP                 QString("Map")

/****************************************************************************
    �����ַ���
****************************************************************************/

QRect GetRectFromString(QString pRectString);
QString GetStringFromRect(QRect pRect);

QRectF GetRotateRect(QRectF pRect, ROTATE_ANGLE pAngle);
CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle);

/****************************************************************************
  �ṹ��
****************************************************************************/

inline QTimeLine::CurveShape GetTimelineType(QString pContent)
{
    if(pContent.compare(QString("EaseIn"), Qt::CaseInsensitive) == 0)
        return QTimeLine::EaseInCurve;
    else if(pContent.compare(QString("EaseOut"), Qt::CaseInsensitive) == 0)
        return QTimeLine::EaseOutCurve;
    else if(pContent.compare(QString("EaseInOut"), Qt::CaseInsensitive) == 0)
        return QTimeLine::EaseInOutCurve;
    else if(pContent.compare(QString("Linear"), Qt::CaseInsensitive) == 0)
        return QTimeLine::LinearCurve;
    else if(pContent.compare(QString("Sine"), Qt::CaseInsensitive) == 0)
        return QTimeLine::SineCurve;
    else if(pContent.compare(QString("Cosine"), Qt::CaseInsensitive) == 0)
        return QTimeLine::CosineCurve;
    return QTimeLine::EaseInOutCurve;
}

#define D_ENUMSPLIT     QString(",")
#define D_RANGESPLIT    QString("~")
QStringList PreprocPara(QString &pContent);

class UiComParser;

/*
����XX:1@xx,xx��ʽ������
QVariant
    0��int���ͣ����
    1��QList<QVariant>���ͣ���ѡֵ
*/
class UiParaParser
{
public:
    UiParaParser(UiComParser *pComParser);
    UiParaParser(QString pString, UiComParser *pComParser);
    ~UiParaParser();

    void SetString(QString pString);

    //>@ XX:100;XX:1@xx,xx;
    QVariant GetPara(int pName);
    void SetPara(int pName, QVariant pValue);

    QString GetString(_MergePara pMergeParaFun);

public:
    QHash<int, QVariant>        mParaHash;  //>@����ĳ����Դ�Ĳ�������
    UiComParser                *mComParser;
};

//<Layout0 H="258,10,198,198" V="37,222,198,198"/>
class UiRectParser : public UiParaParser
{
public:
    UiRectParser(QDomElement pElement, UiComParser *pComParser);

    QRect GetRect(DIRECTION pDirection);
    void SetRect(DIRECTION pDirection, QRect pRect);

public:
    QDomElement                 mElement;
    bool                        mHVSame;        //>@��������Դ�Ƿ���ͬ�����Ϊtrue����ֻ��Ҫȡm_Rect��0������
    QRect                       mRect[2];      //>@0:Vertical, 1:Horitical
};

/*
    <Rect>
        <Layout0 H="258,10,198,198" V="37,222,198,198"/>
        <Layout1 H="258,10,198,198" V="37,222,198,198"/>
        <Layout2 H="258,10,198,198" V="37,222,198,198"/>
        <Layout3 H="258,10,198,198" V="37,222,198,198"/>
    </Rect>
*/
class UiRectListParser : public UiParaParser
{
public:
    UiRectListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRectListParser();

    UiRectParser* GetRect(int pLayout);
    QRect GetRect(int pLayout, DIRECTION pDirection);
    bool SetRect(int pLayout, DIRECTION pDirection, QRect pRect);

    UiRectParser* AddRect(int pLayout, XmlParser* pParser);
    void DelRect(int pLayout);

public:
    QDomElement                 mElement;
    QHash<int, UiRectParser*>   mRects;
};

typedef union{
    void          *mVAR;
    QImage        *mIMG;
    QMovie        *mGIF;
#ifdef  SUPPORT_SVG
    QSvgRenderer  *mSVG;
#endif
#ifdef  SUPPORT_QUI
    iWidget       *mQUI;
#endif
} UD_RCBUFFER;       //>@ͼ��ָ��

typedef struct{
    int            mKey;        //>@ӳ��UI�������Դ��
    AREA_OPERATE   mOperate;    //>@ӳ��UI��������ղ���
} UD_MAPITEM;

//<RC0 Para="Cache:true;Absolute:false" H="Desktop/Resource/RC0.png" Map="Arrow:1@Run;"/>
class UiRcParser: public UiParaParser
{
public:
    UiRcParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRcParser();

    void UpdateHV(bool pSame);
    void Release();

    RC_TYPE GetType();
    bool GetRc(DIRECTION pDirection, QSize pSize=QSize());
    void SetRc(QString pDir, QString pFile, DIRECTION pDirection);  //>@pFileΪ����·���µĵ�ַ

    QImage* GetImage(DIRECTION pDirection, QSize pSize=QSize());
    bool IsImage(DIRECTION pDirection);

public:
    QDomElement                     mElement;

    bool                            mHVSame;        //>@��������Դ�Ƿ���ͬ
    RC_TYPE                         mType[2];          //>@��Դ���ͣ�Ĭ��ΪPNGͼƬ
    QString                         mFile[2];          //>@��Դ�ļ�

    DIRECTION                       mCurIndex;
    UD_RCBUFFER                     mBuffer;        //>@ͼ��ָ��

    //>@ӳ�䣬����ͬ�������ؼ�����ʾ
    QHash<UI_TYPE, UD_MAPITEM*>     mMaps;          //>@ӳ���б�
};

class UiRcListParser : public UiParaParser
{
public:
    UiRcListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRcListParser();

    UiRcParser* NextRc();
    UiRcParser* PrevRc();

    bool Contains(int pKey);
    UiRcParser* GetRc(int pKey);

    void AddRc(int pKey, UiRcParser* pRcInfo);
    void AddRc(int pKey, XmlParser* pParser);
    void DelRc(UiRcParser* pRcInfo);
    void DelRc(int pKey);

public:
    QDomElement                     mElement;

    QHash<int, UiRcParser*>         mRCs;
    QString                         mPrefix;

    UiRcParser                     *mCurValue;
    int                             mCurKey;     //>@��ǰ����ʹ�õ���Դ���
};

//<Run Type="Roll" Para="AnimateType:EaseInOut;Interval:2000;FrameRange:20;Direction:V;Orientation:P;"/>
class UiComParser;
class UiOperateParser : public QThread
{
    Q_OBJECT
public:
    UiOperateParser(EffectType pType, QDomElement pElement, UiComParser *pComParser);
    ~UiOperateParser();

    enum ParaType{
        E_TYPE,         //>@��Ч���ͣ�Ĭ��Ϊ����Ч
        E_DIRECTION,    //>@ͼƬ��Ч����
        E_ORIENTATION,  //>@��Ч������
        E_LOOP,         //>@Ϊ0��ʾ��Զѭ��
        E_FRASTART,
        E_FRAEND,
        E_TIME,
        E_DUTYRATIO,
        E_CURVETYPE
    };

    void Render(UiComParser *pRenderCom);
    void UnRender(UiComParser *pRenderCom);

    void Start();
    void Stop();
    void SetPause(bool pFlag);

    void SetPara(ParaType pType, int pPara);
    int  GetPara(ParaType pType);

    void run();

    QTimeLine::State state();

public slots:
    void slot_FrameChanged(int pFrame);
    void slot_Finished();

signals:
    void sStop();
    void sSetPaused(bool paused);
    void sFinished();

public:
    QDomElement                         mElement;
    AREA_ANIMATE                       *mAnimatePara;

    QList<UiComParser*>                 mRenderComs;

    UiComParser                        *mComParser;

    QTimeLine::State                    mRunMode;
};

class UiOperateListParser : public UiParaParser
{
public:
    UiOperateListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiOperateListParser();

    UiOperateParser* GetOperate(AREA_OPERATE pKey);
    AREA_ANIMATE* GetOperatePara(AREA_OPERATE pKey);

    void AddOperate(AREA_OPERATE pKey, UiOperateParser* pInfo);
    UiOperateParser *AddOperate(AREA_OPERATE pKey, XmlParser* pParser);
    void DelOperate(AREA_OPERATE pKey);

public:
    QDomElement                            mElement;
    QHash<AREA_OPERATE, UiOperateParser*>  mOperations;  //>@��Ч����
};

class UiParser;

class UiComParser : public UiParaParser
{
public:
    UiComParser();
    ~UiComParser();

    bool Init(QDomElement pElement, UiParser *pParser = 0);
    virtual bool InitSubUI(QDomElement pElement, UiParser *pParser = 0);

    void Clear(bool pSync = true);

    //>@----------------------------------------------------------------------
    //>@----    ��������ʵ��
    virtual bool ParsePara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Rect
    virtual bool ParseRectPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubRectPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Resource
    virtual bool ParseRcPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubRcPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Map
    virtual bool ParseMapPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubMapPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Map
    virtual bool ParseOperationPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubOperationPara(QString pLabel, QString pContent, AREA_ANIMATE* pAreaEffect);

    virtual bool PaintBuffer(UiOperateParser* pAnimate, int pFrame) = 0;

public:
    UiParser                               *mUiParser;
    QDomElement                             mElement;
    bool                                    mExternPara;  //>@ָʾ��Դ�Ƿ�Ϊ�ⲿ����ģ�����ǣ�����ע��ʱ����ɾ����ֻ���

    int                                     mLayout;      //
    int                                     mStyle;

    UiRectListParser                       *mComRect;
    UiRcListParser                         *mComResource;      //>@ֻ��Ҫ��¼·������Դ
    UiOperateListParser                    *mComOperation;

    QList<QImage*>                          mCurRcs;
    QImage                                  mImgBuffer;  //һ������
    E_WORK_STATE                            mBufState;
};

#define D_TYPE_USER_DEFINE      100

/*
<Desktop Para="" Layout="1" Style="2">
    <Rect>
        <Layout0 H="258,10,198,198" V="37,222,198,198"/>
        <Layout1 H="258,10,198,198" V="37,222,198,198"/>
        <Layout2 H="258,10,198,198" V="37,222,198,198"/>
        <Layout3 H="258,10,198,198" V="37,222,198,198"/>
    </Rect>
    <Resource Prefix="RC" Init="1">
        <RC0 Para="Cache:true;Absolute:false" H="Desktop/Resource/RC0.png"/>
        <RC1 Para="StyleSheet:rc.qss" H="Desktop/Resource/RC0.ui"/>
        <RC2 HV="Desktop/Resource/RC0.qml"/>
        <RC3 Para="Repeat:true;SoloTrack:true;Volume:80;" HV="Desktop/Resource/RC0.wav"/>
        <RC4 H="Desktop/Resource/RC0.mp4"/>
    </Resource>
    <Operation>
        <HAState Type="Invalid" Para=""/>
        <Run Type="Roll" Para="AnimateType:EaseInOut;Interval:2000;FrameRange:20;Direction:V;Orientation:P;"/>
        <Halt Type="Invalid"/>
    </Operation>
    <SubUI>
        <Desktop Para="">
            <Rect>
                <Layout0 H="258,10,198,198" V="37,222,198,198"/>
                <Layout1 H="258,10,198,198" V="37,222,198,198"/>
                <Layout2 H="258,10,198,198" V="37,222,198,198"/>
                <Layout3 H="258,10,198,198" V="37,222,198,198"/>
            </Rect>
        </Desktop>
    </SubUI>
</Desktop>
*/

class UiParser : public XmlParser
{
    Q_OBJECT
public:
    UiParser(QString fileName);

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

    int GetLayout();
    bool SetLayout(int pLayout);

    int GetStyle();
    bool SetStyle(int pStyle);

    //>@�����ļ��е�Ԫ��
    void GetUiGroup();

public:
    //>@xx.ui�ļ��е������subui��control����DomԪ��(xx.ui)
    QHash<UI_TYPE, QDomElement>  m_ComElementGroup;

    int                          mLayout;
    int                          mStyle;
};

class QUiParser : public XmlParser
{
    Q_OBJECT
public:
    QUiParser(QString pFileName);
    void GetUiGroup(QDomElement pElement);

    QRect GetRect(UI_TYPE pType);
    void ModifyRect(UI_TYPE pType, QRect pRect);

public:
    QHash<UI_TYPE, QDomElement>  m_ComElementGroup;
};

#endif // UIPARSER_H

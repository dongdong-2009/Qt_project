#ifndef UIFUN_H
#define UIFUN_H

#include "bggen.h"
#include "bstui.h"
#ifndef DISPLAY
#include "picflow.h"
#endif
#include <QTime>
#include <QVector>
#include <QDir>
#include <QKeyEvent>
#ifdef SUPPORT_WIDGET
#include <QUiLoader>
#endif

#ifdef  SURRPORT_3D
#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtOpenGL/qglshaderprogram.h>
#include "./OpenGL/Windows/include/glut.h"
#endif

#ifdef SUPPORT_SCENE
class GraphicsScene;
#elif SUPPORT_WIDGET
class UiWidget;
#elif SUPPORT_COMEDIT
class GraphicsScene;
#endif

#ifdef  SURRPORT_3D
class GraphicsGLWidget;
#endif

#define D_SETCTRL_ZVALUE  10000 //>@���ÿؼ�Z��
#define D_MSGCTRL_ZVALUE  10001 //>@��Ϣ�ؼ�Z��
#define D_PRGCTRL_ZVALUE  10002 //>@�������ؼ�Z��
#define D_KEYBOARD_ZVALUE  10003//>@���̿ؼ�Z��
#define D_TEXT_ZVALUE     9000//>@�ı��ؼ�Z��

typedef enum{
    SM_NORMAL = 0,
    SM_PERMANENT,
    SM_TEMPORARY,
    SM_HOLD
}E_SHOWMODE;

inline bool operator==(QPixmap b1, QPixmap b2) { return b1.data_ptr() == b2.data_ptr(); }

bool PreprocContent(QString &pContent);

class PIX_INFO
{
public:
    PIX_INFO()
    {
        mGIF = 0;
    #ifdef  SURRPORT_SVG
        mSVG = 0;
    #endif
    }
    void clear(RC_TYPE pType)
    {
        if(pType == RC_PNG)
        {
            mPNG = QPixmap();
        }
        else if(pType == RC_GIF)
        {
            if(mGIF)
            {
                mGIF->deleteLater();
                mGIF = 0;
            }
        }
#ifdef  SURRPORT_SVG
        else if(pType == RC_SVG)
        {
            if(mSVG)
            {
                mSVG->deleteLater();
                mSVG = 0;
            }
        }
#endif
#ifdef  SURRPORT_3D
        else if(pType == RC_3DS)
            m3DS = -1;
#endif
    }
public:
    QPixmap         mPNG;
    QMovie         *mGIF;
#ifdef  SURRPORT_SVG
    QSvgRenderer   *mSVG;
#endif
#ifdef  SURRPORT_3D
    GLuint          m3DS;
#endif
};

class RC_INFO
{
public:
    RC_INFO() {mRcType = RC_INVALID; clear();}
    ~RC_INFO() {clear();}
    void clear();
public:
    QDomElement             mElement;
    QString                 mRcFile;    //>@��Դ�ļ�·��
    RC_TYPE                 mRcType;    //>@��Դ����
    PIX_INFO                mRcPix;     //>@��ԴͼƬ
    QString                 mTipText;   //>@��ʾ�ı�
    QHash<int, QVariant>    mParaHash;  //>@����ĳ����Դ�Ĳ�������
};

typedef struct _MAP_INFO{
    void clear() { mMap = -1; mParaHash.clear(); }
    int                     mMap;       //>@Ŀ��ӳ��
    QHash<int, QVariant> mParaHash;  //>@����ĳ����Դ�Ĳ�������
} MAP_INFO;

typedef struct _FORM_INFO{
    void clear() { mParaHash.clear(); }
    QString              mFile;       //>@
    QString              mStyleSheet;
    QHash<int, QVariant> mParaHash;  //>@����ĳ����Դ�Ĳ�������
} FORM_INFO;

typedef struct _RECT_INFO{
    QRectF m_Rect[2];  //>@0:Vertical
} RECT_INFO ;

//>@pRect��ʾ���ǵ���ͨ��Element���г�ʼ��������ߵ�ǰui�ļ���û�д˿ؼ�ʱ��ͨ����rect���г�ʼ��ui�еĽڵ��Լ��������λ�á�
class RcContainer
{
public:
    RcContainer(
#ifdef SUPPORT_SCENE
                GraphicsScene *pContainer=0,
#elif SUPPORT_WIDGET
                UiWidget *pContainer=0,
#elif SUPPORT_COMEDIT
                GraphicsScene *pContainer=0,
#endif
                QRectF pRect=QRectF());
    ~RcContainer();

    void SetUiContainer(
    #ifdef SUPPORT_SCENE
                            GraphicsScene *pContainer
    #elif SUPPORT_WIDGET
                            UiWidget *pContainer
    #elif SUPPORT_COMEDIT
                            GraphicsScene *pContainer
    #endif
            );
    virtual bool ModifyGeomotery(QStringList pGeoList) {return false;}
    virtual void SetComGeometory(QRectF pRect) {}

    virtual bool ModifyMap(QStringList pParaList) {return false;}

    virtual QString ExecCommand(QString pCommand) {return QString();}

    QString GetAttribute(QString pEffect, QString pName);
    bool SetAttribute(QString pEffect, QString pName, QString pValue);

    bool SetPara(QDomElement &pElement, xmlParse *pParse); //>@�޸�pElementΪ���Element
    bool GetPara(QDomElement &pElement, xmlParse *pParse);

    virtual bool WriteReg(quint16 pRegAddr, QByteArray pContent) {return true;}
    virtual QByteArray ReadReg(quint16 pRegAddr, quint32 pTimeout) {return QByteArray();}

    MAP_INFO* AddMap(int pSrc, int pMap);
    bool DelMap(int pSrc);
    bool DelAllMap();

    void RemoveRc(int pKey);
    FORM_INFO *LoadForm(int pKey, QDomElement pRcElement);
    RC_INFO *LoadRc(int pKey, QDomElement pRcElement);
    RC_INFO *LoadPath(int pKey, QString pPixmapName);
    virtual MAP_INFO* LoadMap(QDomElement pElement);

    RC_INFO *LoadPixmap(int pKey, QPixmap pPixmap);
    QPixmap LoadPixmap(QString pPixmapName, QSizeF pSize=QSizeF());
    QPixmap ScalePixmap(QPixmap pPixmap, QSizeF pSize=QSizeF());

    RC_INFO *CreateRcInfo(QPixmap pPixmap);

    QString GetPixmapPath(int pKey);
    QMovie* GetGif(RC_INFO *pRcInfo, QSizeF pSize=QSizeF());
    QPixmap GetPixmap(RC_INFO *pRcInfo, QSizeF pSize=QSizeF());
    QPixmap GetPixmap(int pKey, QSizeF pSize=QSizeF());  //>@��m_EffectXXX�м���ͼƬ

    bool hasResource();

    virtual void Update() {}

    int getNextRc(int pCurRcIndex);
    int getPrevRc(int pCurRcIndex);
    QPixmap getNextImg();
    QPixmap getPrevImg();

    void ReleaseEffectGroup();

    void ReleaseRC();
    bool RefreshRC();

    void InitScene(
#ifdef SUPPORT_SCENE
            GraphicsScene* pScene
#elif SUPPORT_WIDGET
            UiWidget* pScene
#elif SUPPORT_COMEDIT
            GraphicsScene* pScene
#endif
            );

    void DeleteComponent();  //>@��UI�ļ���ɾ���˽ڵ������
    bool InitComponent(QHash<int, RC_INFO*> pRcList, QHash<AREA_OPERATE, AREA_ANIMATE*>  pEffectGroup, QRectF pRect=QRectF());
    bool InitComponent(QDomElement &pElement, bool pOffset = false);
    bool InitState(QDomElement pElement);
    virtual bool InitEffectState(EffectType pStateType, AREA_OPERATE pOperate, QDomElement pElement);

    //>@ֱ�Ӵ��ļ�������ȡָ��ǰ׺���ļ���Ϊ��Դ�ļ�
    virtual void InitRC(QString pRcPath, QString pPrefix = D_DEFAULT_PREFIX, bool pInititial = true);

    bool InitSubPara(QString pStateName, QString pLabel, QString pContent);

    bool InitRcPara(AREA_STYLE* pAreaStyle, QString pCommonPara);
    virtual bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent){return false;}

    bool InitMapPara(MAP_STYLE* pStyle, QString pPara);
    virtual bool InitSubMapPara(MAP_STYLE* pStyle, QString pLabel, QString pContent){return false;}

    bool InitEffectPara(AREA_ANIMATE* pAreaEffect, QString pEffectPara);
    virtual bool InitSubEffectPara(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);

    //>@ת���ַ���Ϊ�������֣�����ParserParameter�����С�
    virtual int StrToEnum(QString pString) {return -1;}
    //>@��������ת���ַ���ΪQVariant����
    virtual QVariant StrToValue(int pEnum, QString pString) {return QVariant();}

    virtual void ParserParameter(FORM_INFO *pInfo, QString pParameter);
    virtual void ParserParameter(MAP_INFO *pInfo, QString pParameter);
    virtual void ParserParameter(RC_INFO *pInfo, QString pParameter);

    virtual void Restart() {}
    virtual bool Start() {return true;}  //>@ִ����InitEffect��Ҫִ��Start��������Ч

    QList<EFFECT*> GetDefaultEffects(QString pStateName);
    EFFECT* GetDefaultEffect(QList<EFFECT*> pEffects, QString pEffectName);
    EFFECT* GetDefaultEffect(QString pStateName, QString pEffectName);
    QDomElement GetComStateElement(QString pStateName, QString &pEffectName, QString &pEffectPara);
    QString GetDefaultEffectPara(QString pStateName, QString pEffectName);

    QString GetComStateEffect(QString pStateName);
    QString GetComStateEffectPara(QString pStateName, QString pParaName);
    QString GetComStateRcFile(QString pStateName, QString pRcName);

    //>@���ĳ������������·���µ���Դ�ļ���
    QString GetRelativeRcLocation();

    virtual QRectF  GetCurRect();

public:
#ifdef IDE
    ComCategory                        *m_ComCategory;
#endif
#ifdef SUPPORT_SCENE
    QPointer<GraphicsScene>             m_UiContainer;
#elif SUPPORT_WIDGET
    QPointer<UiWidget>                  m_UiContainer;
#elif SUPPORT_COMEDIT
    QPointer<GraphicsScene>             m_UiContainer;
#endif
    QDomElement                         m_ComElement;
    COM_TYPE                            m_ComType;
    QString                             m_ComPath;

    QRectF                              m_ComRect[3];  //>@��ǰʹ�õĴ�С����

    QString                             m_RcPrefix;

    //>---------------------------------------------------------------------------------
    bool                                m_ExternRC;  //>@ָʾ��Դ�Ƿ�Ϊ�ⲿ����ģ�����ǣ�����ע��ʱ����ɾ��
    //>@Form
    QHash<int, FORM_INFO*>              m_FormList;     //>@
    //>@RC
    QList<int>                          m_RcList;       //>@���ڲ�����һ������һ����Դ
    QHash<int, RC_INFO*>                m_EffectRC;     //>@ֻ��Ҫ��¼·������Դ
    AREA_STYLE*                         m_RcStyle;  //>@��Դ����
    int                                 m_RcIndex;     //>@��ǰ����ʹ�õ���Դ���
    PIXMAP_POINTER                      m_PixmapPointer;  //>@��ǰ����ʹ�õ�ͼƬ��Դ
    //>@MAP
    QHash<int, MAP_INFO*>               m_MapList;     //>@
    MAP_STYLE*                          m_MapStyle;  //>@ӳ������
    //>@EFFECT
    QHash<AREA_OPERATE, AREA_ANIMATE*>  m_EffectGroup;  //>@��Ч����
    bool                                m_EffectValid;  //>@��Чʹ��
};

#endif // LOGFUN_H

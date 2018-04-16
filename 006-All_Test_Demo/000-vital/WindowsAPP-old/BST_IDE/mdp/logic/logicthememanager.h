#ifndef LOGICTHEMEMANAGER_H
#define LOGICTHEMEMANAGER_H

#include <QGraphicsView>
#include "logicbase.h"

#ifdef DISPLAY
#include "eventmap.h"
#ifdef SUPPORT_SCENE
#include "uiscene.h"
class UiView;
class UiScene;
#elif SUPPORT_WIDGET
#include "uiwidget.h"
#elif SUPPORT_PLUGIN
#include "uiplugin.h"
#endif
#endif
#include "physicalplayer.h"

class LogicThemeManager : public LogicDevice
{
    Q_OBJECT
public:
    LogicThemeManager(QObject *parent = 0);
    ~LogicThemeManager();

    quint32 InitPara(); //>@���ش���Ĳ�������
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    void InitTheme(UiView* pView, uiManager *pManager);
#endif
#endif
    QStringList SearchThemes(QString pDirPath);
    void UpdateThemeList();

    bool hasTheme();
    bool SetValue(QString pValue);

    //>@���������ļ���·����������Լ�����������
    QString GetInformation();

    bool SetAutoTest(bool pFlag);
    bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);
    void ReleaseImageBuffer() {} //>@�ͷ������е�ͼƬ����

public slots:
    void slot_ReportOperate(AREA_OPERATE pOperate, QVariant pPara);
    void UpdatePara(QObject* pObject);
    void DeviceStateChanged();

    void slot_Initialize();

    void slot_OperateTheme(AREA_OPERATE pOperate, QVariant pPara);

    void InitTheme();
    void ReleaseTheme();
    void ClearTheme();  //>@�������ĸ����������ʾ
    void ShowTheme();
    void HideTheme();
    void RotateTheme(int pNewRotate);

    bool ModifyCurTheme(QString pThemeName) {return true;}

    void slot_TestTimerTimeout();

public:
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    UiScene            *m_UiContainer;
    QPointer<GraphicsMedia>  m_Media;
#elif SUPPORT_WIDGET
    UiWidget           *m_UiContainer;
    QPointer<WidgetMedia>    m_Media;
#elif SUPPORT_PLUGIN
    UiPlugin           *m_UiContainer;
#endif
    EventMap           *m_EventMap;
#endif

    bool                mAutoTestEnable;
    int                 mThemeLayout[2];

    //>@bk,���ڴ��౸�ݲ�������initparaʱ��Ҫ��ʼ���������罨������ʱ��Ҫ���浱ǰֵ���ڶϿ�����ʱ�ָ���ֵ
    int                 mThemeLayoutBk;

    QPointer<PhysicalPlayer>       mPlayer;

    RANKS               mLocation;
    QString             mRender;
    QString             mThemesPath;        //>@��������ڵ�·��
    QStringList         mThemeNameList;     //>@��������ڵ�·���µ����������ļ�����
    QString             mActiveTheme;       //>@��ǰ������ļ�����

#ifdef N329
    QPixmap             m_TransparentPix;
#endif
};

#endif // PARSERTHEMEOP_H

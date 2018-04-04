#ifndef UISCENE_H
#define UISCENE_H

#include "eventmap/eventmap.h"
#include "graphicsinc.h"
#include "graphicsscene.h"
#ifdef SUPPORT_MENU
#include "settab.h"
class SetTabBar;
#endif

class UiScene;
class LogicThemeManager;
class PhysicalLCD;

class UiView : public QGraphicsView
{
    Q_OBJECT
public:
    UiView(QWidget *parent = 0);
    ~UiView();

signals:
    void sSetMenu();
};

class UiScene : public GraphicsScene
{
    Q_OBJECT
public:
    UiScene(UiView *pView = 0, QObject *parent=0);
    ~UiScene();

    bool Initialize(QString pFileName);
    bool Initialize(uiManager *pManager);
    bool LoadUI(QString pFileName);
    bool ReleaseUI();

    //>@���溯��
    bool InitUI();
    bool OperateUI(AREA_OPERATE pOperate, QList<COM_TYPE> pExceptNodes = QList<COM_TYPE>()); //pExceptNodes������Ľڵ�

    QSizeF GetSceneSize();

    void SetGeometory(QRectF pRect);
    QRectF GetGeometory();

    bool AddComponent(COM_TYPE pComType, QDomElement &pComElement, QPointF pScenePos=QPointF());
    bool RemoveComponent(COM_TYPE pComType);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    PhysicalLCD *GetLCD();
    bool Rotate(int pNewRotate);

signals:
    void sInfo(QString pMsg, int pDelay, E_DEVMODE pMode);
    void sWarn(QString pContent, int pDelay, E_DEVMODE pMode);
    void sError(QString pContent, int pDelay, E_DEVMODE pMode);
    void sProgress(quint32 pValue);
    void sProgress(quint32 pPrg, quint32 pExtPrg);
    void sProgress(QString pMsg, quint32 pValue);  //>@���͸�DeviceManager�ġ�

    void sBeep(QString);  //>@���Ͳ���beep�ź�
    void sIcon(QString);  //>@������ʾͼ���ź�

public slots:
    void slot_SetMenu();
    void slot_Progress(QString pMsg, quint32 pValue);  //>@����DeviceManager���źţ�ת����Message��Progress
    void slot_Progress(QString pMsg, quint32 pPrg, quint32 pExtPrg);

public:
    QRectF              m_ViewGeometery;

    QPointer<UiView>    m_MainForm;
    LogicThemeManager  *m_LogicThemeManager;

    //>@XML������
    iniParse            *m_IniParse;
    //>@Ӧ��Э����
    //>@�ؼ�
    GraphicsMessage     *m_MessageBar;
    GraphicsProgress    *m_ProgressBar;
    GraphicsKeyboard    *m_Keyboard;
    GraphicsDesktop     *m_Desktop;

#ifdef SUPPORT_KEY
    SetTabBar           *m_UiSetMenu;
#endif
};

#endif // UISCENE_H

#ifndef GraphicsSETBAR_H
#define GraphicsSETBAR_H

#include "graphicsoperate.h"
#include "groupbox.h"

typedef struct{
    QRect           mLocation;
    qreal           mTransparent;
    qreal           mZValue;
} ITEM_PARA;

typedef enum {
    ITEM_PERIPHERAL=0,
    ITEM_APPLICATION,
    ITEM_MONITOR,
    ITEM_LOGVIEWER,
    ITEM_UNKNOW
} ITEM_TYPE;

class SetBarItem : public QScrollArea
{
    Q_OBJECT
public:
    explicit SetBarItem(ITEM_TYPE pType, QPixmap pPixmap, QRect rect, QWidget *parent=0);
    ~SetBarItem();

    void ShowUI();
    void HideUI();

protected:
    void resizeEvent(QResizeEvent *e);

public:
    enum ITEM_STAGE{ BEGIN=0, END=1 };

    QPixmap                 m_BgPixmap;
    QWidget*                m_ParaUI;
    ITEM_STAGE              m_ItemStage;

    ITEM_TYPE               mItemType;
    QGraphicsProxyWidget   *mItemProxy;
    SetBarItem             *mItemWidget;
    int                     mCurParaIndex;     //>@�������ITEM���
    int                     mNextParaIndex;    //>@�������ITEM���
};

class DeviceManager;

class GraphicsSetBar : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsSetBar(QGraphicsItem *parent);
    bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    bool Start();
    virtual bool PaintEffect(QPainter *p);

    bool GetValidRect(QRect &pRect);

#ifdef DISPLAY
    void SyncDevManager(DeviceManager *pDevManager) { m_DevManager = pDevManager; }
#endif
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    bool updateEffect(int pIndex);    //>@���õ�ǰλ����ʾ���ΪpIndex��ITEM
    bool updateEffect(bool pFlag);
    bool updateEffect(MOVE_DIRECTION pDirection);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

    void AddItem(int pIndex, ITEM_TYPE pType, QPixmap pPixmap);
    void ReleaseItems();
    bool InitItems();
    bool UpdateItems(int pIndex);

    int GetCurItemIndex();
    SetBarItem* GetCurSetItem();
    QWidget* GetFocusSubctrl();
    bool NeedReboot();


public slots:
    virtual void ExecAction(QAction *pAction) {}
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
    void Operate0();    //>@��תΪ����ģʽ
    void Operate1();    //>@���л���
    void Operate2();    //>@��תΪ����ģʽ
    void OperateItem(int pFrameNum);

    void Initialize();
    void Release();
    bool GotoPage();
    bool LeavePage();
    bool GotoModify();
    bool LeaveModify();
    bool MoveLeft();
    bool MoveRight();
    bool MoveFocusIn();
    bool MoveFocusOut();
    bool MoveFocusUp();
    bool MoveFocusDown();
    bool MoveFocusLoop();
    bool ModifyValueUp();
    bool ModifyValueDown();

public:
#ifdef DISPLAY
    DeviceManager          *m_DevManager;
#endif
    QRect                   m_ValidRect;
    SetBarItem*             m_CurItemInfo;
    QList<SetBarItem*>      m_ItemList;
    OPERATE_INFO            m_SlipOperateInfo;
    int                     m_ItemCount;   //>@��ЧITEM�ĸ���
    int                     m_ItemSlipCount;  //>@��ǰ��Ҫ�ƶ����ٸ�Itemλ��

    SB_PATTERN              m_Pattern;  //>@����ҳ�����ʽ

    qreal                   m_Diff;             //>@ITEM��ִ����Ч��ֵ��
    qreal                   m_CurValue;         //>@ITEM��ǰ����״ֵ̬�����ΪRotate��Ч����Ϊ��ǰ��ת�ĽǶ�
};

class DesignSetBar : public GraphicsSetBar
{
    Q_OBJECT
public:
    DesignSetBar(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsSetBar::PaintEffect(p);
        //>@���Ʊ߿�
        QRectF tmpRect = geometry();
        qreal Width = tmpRect.width();
        qreal Height = tmpRect.height();
        p->translate(Width / 2, Height / 2);
        p->setPen(Qt::DashDotDotLine);
        p->setPen(QColor(10,10,255));
        p->drawRect(QRect(-Width/2, -Height/2, Width, Height));
        p->drawRect(QRect(-Width/2+1, -Height/2+1, Width-2, Height-2));
        return true;
    }

public slots:
    void ExecAction(QAction *pAction);
};

#endif // EFFECTSETBAR_H

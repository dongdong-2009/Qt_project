#ifndef GraphicsTEXT_H
#define GraphicsTEXT_H

#include "graphicsoperate.h"
class GraphicsFunction;

class PhysicalLCD;
class PhysicalPlayer;
class PhysicalCOM;

class GraphicsText : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsText(QGraphicsItem *parent);
    ~GraphicsText();
    bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    QRectF GetCurRect();
    bool Start();
    void updateEffect(int pState, AREA_OPERATE pOperate);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

    void OperateClear(QVariant pPara);

    qreal GetMinSrnLen();

    bool CanUpdateText();  //>@�Ƿ����ˢ��Text���������ǰ�Ƿ�ΪС��ģʽ�µĴ��ı���function�Ͳ���ˢ��Text����

    void UpdateBigRect();
    int GetTextShowMode();
    bool SetTextShowMode(int pMode, bool pTest = false);
    bool ShowTipText(bool pEnable, QString pText);
    QString ExecCommand(QString pCommand);

    void InitGrid();
    bool SetGrid(QRectF pRect, int pRow, int pColumn);

    bool PaintEffect(QPainter *p);
    void OperateTurn(QVariant pPara);

    void ModifyGeomotery(QRectF pRect);
    void SetComGeometory(QRectF pRect);

signals:
    void sSetGeomotery(QRectF);

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void slot_ChangeText();

public:
    int                 m_TextState;

    bool                m_TestMode;   //>@��m_TestModeΪtrueʱ����Ҫȡm_ScreenModeBk��ֵ
    int                 m_ScreenModeBk;
    int                 m_ScreenMode; //>@0:С��ģʽ��1������ģʽ
    //>@������ı�ģʽ������õ��������ݡ�
    int                 m_Alignment;

    quint32             m_MaxRow;
    quint32             m_MaxColumn;

    quint32             m_Row;
    quint32             m_Column;

    quint32             m_SpaceExt;  //>@�ո�����ָһ���ո���չΪ2���ո�
    quint32             m_RowSpace;
    QFont               m_TextFont;
    QColor              m_TextColor;
    QRectF              m_OldRect;
    QRectF              m_BigRect;

    int                 m_AlignS, m_AlignB;

    int                 m_TextMode;
    TEXT_INFO           m_TextInfo[D_TEXT_MODE][D_TEXT_NUM];   //>@����ģʽ��ͬ��ȷ��

    QPointer<PhysicalLCD>       m_LCD;
    QPointer<GraphicsFunction>  m_Function;
    QPointer<PhysicalPlayer>    m_Player;
    QPointer<PhysicalCOM>       m_Com;

    QTimer              mTimer;
};

class DesignText : public GraphicsText
{
    Q_OBJECT
public:
    DesignText(QGraphicsItem *parent);
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsText::PaintEffect(p);
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

#endif // EFFECTTEXT_H

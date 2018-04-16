#ifndef GraphicsCLOCK_H
#define GraphicsCLOCK_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComClock  DesignClock
#elif DISPLAY
    #define ComClock  GraphicsClock
#endif

class GraphicsClock : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsClock(QGraphicsItem *parent);
    ~GraphicsClock() { ReleaseElementInfos(); }
    bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    bool Start();
    virtual bool PaintEffect(QPainter *p);
    void updateEffect(CLOCK_INFO pTime);

    CLOCK_PATTERN GetClockPattern(QString pPattern);
    CLOCK_TYPE GetClockType(QString pType);
    void CalcElementInfo(CLOCK_TYPE pType);
    void ReleaseElementInfos();
    void CalcAnalogClockPointer();

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

public:
    CLOCK_PATTERN            m_Parttern;
    CLOCK_TYPE               m_Type;


    //>@Digital : ͼԪ(ָ��ʾ���ֵ�һ��������)����Ϊ19����RectYear0~RectSecond1 ---- 0~9 SUN~WES : -
    //>@          ͼƬ��Դ����Ϊ10+7+1+1 = 19
    //>@          0~9 Sun~Sat : -
    //>@Analog : ͼԪ����Ϊ11+3����RectYear0~RectWeek  ʱָ�� ��ָ�� ��ָ��
    //>@         ͼƬ��Դ����Ϊ10+7+1+4+1 = 23(1������������ĵ�)��
    //>@         0~9 Sun~Sat - ʱָ�� ��ָ�� ��ָ�� ʱ�ӵ�ͼ
    QHash<CLOCKE_ELEMENT, CLOCK_ELEMENTINFO*>    m_ClockElement;
    qreal                    m_Diff;                 //>@ͼƬһ���ƶ�(��ת)����
    CLOCK_INFO               m_SavedTime;
    qreal                    m_ItemWidth, m_ItemHeight;
    int                      m_FontSize;

    //>@���Analogʱ��
    qreal       m_PointerLength[3];
    qreal       m_PointerAngle[3][2];   //>@����ָ�룬����ָ���ȺͽǶ�(����ת�ĽǶȣ�Ҳ���ǶȲ�)

    //>@ͼ�񻺴棬������ʱ��
    QPixmap     m_BufferPixmap[3];
    int         m_MinSizeLen;
};

class DesignClock : public GraphicsClock
{
    Q_OBJECT
public:
    DesignClock(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName);
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsClock::PaintEffect(p);
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

#endif // EFFECTCLOCK_H

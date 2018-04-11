#ifndef GRAPHICSANIMAL_H
#define GRAPHICSANIMAL_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComAnimal  DesignAnimal
#elif DISPLAY
    #define ComAnimal  GraphicsAnimal
#endif

//>@Ӧ����װ�ε�С���һ������һ�����������������̣�������ѧϰ�����ˡ����˵�
//>@�Ƕ�0���������࣬������ԴͼƬʱ�ر�ע��
class GraphicsAnimal : public GraphicsOperate
{
    Q_OBJECT
public:
    explicit GraphicsAnimal(QGraphicsItem *parent);
    bool Start();
    virtual bool PaintEffect(QPainter *p);
    QLineF GetTrackLine(QPointF pPoint);
    qreal  GetTrackDiffAngle(QPointF pPoint);
    void PaintAnimal_Turn(int pFrame);
    void PaintAnimal_Move(int pFrame);
    void updateEffect(AnimalStage pStage, AREA_OPERATE pOperate);
    void updateEffect(QPointF pDstPos, AREA_OPERATE pOperate);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void Update();
    void UpdateCom(int pFrame);
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

public:
    AnimalType      m_AnimalType;

    AnimalStage     m_AnimalStage;
    QLineF          m_CurTrackLine, m_NextTrackLine;
    qreal           m_CurAngle, m_DiffAngle;        //>@�˶��ǶȲ�
    QPixmap         m_FlipPixmap;
};

inline void GraphicsAnimal::PaintAnimal_Turn(int pFrame)
{
    if(m_Animator.state() != QTimeLine::Running)
    {
        rotate(m_CurAngle + m_DiffAngle);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal t = m_DiffAngle * (qreal)pFrame / (qreal)(m_AreaAnimate->mFrameCount);
    setTransform(QTransform().translate(size().width()/2.0, size().height()/2.0)
                 .rotate(t)
                 .translate(-size().width()/2.0, -size().height()/2.0));
}

//>@��m_CurTrackLine������
inline void GraphicsAnimal::PaintAnimal_Move(int pFrame)
{
    if(m_Animator.state() != QTimeLine::Running)
    {
        setPos(m_CurTrackLine.pointAt(1));
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal t = (qreal)pFrame / (qreal)(m_AreaAnimate->mFrameCount);
    setPos(m_CurTrackLine.pointAt(t));
}

class DesignAnimal : public GraphicsAnimal
{
    Q_OBJECT
public:
    explicit DesignAnimal(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsAnimal::PaintEffect(p);
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


#endif // EFFECTANIMAL_H

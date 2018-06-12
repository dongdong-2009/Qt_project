#ifndef GraphicsFLOOR_H
#define GraphicsFLOOR_H

#include "graphicsoperate.h"
#include <QPaintDevice>

#ifdef IDE
    #define ComFloor  DesignFloor
#elif DISPLAY
    #define ComFloor  GraphicsFloor
#endif

//#define D_SUPPORT_EFFECT

class GraphicsFloor : public GraphicsOperate
{
    Q_OBJECT
public:
    typedef enum{
        PTN_F_DISCREATE = 0,    //>@ͼƬ��ɢ
        PTN_F_INTEGRATE         //>@ͼƬ����
    } FLOOR_PATTERN;

    GraphicsFloor(QGraphicsItem *parent);

    MAP_INFO* LoadMap(QDomElement pElement);
    bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    bool Start();
    void updateEffect(FLOOR_INFO pFloorInfo, AREA_OPERATE pOperate);

    bool PaintEffect(QPainter *p);

    bool PaintFloor(FLOOR_INFO pFloorInfo, QPainter *p);
    QPixmap GenerateFloorImage(FLOOR_INFO pFloorInfo);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
public:
    //>@����Ƿ�ɢͼ��ģʽ������õ��������ݡ�
    //>@����ʮ��λ������һ��ͼƬ�ϣ���¥��ͼƬ��ȷ�Ϊ3�ȷ�
    QSizeF              m_FloorNumSize;
    FLOOR_INFO          m_FloorNumInfo;
    Qt::AlignmentFlag   m_FloorAlign;
    //>@������ı�ģʽ������õ��������ݡ�
    QFont               m_FloorFont;
    QColor              m_FloorColor;
    //>@
    bool                m_IsDiscreted;   //>@�Ƿ�Ϊ����ͼƬ
};

class DesignFloor : public GraphicsFloor
{
    Q_OBJECT
public:
    DesignFloor(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName);
    QList<QAction*> GetActionList();
    bool Start()
    {
        if(GraphicsFloor::Start() == false)
            return false;
        if(m_RcStyle)
        {
            if(m_RcStyle->mRcType == COMRC_IMAGE)
            {
                if(m_IsDiscreted)
                {
                    if(!m_RcList.isEmpty())
                        m_FloorNumInfo.mHundredBits = m_FloorNumInfo.mTenBits = m_FloorNumInfo.mSingleBits = m_RcList.first();
                }
                else
                {
                    if(!m_RcList.isEmpty())
                        m_FloorNumInfo = m_RcList.first();
                }
            }
            else if(m_RcStyle->mRcType == COMRC_TEXT)
            {
                m_FloorNumInfo = "012";
            }

            updateEffect(m_FloorNumInfo, OPERATE_CHANGE);  //>@����¥��
        }
        return true;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsFloor::PaintEffect(p);
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

#endif // EFFECTFLOOR_H

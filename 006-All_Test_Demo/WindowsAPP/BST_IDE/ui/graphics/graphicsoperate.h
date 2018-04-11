#ifndef GRAPHICSOPERATE_H
#define GRAPHICSOPERATE_H

#include "graphicscomponent.h"

//>@MVC�ṹ������scene��property��frame��digramsetting
class GraphicsOperate : public GraphicsComponent
{
    Q_OBJECT
public:
    GraphicsOperate(QGraphicsItem *parent, GraphicsScene *pScene=0, QRectF pRect=QRectF());
    ~GraphicsOperate() {}

    bool RefreshParameter();   //>@�����Լ��������Լ��ɱ༭�����Χ�Լ���Ч/����

    qreal ModifyComLeft(qreal pValue, bool pSync);
    qreal ModifyComTop(qreal pValue, bool pSync);
    qreal ModifyComRight(qreal pValue, bool pSync);
    qreal ModifyComBottom(qreal pValue, bool pSync);
    qreal ModifyComWidth(qreal pValue, bool pSync);
    qreal ModifyComHeight(qreal pValue, bool pSync);
    QPointF ModifyComPos(QRectF pComRect, bool pSync);

    QRectF GetVirtualSceneRect();  //>@��ȡ�����е�scene�µ������С��Ҳ����û��ӳ�䵽��������ϵ�����

    bool ModifySceneRect(QRectF pRect);
    bool ModifyStateEffect(QString pStateName, QString pEffectName);
    bool ModifyEffectPara(QString pStateName, QString pParaName, QString pParaValue);
    bool ModifyRcFile(QString pStateName, QString pRcName, QString pRcFile);
    bool DelRcFile(QString pStateName, QString pRcName);

    //>@ʵ��Զ�����߱༭����


signals:
    //>@һ��������Property�Լ�Frame����ֻ������������Ҫͬ�������ֻҪ����Rect�źż���
    //>@���ң����ڴ�����Ҫ������ô���ͬ�������ݣ���ĳһ�����ô����޸ĺ󣬲�Ҫ�����޸Ĵ˴����е����ݣ�����ͨ��ͳһ���͵ĸı��źų����޸Ĳ�����
    void sSceneRectChanged(QRectF pRect);
    //void sStateEffectChanged(QString pStateName, QString pEffectName);
    //void sEffectParaChanged(QString pStateName, QString pParaName, QString pParaValue);
    //void sRcFileChanged(QString pStateName, QString pRcName, QString pRcFile);
    //void sRcFileDeleted(QString pStateName, QString pRcName);

public slots:

public:
    bool                        mDragValid;

    GraphicsComponent          *mParComponet;

    //>@��ʼ���ؼ����������
    QRectF                      m_CurRect;  //>@��ǰ���������µ�Scene����
    qreal   m_LeftLimitMin,   m_LeftLimitMax,     m_CurLeft;
    qreal   m_TopLimitMin,    m_TopLimitMax,      m_CurTop;
    qreal   m_RightLimitMin,  m_RightLimitMax,    m_CurRight;
    qreal   m_BottomLimitMin, m_BottomLimitMax,   m_CurBottom;

    //>@�෴���������µ�Scene����
    //>@�����෴����ֻ��Ҫ�жϿ���Լ��߶��Ƿ񳬹�������
    qreal   m_OppMaxWidth,   m_OppMaxHeight;
};


#endif // GRAPHICSOPERATE_H

#ifndef WIDGETTIME_H
#define WIDGETTIME_H

#include "widgetbase.h"

class WidgetTime : public WidgetBase
{
    Q_OBJECT
public:
    WidgetTime(QWidget *parent=0);
    ~WidgetTime();

    void InitTime(CLOCKE_ELEMENT pIndex, WidgetBase *pClockWidget);
    void InitRC(QString pRcPath);

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

public:
    //>@Digital : ͼԪ(ָ��ʾ���ֵ�һ��������)����Ϊ19����RectYear0~RectSecond1 ---- 0~9 SUN~WES : -
    //>@          ͼƬ��Դ����Ϊ10+7+1+1 = 19
    //>@          0~9 Sun~Sat : -
    //>@Analog : ͼԪ����Ϊ11+3����RectYear0~RectWeek  ʱָ�� ��ָ�� ��ָ��
    //>@         ͼƬ��Դ����Ϊ10+7+1+4+1 = 23(1������������ĵ�)��
    //>@         0~9 Sun~Sat - ʱָ�� ��ָ�� ��ָ�� ʱ�ӵ�ͼ
    QHash<CLOCKE_ELEMENT, WidgetBase*>    m_ClockWidget;
    CLOCK_INFO                            m_SavedClock;
};

#endif // WIDGETTIME_H

#ifndef WIDGETBUTTON_H
#define WIDGETBUTTON_H

#include "widgetbase.h"

class WidgetButton : public WidgetBase
{
    Q_OBJECT
public:
    WidgetButton(QWidget *parent = 0);
    ~WidgetButton();

    void InitButton(QString pDark, QString pLight);

    void SetValidRect(QRect pRect);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void ReleaseButton();

signals:
    void sPressed();
    void sReleased();

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);
    void slot_PressDelay();

public:
    int                 m_BtnType;  //>@0:��ʾ������1����ʾ�����޷�Ӧ

    bool                m_Checked;
    bool                mIsPressed;

    QList<QPixmap>      mPixmaps;
    AREA_OPERATE        mOperate;

    //>@Ϊ0��ʾ����״̬��1��ʾ����
    int                 mStatues;

    int                 mPressDelay;  //>@��ť���¶���ʱ�������������
    QTimer              mPressDelayTimer;

    QRect               mValidRect;
    bool                mValidFlag;
};


#endif // WIDGETBUTTON_H

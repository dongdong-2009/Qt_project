#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QThread>
#include <QDebug>

#define  DIAL_NUM            8
#define  GPIOBEGIN           18
#define  DEV_PATH(x)        QString("/home/libo/gpio/gpio%1/value").arg(x) // 输入输出电平值设备
#define  DIRECT_PATH(x)     QString("/home/libo/gpio/gpio%1/direction").arg(x)  // GPIO 输入输出控制设备

class DealWithReadDail: public QObject
{
    Q_OBJECT
public:
    DealWithReadDail();
    ~DealWithReadDail();
public slots:
    void readDialValue();
    void setFlag(bool run);
private:
    volatile bool runFlag;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void testReadForDefine(QString path);
    void init();
    bool openDialSwitch();
    void setDialSwitchValue();
    quint8 getDialSwitchValue();
    quint8 getBuandurate();
    void dialNumberChanged();

signals:
    void autoTestSignal();
    void dialChangedSignal();
    void daudrateChangedSignal();
    void startSignal();

private:

    QFile m_exportPath;
    int baudrate;
    QThread *wthread;
    QFile m_value[8];
    QFile m_direction[8];
    QFile f;
    DealWithReadDail dwrd;
};



#endif // WIDGET_H

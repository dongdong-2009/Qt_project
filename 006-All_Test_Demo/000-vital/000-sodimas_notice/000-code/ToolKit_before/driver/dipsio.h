#ifndef DIPSIO_H
#define DIPSIO_H

#include "define.h"
#include <QThread>

#define GPIO_VALUE(x)       QString("/sys/class/gpio/gpio%1/value").arg(x)      // ���������ƽֵ�豸
#define GPIO_EXPORT         QString("/sys/class/gpio/export")            // GPIO �豸�����豸
#define GPIO_DIRECT(x)      QString("/sys/class/gpio/gpio%1/direction").arg(x)  // GPIO ������������豸

class DipsIO : public QThread
{
    Q_OBJECT
public:
    DipsIO(QObject *parent = 0);

    void run(); //��ִ����QThread��start�����󣬻��Զ����µ��߳������д˺���

signals:
    void sValueChanged(quint8 pValue);

public:
    quint8              mDipsValue;

    QFile               mDipsExport;
    QFile               mDips8[8];
};

#endif // DIPSIO_H

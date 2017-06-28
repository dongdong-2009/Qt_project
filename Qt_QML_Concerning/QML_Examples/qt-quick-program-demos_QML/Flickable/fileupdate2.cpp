#include "fileupdate2.h"

FileUpdate2::FileUpdate2(QObject *parent) : QObject(parent)
{
    QTimer *tim = new QTimer(this);
    connect(tim, SIGNAL(timeout()), this, SLOT(ChangePercent()));
    tim->start(1000);
}

void FileUpdate2::ChangePercent()
{
    static int i = 0;
    i++;
    sendPercent(i);
    qDebug("%s, i = %d", __PRETTY_FUNCTION__, i);
    if (i == 100)
        i = 0;
}

#include "widget.h"
#include "ui_widget.h"
#include "define.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mBusyTimeout.setSingleShot(true);
    connect(&mBusyTimeout, SIGNAL(timeout()), this, SLOT(slot_CloseBusy()));
    mBusyFlagIcon = new GifPlayer(this);
    //    mgif->Play("C:\\Users\\yonghao\\Desktop\\busy.gif", true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::ShowBusy(bool pFlag, quint64 pTimeout)
{
    if(!mBusyFlagIcon)
    {
        mBusyFlagIcon = new GifPlayer(this);
    }

    mBusyTimeout.stop();
    if(pFlag)
    {
        mNetTransFlag = false;
        setDisabled(true);
#if 1
        mBusyFlagIcon->setFixedSize(100, 100);
        mBusyFlagIcon->move(QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0, size().height()/2 - mBusyFlagIcon->size().height()/2.0));
#else
#ifdef PHONE
        QDesktopWidget *desktop = qApp->desktop();
        int screen_number = desktop->primaryScreen();
        QRect rect = desktop->screenGeometry(screen_number);
        QPoint tmpPoint = rect.topLeft() + QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0, size().height()/2 - mBusyFlagIcon->size().height()/2.0);
#else
        QPoint tmpPoint = mapToGlobal(QPoint(0,0)) + QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0, size().height()/2 - mBusyFlagIcon->size().height()/2.0);
#endif
        mBusyFlagIcon->setFixedSize(100, 100);
        mBusyFlagIcon->move(tmpPoint);
#endif
        if(pTimeout > 0)
        {
            mBusyFlagIcon->Play(":/image/rc/busy.gif", true);
            mBusyTimeout.setInterval(pTimeout);
            mBusyTimeout.start();
        }
        QtSleep(5);
    }
    else
    {
        QtSleep(100);
        slot_CloseBusy();
    }
}

void Widget::slot_CloseBusy()
{
    mNetTransFlag = true;
    setEnabled(true);
    mBusyFlagIcon->Stop();
}

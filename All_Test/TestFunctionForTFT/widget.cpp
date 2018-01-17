#include "widget.h"
int preValue[DIAL_NUM] = { -1 };
int curValue[DIAL_NUM] = { -1 };
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//    testReadForDefine(DIRECT_PATH(1));
//    testReadForDefine(DIRECT_PATH(2));
    init();
    qDebug()<<__FUNCTION__<< " thread id = " << QThread::currentThreadId();
    connect(this, &Widget::startSignal, &dwrd, &DealWithReadDail::readDialValue, Qt::QueuedConnection);
    dwrd.moveToThread(wthread);
    wthread->start();
    emit startSignal();
}

Widget::~Widget()
{
    qDebug()<<__FUNCTION__<<"is call";
    dwrd.setFlag(false);
    if (NULL != wthread)
    {
        qDebug()<<__FUNCTION__<<"wthread will be delete";
        wthread->requestInterruption();
        wthread->quit();
        wthread->wait();
        delete wthread;
        wthread = NULL;
    }
    for (int i = 0; i < DIAL_NUM; ++i)
    {
        m_value[i].close();
        qDebug()<<__FUNCTION__<<"close m_value file";
    }
}

void Widget::testReadForDefine(QString path)
{
    f.setFileName(path);
    qDebug()<<"path = "<<path;
    if (f.open(QIODevice::ReadWrite))
    {
        qDebug()<<" open success !";
    }
    else
    {
        qDebug()<<" open falied !";
    }
    f.close();
}

void Widget::init()
{
//    m_exportPath.setFileName(EXPORT_PATH);
//    if (!m_exportPath.open(QIODevice::WriteOnly))
//    {
//        qDebug()<<__FUNCTION__<<" open m_exportPath is error!";
//        return;
//    }
    wthread = new QThread();
    if (NULL == wthread)
    {
        qDebug()<<__FUNCTION__<<" allocate wthread memroy is failed!";
        return;
    }
    for (int i = 0; i < DIAL_NUM; i++)
    {
//        char ch;
//        ch = i + GPIOBEGIN +'0';
//        m_exportPath.write(&ch, sizeof(ch));
        m_value[i].setFileName(DEV_PATH(i));
        if (!m_value[i].open(QIODevice::ReadOnly))
        {
            qDebug()<<__FUNCTION__<<"open "<< DEV_PATH(i) << "failed";
            return;
        }
        m_direction[i].setFileName(DIRECT_PATH(i));
        if (!m_direction[i].open(QIODevice::ReadWrite))
        {
            qDebug()<<__FUNCTION__<<"open "<< DIRECT_PATH(i) << "failed";
            return;
        }
        m_value[i].close();
        m_direction[i].close();
    }
//    m_exportPath.close();
//    for (int j = 0; j < DIAL_NUM; ++j)
//    {
//        m_value[j].close();
//        m_direction[j].close();
//    }
}

bool Widget::openDialSwitch()
{
//    IDE_DEBUG();
    return true;
}

void Widget::setDialSwitchValue()
{
//    IDE_DEBUG();
//    return true;
    char ch;
    for (int i = 0; i < DIAL_NUM; ++i)
    {
        if ( !m_value[i].open(QIODevice::ReadOnly))
        {
            qDebug()<<__FUNCTION__<<"open "<< i << "failed";
        }
        m_value[i].read(&ch, sizeof(ch));
        curValue[i] = ch - '0';
    }
}

quint8 Widget::getDialSwitchValue()
{
    quint8 tmp = 1;
    quint8 sum = 0;
    for (int i = 0; i < DIAL_NUM; i++)
    {
        sum = tmp * curValue[i] + sum;
        tmp = tmp *2;
        preValue[i] = curValue[i];
    }
    return sum;
}

quint8 Widget::getBuandurate()
{
    char ch7;
    char ch6;
    m_value[7].read(&ch7, sizeof(ch7));
    m_value[7].read(&ch6, sizeof(ch6));
    if ('0' == ch7 && '1' == ch6)
    {
        baudrate = 50;
        return 50;
    }
    else
    {
        baudrate = 250;
        return 250;
    }
}

void Widget::dialNumberChanged()
{
    setDialSwitchValue(); // 设置curValue的值
    for (int i = 0; i < DIAL_NUM; ++i)
    {
        if (preValue[i] != curValue[i])
        {
            emit dialChangedSignal();
            break;
        }
    }
    getDialSwitchValue();
}

//void Widget::readDialValue()
//{
//    qDebug()<<__FUNCTION__<< " thread id = " << QThread::currentThreadId();
//    while (true)
//    {
//        dialNumberChanged();
//        QThread::msleep(200);
//    }
//}

DealWithReadDail::DealWithReadDail()
{

}

DealWithReadDail::~DealWithReadDail()
{

}

void DealWithReadDail::readDialValue()
{
    qDebug()<<__FUNCTION__<< " thread id = " << QThread::currentThreadId();
    while (runFlag)
    {
//        dialNumberChanged();
        qDebug()<<__FUNCTION__;
        QThread::msleep(200);
    }
}

void DealWithReadDail::setFlag(bool run)
{
    qDebug()<<__FUNCTION__<< run;
    runFlag  = run;
}

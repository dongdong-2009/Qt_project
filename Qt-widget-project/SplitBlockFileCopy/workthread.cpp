#include "workthread.h"

WorkThread::WorkThread(QObject *parent) : QObject(parent)
{
    qDebug()<< __PRETTY_FUNCTION__;
}

WorkThread::~WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
}

void WorkThread::run()
{
    qDebug()<< __PRETTY_FUNCTION__;

}


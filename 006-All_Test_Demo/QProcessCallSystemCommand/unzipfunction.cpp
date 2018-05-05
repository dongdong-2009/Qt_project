#include "unzipfunction.h"
#include <QString>
#include <QDebug>

UnZipFunction::UnZipFunction(QObject *parent) : QObject(parent)
{
    connect(&m_ZipProcess, SIGNAL(finished(int)), this, SLOT(sltUnZipFinished(int)));
    connect(&m_ZipProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(sltOcurrErrors(QProcess::ProcessError)));
    connect(&m_ZipProcess, SIGNAL(started()), this, SLOT(sltStarted()));
}

void UnZipFunction::startUnZip()
{
    // unzip -q /home/libo/Desktop/Sodimas.zip -d /home/libo/Desktop/
    QString cmd = QString("unzip -q /home/libo/Desktop/Sodimas.zip -d /home/libo/Desktop/");
    m_ZipProcess.start(cmd);
    m_ZipProcess.waitForFinished();
    m_ZipProcess.close();
}

void UnZipFunction::sltStarted()
{
    qDebug()<<__PRETTY_FUNCTION__<<"cmd is executable started";
    mTimer.start();
}

void UnZipFunction::sltUnZipFinished(int code)
{
    qDebug()<<__PRETTY_FUNCTION__<<"cmd is executable finished"<<" code = "<<code;
    qint64 num = mTimer.nsecsElapsed();
    qDebug()<<__PRETTY_FUNCTION__<<"num = "<<num;
}

void UnZipFunction::sltOcurrErrors(QProcess::ProcessError error)
{
    qDebug()<<__PRETTY_FUNCTION__<<"error = "<<error;
}

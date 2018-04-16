#include "zipfun.h"

bool ZipTask::Zip(QString pSrcPath, QString pDstPath)
{
    QString tmpString = GetDirectoryName(pSrcPath);
    if(pDstPath.endsWith('/') == false)
        pDstPath.append('/');
    return Zip(pSrcPath, pDstPath + tmpString, true);
}

bool ZipTask::Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore)
{
    quint32 tmpProgress = 0;
    if(pZipFile.isEmpty())
    {
        IDE_TRACE();
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }
    pSrcPath.replace('\\', '/');
    pZipFile.replace('\\', '/');
    if(pZipFile.endsWith(".zip", Qt::CaseInsensitive) == false)
        pZipFile.append(".zip");
    QFile tmpFile(pZipFile);
    if(tmpFile.exists())
    {
        if(pCover)
        {
            IDE_TRACE();
            tmpFile.remove();
        }
        else
        {
            IDE_TRACE();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return true;
        }
    }
    //>@ȷ��Ŀ��·�����ڡ�
    QString tmpDstPath = getFileDirectory(pZipFile);
    CreatPath(tmpDstPath);

    QFileInfo tmpInfo(pSrcPath);
    if(tmpInfo.isFile())
    {
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        if(pSrcPath.endsWith(".zip", Qt::CaseInsensitive)) //>@������ѹ��zip�ļ�
        {
            IDE_TRACE();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        //>@���ļ�׼����ʼѹ��
#ifdef GCC
        QuaZip zip(pZipFile);
        if(!zip.open(QuaZip::mdCreate))
        {
            IDE_TRACE();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        QuaZipFile outFile(&zip);
        QString tmpNameOfZip = getFileName(pSrcPath);//>@ѹ�����ڵĴ��ļ����ļ���
        QFile inFile(pSrcPath);
        if(!inFile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE();
            zip.close();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(tmpNameOfZip, inFile.fileName())))
        {
            IDE_TRACE();
            inFile.close();
            zip.close();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
#endif
        //>@���Ϳ�ʼ�ź�
        emit start();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        //>@��ʼѹ��
#ifdef GCC
        quint64 count = inFile.size();
        quint64 i=0;
        while(i<count)
        {
            if(i)
            {
                emit progress(i*100/count);
                QtSleep(1);
                //QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
            i += outFile.write(inFile.read(MAXFILEFRAME));
        }
        if(outFile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            inFile.close();
            outFile.close();
            zip.close();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        outFile.close();
        if(outFile.getZipError()!=UNZ_OK)
        {
            IDE_TRACE();
            inFile.close();
            zip.close();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        inFile.close();
        zip.close();
        if(zip.getZipError()!=UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
#endif
        //>@���ͽ����ź�
        emit end();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return true;
    }
    else if(tmpInfo.isDir())
    {
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        QStringList tmpFileList = ErgodicDirectory(pSrcPath);
        if (tmpFileList.size() > 0)
        {
#ifdef GCC
            QuaZip zip(pZipFile);
            if(!zip.open(QuaZip::mdCreate))
            {
                IDE_TRACE();
                emit error();
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                return false;
            }
            QuaZipFile outFile(&zip);
#endif
            //>@���Ϳ�ʼ�ź�
            emit start();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            //>@��ʼѹ��
#ifdef GCC
            int totalf = tmpFileList.size();
            QString tmpParentSrcDir;
            if(mContainDir)
                tmpParentSrcDir = getParentDirectory(pSrcPath);//>@����pSrcDir�ĸ�·��
            else
                tmpParentSrcDir = pSrcPath;
            if(tmpParentSrcDir.endsWith("/") == false)
                tmpParentSrcDir.append("/");
            for (int i=0; i<totalf; ++i)
            {
                QString tmpSrcPath = tmpFileList.at(i);
                QString tmpString = tmpSrcPath;
                //>@ǧ��ע���Сд���������ѹ��ʱ�޷�ȥ������·��������ѹ�����ڲ������·����
                QString tmpNameOfZip = tmpString.replace(tmpParentSrcDir,"", Qt::CaseInsensitive);//>@ѹ�����ڵĴ��ļ����ļ���
                QFile inFile(tmpSrcPath);
                if(!inFile.open(QIODevice::ReadOnly))
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(tmpNameOfZip, inFile.fileName())))
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                quint64 count = inFile.size();
                quint64 j=0;
                while(j<count)
                {
                    if(j && !pIngore)
                    {
                        emit progress(j*100/count);
                        QtSleep(1);
                        //QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    }
                    j += outFile.write(inFile.read(MAXFILEFRAME));
                }
                if(outFile.getZipError() != UNZ_OK)
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                outFile.close();
                if(outFile.getZipError()!=UNZ_OK)
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                inFile.close();
                //>@���㴫��İٷֱ�
                quint32 tmpPrg = i*100/totalf;
                if(tmpProgress != tmpPrg)
                {
                    tmpProgress = tmpPrg;
                    emit progress(tmpProgress);
                    emit message(QString("Zip %1 %%2 success").arg(tmpNameOfZip)
                                                              .arg(BiteorMega(inFile.size())));
                    QtSleep(1);
                }
            }
            zip.close();
            if(zip.getZipError()!=UNZ_OK)
            {
                IDE_TRACE();
                emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
                emit error();
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                return false;
            }
#endif
            //>@���ͽ����ź�
            emit end();
            QtSleep(1);
#if (defined(UBUNTU) || defined(LINUX))
            system("sync");
#endif
            return true;
        }
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }
    emit error();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    return false;
}

bool ZipTask::Unzip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore)
{
    if(pDstPath.endsWith("/") == false)
        pDstPath.append("/");
    pDstPath.replace('\\', '/');
    pZipFile.replace('\\', '/');
    CreatPath(pDstPath);

    if(!QFile::exists(pZipFile))
    {
        IDE_TRACE();
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }

#ifdef GCC
    QuaZip zip(pZipFile);
    if(!zip.open(QuaZip::mdUnzip))
    {
        IDE_TRACE();
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }
#endif
    //>@���Ϳ�ʼ�ź�
    emit start();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
#ifdef GCC
    //>@����ѹ���ļ��е����ļ���
    int total=0;
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        total++;
    }
    int i=0;
    QuaZipFile infile(&zip);
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        QString tmpFileName = zip.getCurrentFileName();
        QString tmpString = pDstPath + tmpFileName;
        IDE_TRACE_STR(tmpString);
        QFile outfile(tmpString);/* extract to path ....... */
        if(outfile.exists())
        {
            if(pCover)
            {
                if(!outfile.setPermissions(QFile::WriteOther))
                {
                    IDE_TRACE();
                    continue;
                }
                if(!outfile.remove())
                {
                    IDE_TRACE();
                    continue;
                }
            }
            else
            {
                IDE_TRACE();
                continue;
            }
        }
        QFileInfo infofile(outfile);
        if(!CreatPath(infofile.absolutePath()))
        {
            IDE_TRACE_STR(infofile.absolutePath());
            continue;
        }
        if(!infile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE_STR(tmpFileName);
            continue;
        }
        if(!outfile.open(QIODevice::WriteOnly))
        {
            IDE_TRACE_STR(tmpString);
            infile.close();
            continue;
        }
        quint64 count = infile.size();
        quint64 j=0;
        while(j<count)
        {
            if(j && !pIngore)
            {
                emit progress(j*100/count);
                QtSleep(1);
                //QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
            j += outfile.write(infile.read(MAXFILEFRAME));
        }
        if(infile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Unzip %1 error!").arg(tmpFileName));
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            infile.close();
            outfile.close();
            continue;
        }
        outfile.close();
        infile.close();
        if(infile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Unzip %1 error!").arg(tmpFileName));
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            continue;
        }
        //>@���㴫��İٷֱ�
        emit progress(++i*100/total);
        emit message(QString("Unzip %1 success!").arg(tmpFileName));
        QtSleep(1);
    }
    zip.close();
    if(zip.getZipError()!=UNZ_OK)
    {
        IDE_TRACE();
        emit message(QString("Unzip %1 to %2 error!").arg(pZipFile).arg(pDstPath));
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }
#endif
#if (defined(UBUNTU) || defined(LINUX))
    system("sync");
#endif
    //>@���ͽ����ź�
    IDE_TRACE();
    emit end();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    return true;
}

ZipTaskThread::ZipTaskThread(QObject *parent) :
    QThread(parent)
{
    m_ZipTask.moveToThread(this);
}

bool ZipTaskThread::Zip(QString pSrcPath, QString pZipFile, bool pCover)
{
    if(isRunning())
        return false;
    start(QThread::LowPriority);
    wait(10);
    return m_ZipTask.Zip(pSrcPath, pZipFile, pCover);
}

bool ZipTaskThread::Unzip(QString pZipFile, QString pDstPath, bool pCover)
{
    if(isRunning())
        return false;
    start(QThread::LowPriority);
    wait(10);
    return m_ZipTask.Unzip(pZipFile, pDstPath, pCover);
}

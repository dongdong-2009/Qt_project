#include "workthread.h"
THREADCOPY CopyThread[MAXSIZE];

WorkThread::WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
    runningFlag = true;
    bufferLength = 1024;
    offset = 0;
    len = 0;
    copyedBytes = 0;
    jobId = -1;
}

WorkThread::~WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
    int i = 0;
    for(i = 0; i < MAXSIZE; i++)
    {

    }
}

void WorkThread::run()
{
    qDebug()<< __PRETTY_FUNCTION__;
    //IDE_TRACE_INT(len);
    QFile srcfile(src);
    QFile dstfile(dst);
    if(!srcfile.open(QIODevice::ReadOnly)){
        IDE_TRACE();
        return;
    }

    if(!dstfile.open(QIODevice::WriteOnly)){
        IDE_TRACE();
        return;
    }

    srcfile.seek(offset);
    dstfile.seek(offset);
//    char * buf = new char[bufferLength];
    char buf[bufferLength];
    //int count = len/bufferLength;
    qint64 readLength = 0;
    copyedBytes = 0;
    while(copyedBytes < len && runningFlag) {
        readLength = srcfile.read(buf, bufferLength);
        if((copyedBytes + readLength) <= len) {// 当已经拷贝的文件长度加上当前读到的文件长度小于等于需要拷贝的长度的时候
            dstfile.write(buf, readLength);  // 因为文件没有结束，可能会读到超过既定长度的文件
            copyedBytes += readLength;
            //emit copyedbytes(jobId,readLength);
        }else{
            dstfile.write(buf, len - copyedBytes);
            copyedBytes = len;
            //emit copyedbytes(jobId,len - copyedBytes);
        }
        emit copyedbytes(jobId, copyedBytes);
        //qDebug()<<"id:"<<jobId <<"copyedBytes: "<<copyedBytes;
    }
    srcfile.close();
    dstfile.close();

    emit jobFinished(jobId);
}

void WorkThread::setJob(int jobId, QString src, QString dst, qint64 offset, qint64 len)
{
    this->jobId = jobId;
    this->src = src;
    this->dst = dst;
    this->offset = offset;
    this->len = len;
}

void WorkThread::splitFileLength(const QString &filename, int Max)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    int flen = file.size();
    int blocksize = flen / Max;  // Max 是最大的线程数
    if (0 == flen - blocksize * Max) // 刚好能整除
    {
        int i = 0;
        for(i = 0; i < Max; i++)
        {
            CopyThread[i].id = i;
//            CopyThread[i].wThread = new WorkThread();
            CopyThread[i].size = blocksize;
            CopyThread[i].offset_filehead = CopyThread[i].id * CopyThread[i].size;
        }
        m_IsAverage = true;
    }
    else if (0 <= flen - blocksize * Max) // 不能整除，最后一个块将比其他的多一点
    {
        int i = 0;
        for(i = 0; i < Max-1; i++)
        {
            CopyThread[i].id = i;
            CopyThread[i].size = blocksize;
            CopyThread[i].offset_filehead = CopyThread[i].id * CopyThread[i].size;
        }
        CopyThread[i].id = i;
        CopyThread[i].size = flen - blocksize * (Max-1);
        CopyThread[i].offset_filehead = flen;
        m_IsAverage = false;
    }
}

void WorkThread::fileCopyStar()
{

}

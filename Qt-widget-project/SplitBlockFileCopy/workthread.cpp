#include "workthread.h"
THREADCOPY CopyThread[MAXSIZE];
WorkThread::WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
}

WorkThread::~WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
    int i = 0;
    for(i = 0; i < MAXSIZE; i++)
    {
        delete CopyThread[i].wThread;
    }
}

void WorkThread::run()
{
    qDebug()<< __PRETTY_FUNCTION__;

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
            CopyThread[i].wThread = new WorkThread();
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
            CopyThread[i].wThread = new WorkThread();
            CopyThread[i].size = blocksize;
            CopyThread[i].offset_filehead = CopyThread[i].id * CopyThread[i].size;
        }
        CopyThread[i].id = i;
        CopyThread[i].wThread = new WorkThread();
        CopyThread[i].size = flen - blocksize * (Max-1);
        CopyThread[i].offset_filehead = flen;
        m_IsAverage = false;
    }
}

void WorkThread::fileCopyStar()
{

}

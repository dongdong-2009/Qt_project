#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QDebug>
#include <QFile>
#include <QString>
#include "log.h"
#define MAXSIZE 10

typedef enum{
   COPY_START = 0,
   COPY_STOP,
   COPY_FILE_NAME,
   COPY_TOTAL_RATE,
   COPY_SINGLE_RATE,
   COPY_ERROR_MEM_FULL
}COPY_STATION;

typedef enum{
   SUCCESS = 0,
   ERROR_SRC_PATH_NULL,
   ERROR_DES_PATH_NULL,
   ERROR_NO_FILES,
   ERROR_MEM_FULL
}RETURN_VALUE;

typedef struct _THREADCOPY
{
    int id;                // 线程的id
//    WorkThread *wThread;   // 线程
    int size;              // 文件大小
    int offset_filehead;   // 文件偏移的位置
}THREADCOPY;

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread();
    void run();
    void splitFileLength(const QString & filename, int Max);
    void fileCopyStar();
    ~WorkThread();
    void setJob(int jobId, QString src, QString dst, qint64 offset, qint64 len);

signals:
    void copyedbytes(int jobId, qint64 bytes);
    void jobFinished(int jobId);

public slots:
private:
    bool m_IsAverage;
    qint32 bufferLength;
    volatile bool runningFlag;
    int jobId;
    QString src;
    QString dst;
    qint64 offset;
    qint64 len;
    qint64 copyedBytes;
};



#endif // WORKTHREAD_H

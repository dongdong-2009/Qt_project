#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QDebug>
#include <QFile>
#define MAXSIZE 20
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

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread();
    void run();
    void splitFileLength(const char* filename, int Max);
    void fileCopyStar();
    ~WorkThread();
signals:

public slots:
private:
    bool m_IsAverage;
};

typedef struct _THREADCOPY
{
    int id;                // 线程的id
    WorkThread *wThread;   // 线程
    int size;              // 文件大小
    int offset_filehead;   // 文件偏移的位置
}THREADCOPY;

#endif // WORKTHREAD_H

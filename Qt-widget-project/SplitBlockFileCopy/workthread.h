#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QDebug>

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

class WorkThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = 0);
    void run();
    ~WorkThread();
signals:

public slots:
};

#endif // WORKTHREAD_H

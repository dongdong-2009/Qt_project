#ifndef BLOCKFILECOPY_H
#define BLOCKFILECOPY_H

#include <QObject>
#include "workthread.h"
#include <QFile>
#include <QDebug>
#define MAXSIZE 20

typedef struct BLOCKSIZE
{
    int blockSize;
}BLOCKSIZE;

class BlockFileCopy : public QObject
{
    Q_OBJECT
public:
    explicit BlockFileCopy(QObject *parent = 0);
    ~BlockFileCopy();
    void splitFileLength(const char* filename, int Max);
signals:

public slots:

private:
    WorkThread *wThread[MAXSIZE];
    BLOCKSIZE block[MAXSIZE];
    bool m_IsAverage;
};

#endif // BLOCKFILECOPY_H

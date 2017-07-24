#include "blockfilecopy.h"

BlockFileCopy::BlockFileCopy(QObject *parent) : QObject(parent)
  , m_IsAverage(false)
{
    qDebug()<< __PRETTY_FUNCTION__;
}

BlockFileCopy::~BlockFileCopy()
{
    qDebug()<< __PRETTY_FUNCTION__;
}

// 将要拷贝的文件平均分割成不同的段，记录大小
void BlockFileCopy::splitFileLength(const char *filename, int Max)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    int flen = file.size();
    int blocksize = flen / Max;
    if (0 == flen - blocksize * Max) // 刚好能整除
    {
        int i = 0;
        for(i = 0; i < Max; i++)
        {
            block[i].blockSize = blocksize;
            m_IsAverage = true;
        }
    }
    else if (0 <= flen - blocksize * Max) // 不能整除，最后一个块将比其他的多一点
    {
        int i = 0;
        for(i = 0; i < Max-1; i++)
        {
            block[i].blockSize = blocksize;
        }
        block[Max-1].blockSize = flen - blocksize * (Max-1);
        m_IsAverage = false;
    }
}

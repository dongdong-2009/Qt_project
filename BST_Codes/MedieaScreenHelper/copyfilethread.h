#ifndef COPYFILETHREAD
#define COPYFILETHREAD
#include <QThread>
#include <QStringList>
#include "widget.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#define F_LEN 1024
class Widget;
enum{
    COPY_START = 0,
    COPY_STOP,
    COPY_FILE_NAME,
    COPY_TOTAL_RATE,
    COPY_SINGLE_RATE,
    COPY_ERROR_MEM_FULL
}COPY_STATION;

enum{
    SUCCESS = 0,
    ERROR_SRC_PATH_NULL,
    ERROR_DES_PATH_NULL,
    ERROR_NO_FILES,
    ERROR_MEM_FULL
}RETURN_VALUE;

typedef struct FILEINFO
{
    QString f_name;
    unsigned long f_size;
}FILEINFO;

class CopyfileThread : public QThread
{
    Q_OBJECT

public:
    CopyfileThread();

    void run();
    int copyStart();
    void copyStop();

    void setSrcDirPath(QString path) { srcPath=path; }
    void setDesDirPath(QString path) { desPath=path; }
    int GetListCounts(QStringList list);
    void printf_lists(QStringList list);
    QString Filepath_to_Dirpath(QString filepath);
    void GetAllFilesSize();
    unsigned long GetFiletotalSizes(QStringList list, FILEINFO file_Arr[]);


private:
    QStringList fileList;

    QString srcPath;
    QString desPath;
    bool bStop;
    unsigned long fileTotalSize;
    unsigned long curSize;
    Widget *wid;

    int Copyfile(QString fileName);
    FILEINFO fileArr[F_LEN];  // 统计一个文件下的各个文件的文件名和大小
    int m_filenumber;

signals:
    void copyStationSig(int flag,QString qsText);
};


#endif // COPYFILETHREAD


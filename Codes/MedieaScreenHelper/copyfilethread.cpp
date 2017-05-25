#include "copyfilethread.h"

CopyfileThread::CopyfileThread()
{
    wid = new Widget;
}

int CopyfileThread::copyStart()
{
    QStringList Sourcefilelist;
    Sourcefilelist.clear();
    if (NULL != wid)
    {
        if (wid->GetvideoClicked())
        {
            Sourcefilelist.at(0) = wid->getVideoList();
            qDebug()<<Sourcefilelist.at(0)<<'\n';
        }
        else if (wid->GetpictureClicked())
        {
            Sourcefilelist = wid->GetPictureList();
            printf_lists(Sourcefilelist);
        }
    }


}
// 获取QStringList 的个数
int CopyfileThread::GetListCounts(QStringList list)
{
    QStringList::iterator it = list.begin();
    int i = 0;
    while(it != list.end())
    {
        ++i;
        ++it;
    }
    return i;
}

//调试打印输出
void CopyfileThread::printf_lists(QStringList list)
{
    int i = GetListCounts(list);
    int j;
    for(j = 0; j < i; j++)
    {
        qDebug()<<list.at(i)<<'\n';
    }
}

//将文件的路径转换成文件夹路径
QString CopyfileThread::Filepath_to_Dirpath(QString filepath)
{
    QStringList list = filepath.split("/");
    QString path;
    path.clear();
    int i = GetListCounts(list);
    for (j = 0; j < i-1; j++)   //去掉最后一个文件名，组成文件夹的路径
    {
        path.append("/");
        path.append(list.at(j));
    }
    path.append("/");
    return path;
}

// 获取用户选择文件夹下的所有文件的大小，调用QFileInfo的size方法实现
void CopyfileThread::GetAllFilesSize()
{
    // loading filelist
    QString fileName;
    QDir fileDir;
    QFileInfoList fileInfoList;
    QFileInfo fileInfo;
    int i = 0;
    m_filenumber = 0;
    fileDir.setPath(srcPath);          // 传入源文件的文件夹路径
    fileDir.setFilter(QDir::Files);             // 只选择文件其他的过滤掉
    fileInfoList = fileDir.entryInfoList();     // 获取文件信息列表

    do{
        fileInfo = fileInfoList.at(i);
        fileArr[i].f_name = fileInfo.fileName();
        fileArr[i].f_size = fileInfo.size()/1024;  // 保存文件夹下所有文件的大小
        fileName = srcPath+"/"+fileInfo.fileName();
        i++;
    }while(i < fileInfoList.size());
    m_filenumber = i;
    qDebug() << "m_filenumber = "<< m_filenumber << '\n';
}

// 获取用户选择的拷贝的文件大小
unsigned long CopyfileThread::GetFiletotalSizes(QStringList list, FILEINFO file_Arr[])
{
    int i = GetListCounts(list);
    int j, k;
    fileTotalSize = 0;
    for(j = 0; j < m_filenumber; j++)
    {
        for (k = 0; k < i; k++)
        {
            if (file_Arr[k].f_name == list.at(j))
            {
                fileTotalSize += file_Arr[k].f_size;
            }
        }
    }
    qDebug() << "filetotalsize = "<<fileTotalSize << '\n';
    return fileTotalSize;
}

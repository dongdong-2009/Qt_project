#include "widget.h"
#include "ui_widget.h"

qint64 CopyedBytes[MAXSIZE];
QMap<int, WorkThread*> wThreadMap;
QStringList m_listview = QStringList();

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_filemodel = NULL;

    cpthread = NULL;
    qthread = NULL;
    initUi();
}

void Widget::initUi()
{
    ui->progressBar->hide();
    ui->btn_copyFile->setEnabled(false);
    ui->btn_delete->setEnabled(false);
    ui->btn_clear->setEnabled(false);

    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    connect(ui->btn_selectFile, &QPushButton::clicked, this, &Widget::onOpenmultifileDaliog);
    connect(ui->btn_copyFile, &QPushButton::clicked, this, &Widget::onCopyFileClick);
    connect(ui->btn_clear, &QPushButton::clicked, this, &Widget::onClearFileList);
    connect(ui->btn_delete, &QPushButton::clicked, this, &Widget::onDeleteFileList);
    connect(this, &Widget::btnEnabledChanged, this, &Widget::onSetBtnEnabled);
//    connect(this, SIGNAL(copyPercentages(int)), this, SLOT(onUpdateProgressBar(int)));

    initCopyThread();
}

Widget::~Widget()
{
    delete ui;
}

QStringList Widget::getFileList(QStringListModel *qlistmode)
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    if (NULL != qlistmode)
    {        
        qDebug()<<"stringlist = " << qlistmode->stringList();
        return qlistmode->stringList();
    }
    qDebug()<<"stringlist is empty";
    return QStringList();
}

void CopyThread::countTotalFileSize(QStringList m_list)
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    QStringList::iterator it = m_list.begin();
    while (it != m_list.end())
    {
//        m_FileTotalSize += QFileInfo(*it).size() / 1024; // 计算出指定路径文件的大小
        m_FileTotalSize += QFile(*it).size(); // 计算出指定路径文件的大小
        qDebug()<<"*it = "<< *it << QFile(*it).size()<< m_FileTotalSize;
        ++ it;
    }
}

QString CopyThread::getFileName(QString filepath)
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    QString flist = filepath.split("/").last();
    qDebug()<< "flist = "<< flist;
    return flist;
}

// QMap<int, WorkThread *> 不能没有参数<int , WorkThread*>
QMap<int, WorkThread *> Widget::initCopyThread()
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    wThreadMap.clear();
    for (int j = 0; j < MAXSIZE; j++)
    {
        WorkThread *cthread = new WorkThread();
        if (NULL != cthread)
        {
            wThreadMap.insert(j, cthread);
        }
        CopyedBytes[j] = 0;
    }
    return wThreadMap;
}

// 将全局的拷贝了字节数目的数组清零
void Widget::resetCopyBytes()
{
    for(int i = 0; i < MAXSIZE; i++)
    {
        CopyedBytes[i] = 0;
    }
}

//void Widget::startWork()
//{
//    qDebug()<<"enter startwork()";
//    countTotalFileSize(m_listview);
//    ui->progressBar->setVisible(true);
//    for(int i = 0; i < m_listview.length(); i++)
//    {
//        qDebug()<<"m_listview.at(i) = "<< m_listview.at(i);
//        QString desFilename = m_DestPath + "/" + getFileName(m_listview.at(i)); // 目标文件的文件名已经路径
//        QMap<int, WorkThread *>::const_iterator it = wThreadMap.constBegin();  // 获取多线程对象的头结点
//        WorkThread *beginThread;
//        int j = 0;
//        while (it != wThreadMap.constEnd())
//        {
//            qDebug()<< "*it = "<< *it;
//            beginThread = it.value(); // 取出容器中的每个value的值
//            if (0 == j)
//                beginThread->splitFileLength(m_listview.at(i), MAXSIZE);
//            beginThread->setJob(j++, m_listview.at(i), desFilename); // 给每个线程设置对应的ID，源文件路径文件，目标文件的路径文件
//            ++ it;
//        }
//        QMap<int, WorkThread *>::const_iterator itstart = wThreadMap.constBegin();  // 获取多线程对象的头结点
//        while(itstart != wThreadMap.constEnd())
//        {
//            qDebug()<< "*itstart = "<< *itstart;
//            beginThread = itstart.value();
//            beginThread->start(); // 开始启动多线程进行分段拷贝
//            ++ itstart;
//        }
//        ui->label_copyname->setText(getFileName(m_listview.at(i)));
//        QMap<int, WorkThread *>::iterator itstart1 = wThreadMap.begin();  // 获取多线程对象的头结点
//        int flagarr[MAXSIZE] = {0};
//        bool flag = false;
//        while(1) // 等待第一个文件拷贝结束之后，进行第二个文件的拷贝
//        {
//            int k = 0; qDebug()<< "enter while 1";
//            while(itstart1 != wThreadMap.end())
//            {
//                beginThread = itstart1.value();
//                if (beginThread->selfCopyEndFlag)
//                {
//                    flagarr[k] = 1;
//                    qDebug("flagarr[%d]=  %d",k ,flagarr[k]);
//                    k++;
//                }
//                else
//                {
//                    flagarr[k] = 0;
//                    qDebug("flagarr[%d]=  %d",k ,flagarr[k]);
//                    k++;
//                }
//                ++ itstart1;
//            }
//            for (int p = 0; p < MAXSIZE; p++)
//            {
//                if (0 == flagarr[p])
//                {
//                    flag = false; qDebug()<< "0 flag = "<< flag;
//                    break;
//                }
//                else
//                {
//                    flag = true; qDebug()<< "1 flag = "<< flag;
//                }
//                QCoreApplication::processEvents();
//            }
//            if (flag)
//            {
//                flag = false;
//                break;
//            }
//            itstart1 = wThreadMap.begin();
//        }
//    }
//}

void Widget::onClearFileList()
{
    m_filemodel = NULL;
    this->ui->listView->setModel(m_filemodel);
    m_listview = getFileList(m_filemodel);  // 将m_listview 置为空
    emit btnEnabledChanged(m_listview);
}

void Widget::onSetBtnEnabled(QStringList m_list)
{
    if (m_list.length() > 0)
    {
        ui->btn_copyFile->setEnabled(true);
        ui->btn_delete->setEnabled(true);
        ui->btn_clear->setEnabled(true);
    }
    else
    {
        ui->btn_copyFile->setEnabled(false);
        ui->btn_delete->setEnabled(false);
        ui->btn_clear->setEnabled(false);
    }
}

void CopyThread::onCountPercentage(int id, qint64 fbytes)
{
    qDebug()<< __PRETTY_FUNCTION__ << "id = "<< id << " fbytes = "<< fbytes;
    qint64 tmp = 0;
    int percent = 0;
    CopyedBytes[id] = fbytes;
    for(int i = 0; i< MAXSIZE; i++){
        tmp += CopyedBytes[i]; // 计算多线程总的拷贝长度
    }
    qDebug()<< "tmp = "<< tmp << "m_FileTotalsize = "<< m_FileTotalSize;
    percent = tmp * 100 / m_FileTotalSize ;
    m_curPercent = percent;
    qDebug()<<"percent = "<< percent;
    if (m_curPercent > m_perPercent)
    {
        m_perPercent = m_curPercent;
        emit copyPercentages(m_curPercent);
    }
}

void Widget::onUpdateProgressBar(int value)
{
    if (value <= 100)
    {
        qDebug()<<"progressBar value = "<< value;
        ui->progressBar->setValue(value);
    }
}

void Widget::onShowProgress(bool visflag)
{
    qDebug()<< __PRETTY_FUNCTION__<< "visflag = "<< visflag;
    ui->progressBar->setVisible(visflag);
}

void Widget::onShowLabelText(int i)
{
    qDebug()<< __PRETTY_FUNCTION__<< "i = "<< i;
    ui->label_copyname->setText(m_listview.at(i));
}

// 当用户点击完复制文件之后，所做的处理
void Widget::onCopyFileClick()
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    /*将拷贝的线程单独放到一个线程中进行，不能在主线程中进行，在主线程中会阻碍界面的更新*/
    qthread = new QThread(this);
    cpthread = new CopyThread();
    cpthread->moveToThread(qthread);
    connect(cpthread, &CopyThread::showProgress, this, &Widget::onShowProgress, Qt::QueuedConnection);
    connect(cpthread, &CopyThread::showLabelText, this, &Widget::onShowLabelText, Qt::QueuedConnection);
    connect(cpthread, &CopyThread::copyPercentages, this, &Widget::onUpdateProgressBar, Qt::QueuedConnection);
    qthread->start();
    cpthread->startWork();
}

QStringListModel * Widget::onDeleteFileList(/*QStringList files*/)
{
    int index = getFileList(m_filemodel).length() - 1;
    m_filemodel->removeRow(index);
    m_listview = getFileList(m_filemodel);
    emit btnEnabledChanged(m_listview);
    return m_filemodel;
}

void Widget::onOpenmultifileDaliog()
{
    QStringList files = QFileDialog::getOpenFileNames(this,"Select one or more files to open",
    ".","Files you choice (*.png *.jpg *.mp4 *.flv *.rmvb *.sh *.txt *.doc *.pdf *.xlsx)");
    m_listview.clear();
    m_listview = files;
    qDebug()<< "m_listview = "<< m_listview;
    onSetBtnEnabled(m_listview);

    m_filemodel = new QStringListModel(files, this);
    this->ui->listView->setModel(m_filemodel);
}

CopyThread::CopyThread()
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    m_DestPath = QApplication::applicationDirPath();
    m_curPercent = 0;
    m_perPercent = 0;
    m_FileTotalSize = 0;
}

CopyThread::~CopyThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
}

// 连接所有新建线程的信号和槽函数
void CopyThread::connThreadSlot()
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    countTotalFileSize(m_listview);
    QMap<int, WorkThread*>::const_iterator itw = wThreadMap.constBegin();
    WorkThread *wjob;
    while (itw != wThreadMap.constEnd())
    {
        wjob = itw.value();
        bool flag = connect(wjob, SIGNAL(copyedbytes(int,qint64)),
                            this, SLOT(onCountPercentage(int,qint64)), Qt::QueuedConnection);
        ++ itw;
        qDebug()<< "flag = "<< flag ;
    }
}

void CopyThread::startWork()
{
    qDebug()<<__PRETTY_FUNCTION__<<"线程ID为："<<QThread::currentThreadId();
    connThreadSlot();
    QStringList temp_list = m_listview;
    emit showProgress(true);
    for(int i = 0; i < temp_list.length(); i++)
    {
        qDebug()<<"m_listview.at(i) = "<< temp_list.at(i);
        QString desFilename = m_DestPath + "/" + getFileName(temp_list.at(i)); // 目标文件的文件名已经路径
        QMap<int, WorkThread *>::const_iterator it = wThreadMap.constBegin();  // 获取多线程对象的头结点
        WorkThread *beginThread;
        int j = 0;
        while (it != wThreadMap.constEnd())
        {
            qDebug()<< "*it = "<< *it;
            beginThread = it.value(); // 取出容器中的每个value的值
            if (0 == j)
                beginThread->splitFileLength(temp_list.at(i), MAXSIZE);
            beginThread->setJob(j++, temp_list.at(i), desFilename); // 给每个线程设置对应的ID，源文件路径文件，目标文件的路径文件
            ++ it;
        }
        QMap<int, WorkThread *>::const_iterator itstart = wThreadMap.constBegin();  // 获取多线程对象的头结点
        while(itstart != wThreadMap.constEnd())
        {
            qDebug()<< "*itstart = "<< *itstart;
            beginThread = itstart.value();
            beginThread->start(); // 开始启动多线程进行分段拷贝
            ++ itstart;
        }
        emit showLabelText(i);
        QMap<int, WorkThread *>::iterator itstart1 = wThreadMap.begin();  // 获取多线程对象的头结点
        int flagarr[MAXSIZE] = {0};
        bool flag = false;
        while(1) // 等待第一个文件拷贝结束之后，进行第二个文件的拷贝
        {
            int k = 0; qDebug()<< "enter while 1";
            while(itstart1 != wThreadMap.end())
            {
                beginThread = itstart1.value();
                if (beginThread->selfCopyEndFlag)
                {
                    flagarr[k] = 1;
                    qDebug("flagarr[%d]=  %d",k, flagarr[k]);
                    k++;
                }
                else
                {
                    flagarr[k] = 0;
                    qDebug("flagarr[%d]=  %d",k, flagarr[k]);
                    k++;
                }
                ++ itstart1;
            }
            for (int p = 0; p < MAXSIZE; p++)
            {
                if (0 == flagarr[p])
                {
                    flag = false; qDebug()<< "0 flag = "<< flag;
                    break;
                }
                else
                {
                    flag = true; qDebug()<< "1 flag = "<< flag;
                }
                QCoreApplication::processEvents();
            }
            if (flag)
            {
                flag = false;
                break;
            }
            itstart1 = wThreadMap.begin();
        }
    }
}


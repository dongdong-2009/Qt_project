#include "widget.h"
#include "ui_widget.h"
qint64 CopyedBytes[MAXSIZE];
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_FileTotalSize = 0;
    initUi();
    m_curPercent = 0;
    m_perPercent = 0;
}

void Widget::initUi()
{
    ui->progressBar->hide();
    ui->btn_copyFile->setEnabled(false);
    ui->btn_delete->setEnabled(false);
    ui->btn_clear->setEnabled(false);
//    ui->label_copyname->setVisible(false);
    connect(ui->btn_selectFile, &QPushButton::clicked, this, &Widget::openmultifileDaliog);
    connect(ui->btn_copyFile, &QPushButton::clicked, this, &Widget::startWork);
    connect(ui->btn_clear, &QPushButton::clicked, this, &Widget::clearFileList);
    connect(ui->btn_delete, &QPushButton::clicked, this, &Widget::deleteFileList);
    connect(this, &Widget::btnEnabledChanged, this, &Widget::setBtnEnabled);
    connect(this, SIGNAL(copyPercentages(int)), this, SLOT(updateProgressBar(int)));
    m_DestPath = QApplication::applicationDirPath();
    qDebug()<< "m_DestPath = " << m_DestPath; // 是一个目录不带 /
    initCopyThread();
    connThreadSlot();
}

Widget::~Widget()
{
    delete ui;
}

QStringList Widget::getFileList(QStringListModel *qlistmode)
{
    if (NULL != qlistmode)
    {        
        qDebug()<<"stringlist = " << qlistmode->stringList();
        return qlistmode->stringList();
    }
    qDebug()<<"stringlist is empty";
    return QStringList();
}

void Widget::countTotalFileSize(QStringList m_list)
{
    QStringList::iterator it = m_list.begin();
    while (it != m_list.end())
    {
//        m_FileTotalSize += QFileInfo(*it).size() / 1024; // 计算出指定路径文件的大小
        m_FileTotalSize += QFile(*it).size(); // 计算出指定路径文件的大小
        qDebug()<<"*it = "<< *it << QFile(*it).size()<< m_FileTotalSize;
        ++ it;
    }
}

QString Widget::getFileName(QString filepath)
{
    QString flist = filepath.split("/").last();
    qDebug()<< "flist = "<< flist;
    return flist;
}

// QMap<int, WorkThread *> 不能没有参数<int , WorkThread*>
QMap<int, WorkThread *> Widget::initCopyThread()
{
    wThread.clear();
    for (int j = 0; j < MAXSIZE; j++)
    {
        WorkThread *cthread = new WorkThread();
        if (NULL != cthread)
        {
            wThread.insert(j, cthread);
        }
        CopyedBytes[j] = 0;
    }
    return wThread;
}

// 连接所有新建线程的信号和槽函数
void Widget::connThreadSlot()
{
    qDebug()<< __PRETTY_FUNCTION__;
    QMap<int, WorkThread*>::const_iterator itw = wThread.constBegin();
    WorkThread *wjob;
    while (itw != wThread.constEnd())
    {
        wjob = itw.value();
        bool flag = connect(wjob, SIGNAL(copyedbytes(int,qint64)),
                            this, SLOT(countPercentage(int,qint64)), Qt::QueuedConnection);
        ++ itw;
        qDebug()<< "flag = "<< flag ;
    }
}

// 将全局的拷贝了字节数目的数组清零
void Widget::resetCopyBytes()
{
    for(int i = 0; i < MAXSIZE; i++)
    {
        CopyedBytes[i] = 0;
    }
}

void Widget::startWork()
{
    qDebug()<<"enter startwork()";
    countTotalFileSize(m_listview);
    ui->progressBar->setVisible(true);
    for(int i = 0; i < m_listview.length(); i++)
    {
        qDebug()<<"m_listview.at(i) = "<< m_listview.at(i);
        QString desFilename = m_DestPath + "/" + getFileName(m_listview.at(i)); // 目标文件的文件名已经路径
        QMap<int, WorkThread *>::const_iterator it = wThread.constBegin();  // 获取多线程对象的头结点
        WorkThread *beginThread;
        int j = 0;
        while (it != wThread.constEnd())
        {
            qDebug()<< "*it = "<< *it;
            beginThread = it.value(); // 取出容器中的每个value的值
            if (0 == j)
                beginThread->splitFileLength(m_listview.at(i), MAXSIZE);
            beginThread->setJob(j++, m_listview.at(i), desFilename); // 给每个线程设置对应的ID，源文件路径文件，目标文件的路径文件
            ++ it;
        }
        QMap<int, WorkThread *>::const_iterator itstart = wThread.constBegin();  // 获取多线程对象的头结点
        while(itstart != wThread.constEnd())
        {
            qDebug()<< "*itstart = "<< *itstart;
            beginThread = itstart.value();
            beginThread->start(); // 开始启动多线程进行分段拷贝
            ++ itstart;
        }
        ui->label_copyname->setText(getFileName(m_listview.at(i)));
//        QMap<int, WorkThread *>::iterator itstart1 = wThread.begin();  // 获取多线程对象的头结点
//        int flagarr[MAXSIZE] = {0};
//        bool flag = false;
//        while(1) // 等待第一个文件拷贝结束之后，进行第二个文件的拷贝
//        {
//            int k = 0; qDebug()<< "enter while 1";
//            while(itstart1 != wThread.end())
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
//            itstart1 = wThread.begin();
//        }
    }
}

void Widget::clearFileList()
{
    m_filemodel = NULL;
    this->ui->listView->setModel(m_filemodel);
    m_listview = getFileList(m_filemodel);  // 将m_listview 置为空
    emit btnEnabledChanged(m_listview);
}

void Widget::setBtnEnabled(QStringList m_list)
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

void Widget::countPercentage(int id, qint64 fbytes)
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

void Widget::updateProgressBar(int value)
{
    if (value <= 100)
    {
        qDebug()<<"progressBar value = "<< value;
        ui->progressBar->setValue(value);
    }
}

void Widget::controlProgressBar(bool visflag)
{
    qDebug()<< "visflag = "<< visflag;
    ui->progressBar->setVisible(visflag);
    ui->label_copyname->setVisible(visflag);
}

QStringListModel * Widget::deleteFileList(/*QStringList files*/)
{
    int index = getFileList(m_filemodel).length() - 1;
    m_filemodel->removeRow(index);
    m_listview = getFileList(m_filemodel);
    emit btnEnabledChanged(m_listview);
    return m_filemodel;
}

void Widget::openmultifileDaliog()
{
    QStringList files = QFileDialog::getOpenFileNames(this,"Select one or more files to open",
    ".","Files you choice (*.png *.jpg *.mp4 *.flv *.rmvb *.sh *.txt *.doc *.pdf *.xlsx)");
    m_listview.clear();
    m_listview = files;
    qDebug()<< "m_listview = "<< m_listview;
    setBtnEnabled(m_listview);

    m_filemodel = new QStringListModel(files, this);
    this->ui->listView->setModel(m_filemodel);
}


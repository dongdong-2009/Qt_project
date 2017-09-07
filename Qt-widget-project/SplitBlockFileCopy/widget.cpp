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
    m_DestPath = QApplication::applicationDirPath();
    qDebug()<< "m_DestPath = " << m_DestPath; // 是一个目录不带 /
    initCopyThread();
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
    QMap<int, WorkThread *>::const_iterator it = wThread.constBegin();
    WorkThread *wjob;
    while (it != wThread.constEnd()) {
        wjob = it.value();
        connect(wjob, SIGNAL(copyedbytes(int,qint64)), this, SLOT(countPercentage(int,qint64)));
        ++it;
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
    countTotalFileSize(m_listview);
    for(int i = 0; i < m_listview.length(); i++)
    {
        qDebug()<<"m_listview.at(i) = "<< m_listview.at(i);
        getFileName(m_listview.at(i));
        QMap<int, WorkThread *>::iterator it = wThread.constBegin();
        WorkThread *beginThread;
        while (it != wThread.constEnd()) {
            beginThread = it.value();
            beginThread->setJob();
        }
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
    qint64 tmp = 0;
    int percent = 0;
    CopyedBytes[id-1] = fbytes;
    for(int i = 0; i< MAXSIZE; i++){
        tmp += CopyedBytes[i]; // 计算多线程总的拷贝长度
    }
    percent = tmp / m_FileTotalSize * 100;
    emit copyPercentages(percent);

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
                            ".","Images or video (*.png *.jpg *.mp4 *.flv *.rmvb)");
    m_listview.clear();
    m_listview = files;
    qDebug()<< "m_listview = "<< m_listview;
    setBtnEnabled(m_listview);

    m_filemodel = new QStringListModel(files, this);
    this->ui->listView->setModel(m_filemodel);
}


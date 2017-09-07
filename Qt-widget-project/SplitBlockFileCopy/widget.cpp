#include "widget.h"
#include "ui_widget.h"

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

void Widget::startWork()
{
    countTotalFileSize(m_listview);
    for(int i = 0; i < m_listview.length(); i++)
    {
        qDebug()<<"m_listview.at(i) = "<< m_listview.at(i);
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


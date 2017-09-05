#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUi();
}

void Widget::initUi()
{
    ui->progressBar->hide();
    ui->btn_copyFile->setEnabled(false);
    ui->btn_delete->setEnabled(false);
    ui->btn_clear->setEnabled(false);
    connect(ui->btn_selectFile, &QPushButton::clicked, this, &Widget::openmultifileDaliog);
    connect(ui->btn_copyFile, &QPushButton::clicked, this, &Widget::startWork);
}

Widget::~Widget()
{
    delete ui;
}

QStringList Widget::getFileList(QListView *qlist)
{
    if (NULL != qlist)
    {
        ui->listView->model()
    }
}

void Widget::startWork()
{

}

void Widget::openmultifileDaliog()
{
    QStringList files = QFileDialog::getOpenFileNames(this,"Select one or more files to open",
                            ".","Images or video (*.png *.jpg *.mp4 *.flv *.rmvb)");
    m_listview.clear();
    m_listview = files;
    qDebug()<< "m_listview = "<< m_listview;
    if (m_listview.length() > 0)
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
//    QStringListModel m;
//    m.setStringList(files);
    m_filemodel = new QStringListModel(files, this);
    this->ui->listView->setModel(model);
}


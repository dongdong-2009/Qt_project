#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug()<<"mUsbAct.readFile() = "<< mUsbAct.readFile();
    setCopyFileList(mUsbAct.readFile());
    ui->listView->setModel(&model);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    mUsbAct.dirFileSize("/home/libo/Desktop/1243");
    connect(&mUsbAct, &UsbAction::sigUpdateProgress, this, &Widget::updateProgressBar);
    mUsbAct.copyFile("", "");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setCopyFileList(QStringList list)
{
    model.setStringList(list);
}

void Widget::updateProgressBar(int num)
{
    ui->progressBar->setValue(num);
}

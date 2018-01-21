#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(&mUsbAct, &UsbAction::sigUpdateProgress, this, &Widget::updateProgressBar);

    mUsbAct.dirFileSize("/home/libo/Desktop/1243");  // 设置文件夹路径
    // 判断文件夹中的文件的个数
    if (0 >= mUsbAct.getFileNum())   // 没有对应的文件夹或者对应的文件夹下没有文件
    {
        ui->label_waring->setText("U disk has no resource, Please confirm!");
        ui->listView->setVisible(false);
        ui->progressBar->setVisible(false);
    }
    else
    {
        mUsbAct.copyFile("", "");  // 执行拷贝文件
        qDebug()<<"mUsbAct.readFile() = "<< mUsbAct.readFile();
        setCopyFileList(mUsbAct.readFile());
        ui->label_waring->setVisible(false);
        ui->listView->setModel(&model);
        ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    }
//    QPixmap mUsbPic("/usr/bst/usrfs/Theme/sodimas/usb.png");
    QPixmap mUsbPic("/home/libo/Desktop/Repository/new_project/ToolKit/usrfs/Theme/sodimas/usb.png");
    ui->label_usbpicture->setPixmap(mUsbPic);
    ui->label_usbpicture->setScaledContents(true);  // 设置自动缩放，不然图片显示不清晰
    ui->label_usbpicture->show();
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
    if (ui->label_usbpicture->isHidden())
    {
        ui->label_usbpicture->show();
    }
    else
    {
        ui->label_usbpicture->hide();
    }
    if (100 == num)
    {
        ui->label_usbpicture->show();
    }
}

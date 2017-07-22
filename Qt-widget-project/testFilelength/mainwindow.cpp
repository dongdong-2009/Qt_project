#include "mainwindow.h"
#include "ui_mainwindow.h"
static unsigned char Covert[20];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CountFileSize(FILENAME, Covert, 4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CountFileSize(const char *filename, unsigned char *str, int count)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    int length = file.size();
    qDebug("filelength = %d", length);
    IntToUnsignedChar(length, count, str);
}

//void MainWindow::IntToUnsignedChar(int length, int count, unsigned char *str)
//{
//    qDebug()<< __PRETTY_FUNCTION__;
//    unsigned char temp[10];
////    int i = 0;
//    int j = 1;
//    int t = 1;
//    int c = count;
//    int len;
//    int num = 0;
//    while (length)
//    {
//        for(j = 1; j < c; j++) // 计算对应的权值10的多少次方
//        {
//            t = t*10;
//        }
//        c = c - 1;
//        len = length / t;  // 计算商
//        qDebug("len = %d", len);
//        if (0 == len)
//        {
//            temp[num] = len + 0x00;
//            qDebug("temp[%d] = %x", num, temp[num]);
//        }
//        else if (len > 0)
//        {
//            if (len <= 255)
//            {
//                temp[num] = len + 0x00;
//                length = length - len*t;
//            }
//            else if (len > 255)
//            {
//                temp[num] = 0xff;
//                length = length - 255*t;
//            }
//        }
//        qDebug("temp[%d] = %x", num, temp[num]);
//        if (num <= count)
//            num++;
//        t = 1;
//    }
//    memcpy(str, temp, count);
//}

void MainWindow::IntToUnsignedChar(int length, int count, unsigned char *str)
{
    qDebug()<< __PRETTY_FUNCTION__;
    int array[10];
    int i = 0;
    while(length)
    {
        array[i] = length % 10;
        length = length / 10;
        qDebug("array[%d] = %d", i, array[i]);
        i++;
    }
    qDebug("i = %d", i);
    int j = 0;
    for (j = i-1; j >= 0; j--)
    {
        qDebug("array[%d] = %d", j, array[j]);
    }
}

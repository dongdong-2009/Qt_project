#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QObject>
#include <stdio.h>
using namespace std;
static unsigned char sstr[100];
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label_floor->setTextFormat(Qt::RichText);
    ui->label_floor->setText("1");
    pro = Protocoldeal::GetInstance();
    connect(pro, SIGNAL(AcceptDataFormBottom(unsigned char)), this, SLOT(setstring(unsigned char)), Qt::DirectConnection);
}

Widget::~Widget()
{
    delete pro;
    delete ui;
}

void Widget::setstring(unsigned char str)
{
    qDebug()<<__PRETTY_FUNCTION__<< "the slots is running !";
    if (NULL != pro )
    {
        pro->CopyStringFromProtocol(str, sstr);
        cout << "end copy in setstring\n";
        pro->PrintString(sstr, 19);
    }
    if ('a' == *(sstr + 1))
        ui->label_floor->setText("a");
    else
        ui->label_floor->setText("A");
    int i;
    for(i = 0; i < 19; i++)
    {
        //cout<< totalBuf[i]<< " ";
        printf("%X ", sstr[i]);
    }
    memset(sstr, 0 , 5);
}

void Widget::DealData(char str[])
{
//    if (NULL != pro)
//    {
//        pro->CopyStringFromProtocol(str, str);
//    }
}




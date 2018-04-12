#include "widget.h"
#include "ui_widget.h"
#include <QByteArray>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QByteArray byteArray("hello world");
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();;
    char ch = 0x21;
    byteArray.append(ch);
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();
    ch = 0x24;
    byteArray.insert(0, ch);
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();
}

Widget::~Widget()
{
    delete ui;
}

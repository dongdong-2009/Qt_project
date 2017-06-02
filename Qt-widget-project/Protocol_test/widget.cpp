#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QObject>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label_floor->setTextFormat(Qt::RichText);
    ui->label_floor->setText("1");
    pro = new Protocoldeal;
    connect(pro, SIGNAL(AcceptDataFormBottom(QString)), this, SLOT(setstring(QString)));
}

Widget::~Widget()
{
    delete pro;
    delete ui;
}

void Widget::sett()
{
    ui->label_floor->setText("15");
}

void Widget::setstring(QString str)
{
    qDebug()<<__PRETTY_FUNCTION__<< "the slots is running !";
    ui->label_floor->setText(str);
}

void Widget::DealData(char str[])
{

}




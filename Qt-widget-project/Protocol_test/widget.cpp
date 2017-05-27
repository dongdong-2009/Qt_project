#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QObject>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    ui->label_floor->setTextFormat(Qt::RichText);
    ui->label_floor->setText("1");
    QTimer* tim = new QTimer;
    connect(tim, SIGNAL(timeout()), this, SLOT(sett()));
    tim->start(3000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sett()
{
    ui->label_floor->setText("15");
}

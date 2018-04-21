#include "widget.h"
#include <QDebug>
#include "define.h"
#include <QDir>
#include <stdio.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QString tmp(__FILE__);
    qDebug()<<__FILE__<<tmp;
    QDir temDir(tmp);
    QString absDir = temDir.absolutePath();
    IDE_TRACE();
    qDebug()<<QDir::homePath();
    qDebug()<<"absDir = " <<absDir;
    QDir dir;
    qDebug()<<dir.absoluteFilePath("Widget.cpp");
    QString cmd = QString("pwd > %1/%2").arg(QCoreApplication::applicationDirPath()).arg("path.txt");
    qDebug()<<"cmd = "<<cmd;
    system(cmd.toLatin1().data());
}

Widget::~Widget()
{

}

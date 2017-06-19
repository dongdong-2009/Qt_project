#include "widget.h"
#include "fileupdate.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    Widget w;
    w.show();
//    w.showFullScreen();
    FileUpdate f;
    f.show();
    return a.exec();
}

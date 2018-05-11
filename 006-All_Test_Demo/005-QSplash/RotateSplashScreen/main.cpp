#include "widget.h"
#include <QApplication>
#include "cmysplashscreen.h"
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMySplashScreen *splash = new CMySplashScreen(4500);

    QGraphicsScene tmpScene;
    QGraphicsView tmpView;

    tmpView.setScene(&tmpScene);
    tmpView.show();

//    tmpView.setGeometry(0, 0, 272, 480);
//    splash->setDirection(Vertical);
    tmpView.setGeometry(0, 0, 480, 272);
    splash->setDirection(Horizontal);
    splash->init();

    tmpScene.addWidget(splash);
//    tmpView.rotate(90);
    tmpView.rotate(180);

    tmpView.setGeometry(-5, -5, 480+10, 272+10);
    tmpView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmpView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    splash->show();
    QtSleep(5500);

    Widget w;
    splash->finish(&w);

    splash->deleteLater();
    splash = 0;

    return a.exec();
}

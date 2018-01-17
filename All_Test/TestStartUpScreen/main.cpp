#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <mainwindow.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include "cmysplashscreen.h"
#include "customprogressindicator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPixmap pixmap("/home/libo/Desktop/logo.png");
//    QSplashScreen screen(pixmap);
//    screen.show();
#if 1
    int delayTime = 5;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
         app.processEvents();
    }
#endif

//#if 1
//    QDateTime n = QDateTime::currentDateTime();
//    QDateTime now;
//    do{
//        now = QDateTime::currentDateTime();
//        app.processEvents();
//    } while (n.secsTo(now) <= 5);//5为需要延时的秒数
//#endif

//    MainWindow window;
//    window.show();

//    screen.finish(&window);
//    CustomProgressIndicator *pIndicator = new CustomProgressIndicator;
//    pIndicator->setColor(Qt::red);
//    pIndicator->startAnimation();

    CMySplashScreen *splash = new CMySplashScreen(pixmap, 3500);
    splash->setDisabled(true); //禁用用户的输入事件响应
    splash->show();
    return app.exec();
}

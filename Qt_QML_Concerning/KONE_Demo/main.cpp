#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "findqmlcomponent.h"
#include <QObject>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<ShowFloor>("lb2616.tools.ShowFloor", 1, 0, "ShowFloor");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject * topLevel = engine.rootObjects().value(0);
    QQuickWindow* window = qobject_cast<QQuickWindow*>(topLevel);
    window->showFullScreen();
//    window->show();

//    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:///main.qml")));
//    QObject *obj = component.create();
//    QObject *ArrowDirection = obj->findChild<QObject *>("arrowdownpicture");
//    if (ArrowDirection)
//    {
//        ArrowDirection->setProperty("visble", false);
//    }
    return app.exec();
}


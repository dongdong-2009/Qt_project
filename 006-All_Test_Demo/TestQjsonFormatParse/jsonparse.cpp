#include "jsonparse.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonValue>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QCoreApplication>

JsonParse::JsonParse(QObject *parent) : QObject(parent)
  , mRunPath(QCoreApplication::applicationDirPath())
{
}

void JsonParse::readJson()
{
    QString val;
    QFile file;
    file.setFileName(mRunPath + "/test.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return;
    }

    val = file.readAll();
    file.close();
    qWarning() << val;

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();

    QJsonValue value0 = sett2.value(QString("appDesc"));
    qWarning() << value0;

    QJsonValue value = sett2.value(QString("appName"));
    qWarning() << value;
    QJsonObject item = value.toObject();
    qWarning() << tr("QJsonObject of description: ") << item;

    /* incase of string value get value and convert into string*/
    qWarning() << tr("QJsonObject[appName] of description: ") << item["description"];
    QJsonValue subobj = item["description"];
    qWarning() << subobj.toString();

    /* incase of array get array and convert into string*/
    qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
    QJsonArray test = item["imp"].toArray();
    qWarning() << test[1].toString();
}

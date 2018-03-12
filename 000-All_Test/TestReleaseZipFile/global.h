#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include "define.h"

QString GetDirectoryName(QString DirPath);
QString getFileDirectory(QString FilePath);
bool CreatPath(QString pPath);
QStringList ErgodicDirectory(QString d);
QString getFileName(QString FilePath);
QString getParentDirectory(QString DirPath);
QString  BiteorMega(int peso);
#endif // GLOBAL_H

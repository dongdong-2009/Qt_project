#ifndef FILEUPDATE_H
#define FILEUPDATE_H

#include <QObject>
#include <QTimer>
#include <QDebug>
class FileUpdate2 : public QObject
{
    Q_OBJECT
public:
    explicit FileUpdate2(QObject *parent = 0);

signals:
    void sendPercent(int val);
public slots:
    void ChangePercent();
};

#endif // FILEUPDATE_H

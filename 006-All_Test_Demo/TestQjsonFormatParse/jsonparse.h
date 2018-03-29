#ifndef JSONPARSE_H
#define JSONPARSE_H

#include <QObject>

class JsonParse : public QObject
{
    Q_OBJECT
public:
    explicit JsonParse(QObject *parent = 0);
    void readJson();
signals:

public slots:
private:
    QString mRunPath;
};

#endif // JSONPARSE_H

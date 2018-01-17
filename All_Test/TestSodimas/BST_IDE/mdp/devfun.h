#ifndef DEVFUN_H
#define DEVFUN_H

#include <QObject>

class devfun : public QObject
{
    Q_OBJECT
public:
    explicit devfun(QObject *parent = 0);

signals:

public slots:
};

#endif // DEVFUN_H

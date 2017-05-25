#ifndef DIGITCLOCK_H
#define DIGITCLOCK_H

#include <QWidget>
#include <QLCDNumber>
class digitclock : public QWidget
{
    Q_OBJECT

public:
    digitclock(QWidget *parent = 0);
    ~digitclock();
private slots:
    void showTime();
};

#endif // DIGITCLOCK_H

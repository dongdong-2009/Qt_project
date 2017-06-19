#ifndef FILEUPDATE_H
#define FILEUPDATE_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class FileUpdate;
}

class FileUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit FileUpdate(QWidget *parent = 0);
    ~FileUpdate();
public slots:
    void ChangeValue(int val);
private:
    Ui::FileUpdate *ui;
};

#endif // FILEUPDATE_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QStringList>
#include <QStringListModel>
#include <QPushButton>
#include <QFileDialog>
#include <QListView>
#include <QMap>
#include "workthread.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void initUi();
    ~Widget();
    QStringList getFileList(QStringListModel *qlistmode);

private:
    Ui::Widget *ui;
    QStringList m_listview;
    QStringListModel *m_filemodel;
    QMap<int, WorkThread*> wThread;

public slots:
    void openmultifileDaliog();
    void startWork();
//    QStringListModel * clearFileList(QStringList files);
//    QStringListModel * deleteFileList(QStringList files);
    void clearFileList();
};

#endif // WIDGET_H

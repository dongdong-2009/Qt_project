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
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include "workthread.h"
#define ONE_KB 1024

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
    void countTotalFileSize(QStringList m_list);

private:
    Ui::Widget *ui;
    QStringList m_listview;
    QStringListModel *m_filemodel;
    QMap<int, WorkThread*> wThread;
    qint64 m_FileTotalSize;
    QString m_DestPath;

public slots:
    void openmultifileDaliog();
    void startWork();
//    QStringListModel * clearFileList(QStringList files);
    QStringListModel * deleteFileList(/*QStringList files*/);
    void clearFileList();
    void setBtnEnabled(QStringList m_list);
    void updateProgressBar(int value);
    void controlProgressBar(bool visflag);

signals:
    void btnEnabledChanged(QStringList m_list);
    void allCopyWork(bool flag);
};

#endif // WIDGET_H

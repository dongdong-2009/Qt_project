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
    QString getFileName(QString filepath);
    QMap<int, WorkThread*> initCopyThread();
    void connThreadSlot();
    void resetCopyBytes();

private:
    Ui::Widget *ui;
    QStringList m_listview;
    QStringListModel *m_filemodel;
    QMap<int, WorkThread*> wThread;
    qint64 m_FileTotalSize;
    QString m_DestPath;
    int m_perPercent;
    int m_curPercent;

public slots:
    void openmultifileDaliog();
    void startWork();
    QStringListModel * deleteFileList(); // 注意信号的参数要大于等于槽函数的信号的个数
    void clearFileList();
    void setBtnEnabled(QStringList m_list);
    void countPercentage(int id, qint64 fbytes);
    void updateProgressBar(int value);
    void controlProgressBar(bool visflag);

signals:
    void btnEnabledChanged(QStringList m_list);
    void allCopyWork(bool flag);
    void copyPercentages(int per);
};

#endif // WIDGET_H

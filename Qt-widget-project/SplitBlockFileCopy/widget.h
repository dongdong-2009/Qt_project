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
#include <QThread>
#define ONE_KB 1024

namespace Ui {
class Widget;
}

class CopyThread:public QObject
{
public slots:
    void startWork();
    void connThreadSlot();

signals:
    void showProgress(bool flag);
    void updateProgess(int pvalue);
    void showLabelText(int i);
};

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
    CopyThread *cpthread;
    QThread *qthread;

public slots:
    void onOpenmultifileDaliog();
    void startWork();
    QStringListModel * onDeleteFileList(); // 注意信号的参数要大于等于槽函数的信号的个数
    void onClearFileList();
    void onSetBtnEnabled(QStringList m_list);
    void onCountPercentage(int id, qint64 fbytes);
    void onUpdateProgressBar(int value);
    void onControlProgressBar(bool visflag);
    void onCopyFileClick();

signals:
    void btnEnabledChanged(QStringList m_list);
    void allCopyWork(bool flag);
    void copyPercentages(int per);
};

#endif // WIDGET_H

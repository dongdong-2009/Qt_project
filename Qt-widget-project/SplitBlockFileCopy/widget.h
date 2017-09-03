#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QStringList>
#include <QStringListModel>
#include <QPushButton>
#include <QFileDialog>
#include <QListView>

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

private:
    Ui::Widget *ui;
    QStringList m_listview;

public slots:
    void OpenmultifileDaliog();
};

#endif // WIDGET_H

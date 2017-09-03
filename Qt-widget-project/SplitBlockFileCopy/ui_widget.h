/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QProgressBar *progressBar;
    QPushButton *btn_selectFile;
    QListView *listView;
    QPushButton *btn_copyFile;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(520, 476);
        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(140, 340, 251, 23));
        progressBar->setValue(0);
        btn_selectFile = new QPushButton(Widget);
        btn_selectFile->setObjectName(QStringLiteral("btn_selectFile"));
        btn_selectFile->setGeometry(QRect(60, 280, 99, 27));
        listView = new QListView(Widget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(10, 20, 501, 231));
        btn_copyFile = new QPushButton(Widget);
        btn_copyFile->setObjectName(QStringLiteral("btn_copyFile"));
        btn_copyFile->setGeometry(QRect(350, 280, 99, 27));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        btn_selectFile->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\346\226\207\344\273\266", 0));
        btn_copyFile->setText(QApplication::translate("Widget", "\345\244\215\345\210\266\346\226\207\344\273\266", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

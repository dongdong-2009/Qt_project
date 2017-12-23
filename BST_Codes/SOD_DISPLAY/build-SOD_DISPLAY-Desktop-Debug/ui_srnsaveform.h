/********************************************************************************
** Form generated from reading UI file 'srnsaveform.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SRNSAVEFORM_H
#define UI_SRNSAVEFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SrnSaveForm
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_logo;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *SrnSaveForm)
    {
        if (SrnSaveForm->objectName().isEmpty())
            SrnSaveForm->setObjectName(QString::fromUtf8("SrnSaveForm"));
        SrnSaveForm->resize(451, 371);
        SrnSaveForm->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        verticalLayout = new QVBoxLayout(SrnSaveForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 157, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_logo = new QLabel(SrnSaveForm);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        label_logo->setScaledContents(true);

        horizontalLayout->addWidget(label_logo);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 156, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(SrnSaveForm);

        QMetaObject::connectSlotsByName(SrnSaveForm);
    } // setupUi

    void retranslateUi(QWidget *SrnSaveForm)
    {
        SrnSaveForm->setWindowTitle(QApplication::translate("SrnSaveForm", "Form", 0, QApplication::UnicodeUTF8));
        label_logo->setText(QApplication::translate("SrnSaveForm", "Sodimas", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SrnSaveForm: public Ui_SrnSaveForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SRNSAVEFORM_H

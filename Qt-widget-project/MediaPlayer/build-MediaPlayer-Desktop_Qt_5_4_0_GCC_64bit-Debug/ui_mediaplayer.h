/********************************************************************************
** Form generated from reading UI file 'mediaplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIAPLAYER_H
#define UI_MEDIAPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customslider.h>

QT_BEGIN_NAMESPACE

class Ui_MediaPlayer
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_4;
    QLabel *label_player;
    QHBoxLayout *horizontalLayout_2;
    CustomSlider *slider_progress;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_volume;
    QPushButton *pushButton_play_and_pause;
    QPushButton *pushButton_open_file;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MediaPlayer)
    {
        if (MediaPlayer->objectName().isEmpty())
            MediaPlayer->setObjectName(QStringLiteral("MediaPlayer"));
        MediaPlayer->resize(647, 420);
        centralWidget = new QWidget(MediaPlayer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_player = new QLabel(centralWidget);
        label_player->setObjectName(QStringLiteral("label_player"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_player->sizePolicy().hasHeightForWidth());
        label_player->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(157, 157, 157, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        label_player->setPalette(palette);
        label_player->setStyleSheet(QStringLiteral("background-color: rgb(157, 157, 157);"));
        label_player->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_player, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        slider_progress = new CustomSlider(centralWidget);
        slider_progress->setObjectName(QStringLiteral("slider_progress"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(slider_progress->sizePolicy().hasHeightForWidth());
        slider_progress->setSizePolicy(sizePolicy1);
        slider_progress->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(slider_progress);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_volume = new QPushButton(centralWidget);
        pushButton_volume->setObjectName(QStringLiteral("pushButton_volume"));

        horizontalLayout_2->addWidget(pushButton_volume);

        pushButton_play_and_pause = new QPushButton(centralWidget);
        pushButton_play_and_pause->setObjectName(QStringLiteral("pushButton_play_and_pause"));

        horizontalLayout_2->addWidget(pushButton_play_and_pause);

        pushButton_open_file = new QPushButton(centralWidget);
        pushButton_open_file->setObjectName(QStringLiteral("pushButton_open_file"));

        horizontalLayout_2->addWidget(pushButton_open_file);


        gridLayout_4->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        MediaPlayer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MediaPlayer);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 647, 23));
        MediaPlayer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MediaPlayer);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MediaPlayer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MediaPlayer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MediaPlayer->setStatusBar(statusBar);

        retranslateUi(MediaPlayer);

        QMetaObject::connectSlotsByName(MediaPlayer);
    } // setupUi

    void retranslateUi(QMainWindow *MediaPlayer)
    {
        MediaPlayer->setWindowTitle(QApplication::translate("MediaPlayer", "MediaPlayer", 0));
        label_player->setText(QApplication::translate("MediaPlayer", "\346\222\255\346\224\276\345\214\272\345\237\237", 0));
        pushButton_volume->setText(QApplication::translate("MediaPlayer", "\351\237\263\351\207\217", 0));
        pushButton_play_and_pause->setText(QApplication::translate("MediaPlayer", "\346\222\255\346\224\276", 0));
        pushButton_open_file->setText(QApplication::translate("MediaPlayer", "\346\211\223\345\274\200", 0));
    } // retranslateUi

};

namespace Ui {
    class MediaPlayer: public Ui_MediaPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPLAYER_H

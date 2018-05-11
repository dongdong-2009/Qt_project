#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_player = new Mplayer();
    if (m_player)
    {
//        m_player->setBackgroundMuisc("/usr/bst/usrfs/Theme/sodimas/music/M0.wav", 10);
        m_player->startWork();
//        m_player->setBackgroundPlayerVolume(10);
    }
}

Widget::~Widget()
{
    delete ui;
}

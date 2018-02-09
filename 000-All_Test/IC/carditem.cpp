#include "carditem.h"
#include "ui_carditem.h"

CardItem::CardItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardItem)
{
    ui->setupUi(this);
}

CardItem::~CardItem()
{
    delete ui;
}

void CardItem::SetCardState(QString pState)
{
    if(!pState.compare(GBKToUTF8("����")))
    {
        ui->btn_act->setText(GBKToUTF8("��ʧ"));
    }
    else if(!pState.compare(GBKToUTF8("�ѹ�ʧ")))
    {
        ui->btn_act->setText(GBKToUTF8("����"));
    }
}

void CardItem::on_btn_log_clicked()
{
    emit sCardLog();
}

void CardItem::on_btn_act_clicked()
{
    QString tmpText = ui->btn_act->text();
    if(!tmpText.compare(GBKToUTF8("��ʧ")))
    {
        emit sCardLoss();
    }
    else if(!tmpText.compare(GBKToUTF8("����")))
    {
        emit sCardRepair();
    }
}

void CardItem::on_btn_LogOut_clicked()
{
    emit sCardLogOut();
}

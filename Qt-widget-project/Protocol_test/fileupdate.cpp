#include "fileupdate.h"
#include "ui_fileupdate.h"

FileUpdate::FileUpdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileUpdate)
{
    qDebug()<< __PRETTY_FUNCTION__;
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->label_text->hide();
}

FileUpdate::~FileUpdate()
{
    qDebug()<< __PRETTY_FUNCTION__;
    delete ui;
}

void FileUpdate::ChangeValue(int val)
{
    qDebug()<< __PRETTY_FUNCTION__;
    ui->progressBar->show();
    ui->label_text->show();
    ui->progressBar->setValue(val);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTimeEdit>
static int i = 1;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->display("1");
    clock_temp = new QTimer;
    connect(clock_temp, SIGNAL(timeout()), this, SLOT(startlcd()));
    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(settime()));
    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(startlcd()));
    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(cleartime()));
    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(pauselcd()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::settime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->lineEdit->setText(str);
}

void MainWindow::cleartime()
{
    ui->lineEdit->clear();
}

void MainWindow::startlcd()
{
 //   i = 3;
    i ++;
    clock_temp->start(1000);
    ui->lcdNumber->display(i);
}

void MainWindow::pauselcd()
{
    clock_temp->stop();
}

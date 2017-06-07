#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QObject>
#include <stdio.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDateTime>
#include <QFont>
#include <QPalette>
#include <QPropertyAnimation>
using namespace std;
static unsigned char sstr[100];

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    bool flagtime = connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(ChangePosition()));
    qDebug() << "flagtiem = "<< flagtime;

    pro = Protocoldeal::GetInstance();
    connect(pro, SIGNAL(AcceptDataFormBottom(unsigned char)), this, SLOT(setstring(unsigned char)));

    UiInit();

    SetPicture(":/new/prefix1/images/ArrowUp.png", ui->graphicsView_Arrow);
    SetPicture(":/new/prefix1/images/logo.png", ui->graphicsView_logo);
    SetPicture(":/new/prefix1/images/number2.png", ui->graphicsView_floor1);
    SetPicture(":/new/prefix1/images/number8.png", ui->graphicsView_floor2);
    SetPicture(":/new/prefix1/images/welcome_text.png", ui->graphicsView_scroll);
    AnimationForPicture();
}

Widget::~Widget()
{
    delete pro;
    delete timer;
    delete ui;
    qDebug() << __PRETTY_FUNCTION__ << "delete all pointers";
}

void Widget::UiInit()
{
    ShowDate();
    showTime();
    SetWidgetBackGround(":/new/prefix1/images/background.png");     // 设置图片作为widget的背景
    HideGraphicViewBorder();   // 将QGraphicview控件边框设置为透明的
}

//
void Widget::AnimationForPicture()
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->graphicsView_scroll, "pos");
    pAnimation->setDuration(100);
    pAnimation->setStartValue(QRect(549, 696, 324, 36));
    pAnimation->setEndValue(QRect(250, 250, 324, 36));
    pAnimation->setLoopCount(-1);
    ui->graphicsView_scroll->move(300, 696);
    qDebug() << __PRETTY_FUNCTION__;
    //pAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
}

void Widget::ChangePosition()
{
    static int x = 549;
    if (x - 10 > 292)
    {
        x = x - 10;
        ui->graphicsView_scroll->move(x, 696);
    }
    else
    {
        ui->graphicsView_scroll->move(549, 696);
        x = 549;
    }
}

//  设置图片作为widget的背景
void Widget::SetWidgetBackGround(QString path)
{
    QPalette pal;
    QPixmap pixmap(path);
    pal.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(pal);
}

// 将QGraphicview控件边框设置为透明的
void Widget::HideGraphicViewBorder()
{
    ui->graphicsView_Arrow->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_emergency->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_floor1->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_floor2->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_logo->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_scroll->setStyleSheet("background: transparent;border:0px");
}

// 显示图片，将图片显示在指定的QGraphicview 的控件中
void Widget::SetPicture(QString path, QGraphicsView *graphic)
{
//    this->image = new QImage();
    QImage image;
//    bool flag = image->load(str);
    bool flag = image.load(path);
    cout << "flag = "<< flag << endl;
    cout << "noting"<< endl;
    QGraphicsScene *scene = new QGraphicsScene(this);
//    scene->addPixmap(QPixmap::fromImage(*image));
    scene->addPixmap(QPixmap::fromImage(image));
    graphic->setScene(scene);
    graphic->show();
}

// 显示日期
void Widget::ShowDate()
{
    QDateTime Time = QDateTime::currentDateTime();
    QString tim = Time.toString("MM-dd-yyyy");
    qDebug()<<"tim = "<< tim<<'\n';

    QFont ft;      // 设置字号
    ft.setPointSize(20);
    ui->label_date->setFont(ft);

    QPalette pe;   // 设置颜色
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_date->setPalette(pe);
    ui->label_date->setText(tim);
}

// 显示时间，每秒刷新一次
//QString Widget::GetTime()
//{
//    QDateTime Time = QDateTime::currentDateTime();
////    QString tim = Time.toString("h:mm ap"); // 12小时制
//    QString tim = Time.toString("hh:mm"); // 24小时制
//    qDebug()<<"tim = "<< tim<<'\n';
//    return tim;
//}

// 显示时间，每秒刷新一次
void Widget::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh : mm");
    if ((time.second() % 2) == 0)
        text[3] = ' ';

    QFont ft;                    // 设置字号
    ft.setPointSize(20);
    ui->label_time->setFont(ft);

    QPalette pe;                // 设置颜色
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_time->setPalette(pe);
    ui->label_time->setText(text);
    qDebug()<< "text = "<< text;
}


void Widget::setstring(unsigned char str)
{
    qDebug()<<__PRETTY_FUNCTION__<< "the slots is running !";
    if (NULL != pro )
    {
        pro->CopyStringFromProtocol(str, sstr);
        cout << "end copy in setstring\n";
        pro->PrintString(sstr, 19);
    }
    if ('a' == *(sstr + 1))
        ui->label_time->setText("a");
    else
        ui->label_time->setText("A");
    int i;
    for(i = 0; i < 19; i++)
    {
        //cout<< totalBuf[i]<< " ";
        printf("%X ", sstr[i]);
    }
    printf("\n");
    memset(sstr, 0 , 5);
}

void Widget::DealData(char str[])
{
//    if (NULL != pro)
//    {
//        pro->CopyStringFromProtocol(str, str);
//    }
}




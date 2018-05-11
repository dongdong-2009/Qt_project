#ifndef CMYSPLASHSCREEN_H
#define CMYSPLASHSCREEN_H

#include <QSplashScreen>
#include <QPixmap>
#include <QProgressBar>
#include <QList>
#include <QtGlobal>
#include <QLabel>
#include <QString>

typedef enum{
    Vertical = 0,
    Horizontal = 1,
    HVBoth = 2,
    HVNone
} DIRECTION;///横向 纵向

class CMySplashScreen: public QSplashScreen
{
    Q_OBJECT

public:
    CMySplashScreen(int time);
    ~CMySplashScreen();
    void setBackGround(QString path);
    void setLogoPosition(QString path);
    void setProgressBarPositon();
    void setTextSysInfo(QString pSys);
    void setTextAppInfo(QString pApp);
    void setSysAndAppPosition();
    void setDirection(DIRECTION pDirection);
    void init();

private:
    //进度条
    QProgressBar *ProgressBar;
    //随机数列表
    QList<int> numbersList;
    //启动界面停留的时间
    int elapseTime;
    QLabel *bootLogo;
    QLabel *sysInfo;
    QLabel *appInfo;
    QString runPath;
    DIRECTION m_direction;

private:
    void setProgress();
    void generateAscendRandomNumber();

private slots:
    void slotUpdateProgress();
};

#endif // CMYSPLASHSCREEN_H

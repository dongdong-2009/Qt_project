#ifndef SAFTHREAD_H
#define SAFTHREAD_H

#include <QThread>
#include <QVector>

class SafThread: public QThread
{
    Q_OBJECT
public:
    SafThread(QObject *parent = 0);

signals:
    void sigArrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL);
    void sMessageAudio(int pAudioNum);
     void sigShowFuncLogo(int funcLogoIndex,int soundNumber);

    void gpioChange();

private slots:
      void parseFrame();

private:
    void run();

    static void my_signal_func(int signum);

    static int s_fd;

    static quint8  s_saf;
    static quint8  s_flm;
    static quint8  s_fld;
    static quint8  s_sgo;
    static quint8  s_talk;
    static quint8  s_sf;
    static quint8  s_dip1;
    static quint8  s_dip2;
    static quint8  s_dip3;
    static quint8  s_dip4;

    quint8 m_sgoLast;
    quint8 m_sgoUpedge;
    quint8 m_talkLast;
    quint8 m_talkUpedge;
    quint8 m_safLast;

    quint8 m_openingMessage;
    bool   m_scroll;

    static bool s_gpioChangeFlag;
    static bool s_parseFrameFlag;
};

#endif // SAFTHREAD_H



/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
static int fd;

void my_signal_func(int signum)
{
    unsigned char Value[10];
    unsigned char Cnt;
    char *str[] = {"ArrowEnable:","GongSta:","ArrowUp:","TalkSta:","Floor:",
                   "ArrowDn:","DIP_4:","DIP_3:","DIP_2:","DIP_1:"};

    read(fd,Value,10);
    for(Cnt = 0; Cnt < 10; Cnt++){
        printf("%s%d ",str[Cnt],Value[Cnt]);
    }
    printf("\r\n");
}

int main(int argc,char **argv)
{
    int flag;

    //��Ӧ�ó����в�׽SIGIO�ź�(����ź������������ͳ�����)
    signal(SIGIO,my_signal_func);
    fd = open("/dev/saf",O_RDWR);
    if(fd < 0){
        printf("Can't open\n");
        return -1;
    }
    printf("Open file id is:%d\n",fd);
    //����ǰ����PID����Ϊfd�ļ�����Ӧ��������Ҫ����SIGIO�źŽ���PID
    fcntl(fd,F_SETOWN,getpid());
    //��ȡfd�Ĵ򿪷�ʽ
    flag = fcntl(fd,F_GETFL);
    //��fd�Ĵ򿪷�ʽ����ΪFASYNC,��֧���첽֪ͨ
    fcntl(fd,F_SETFL,flag|FASYNC);
    while(1){
        sleep(1000);
    }
}
*/

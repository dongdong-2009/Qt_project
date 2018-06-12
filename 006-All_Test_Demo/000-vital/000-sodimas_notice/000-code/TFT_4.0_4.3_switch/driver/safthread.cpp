#include "safthread.h"
#include "define.h"

#ifdef LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#endif

int SafThread::s_fd = 0;
quint8 SafThread::s_saf = 0;
quint8 SafThread::s_flm = 0;
quint8 SafThread::s_fld = 0;
quint8 SafThread::s_sgo = 0;
quint8 SafThread::s_talk = 0;
quint8 SafThread::s_sf = 0;

quint8 SafThread::s_dip1 = 0;
quint8 SafThread::s_dip2 = 0;
quint8 SafThread::s_dip3 = 0;
quint8 SafThread::s_dip4 = 0;

bool SafThread::s_gpioChangeFlag = false;
bool SafThread::s_parseFrameFlag = false;

SafThread::SafThread(QObject *parent)
    :QThread(parent)
{
    m_safLast = 0;
    m_sgoLast = 0;
    m_sgoUpedge = 0;
    m_talkLast = 0;
    m_talkUpedge = 0;
    m_openingMessage = 1;   //TODO: �Ժ��������ļ�����
    m_scroll = false;
    start(QThread::HighestPriority);
}

void SafThread::run()
{
    int flag;
#ifdef LINUX
    //��Ӧ�ó����в�׽SIGIO�ź�(����ź������������ͳ�����)
    signal(SIGIO, SafThread::my_signal_func);
    s_fd = open("/dev/saf",O_RDWR);
    if(s_fd < 0){
        printf("Can't open\n");
        return;
    }
    printf("Open file id is:%d\n",s_fd);
    //����ǰ����PID����Ϊfd�ļ�����Ӧ��������Ҫ����SIGIO�źŽ���PID
    fcntl(s_fd,F_SETOWN,getpid());
    //��ȡfd�Ĵ򿪷�ʽ
    flag = fcntl(s_fd,F_GETFL);
    //��fd�Ĵ򿪷�ʽ����ΪFASYNC,��֧���첽֪ͨ
    fcntl(s_fd,F_SETFL,flag|FASYNC);

   while(1)
   {
       if(s_gpioChangeFlag)
            parseFrame();
   }
#endif
}

void SafThread::parseFrame()
{
    s_gpioChangeFlag = false;
    //sgo������
    if(s_sgo == 1)
    {
        if(m_sgoLast == 0)
            m_sgoUpedge = 1;
        else
             m_sgoUpedge = 0;
        m_sgoLast = 1;
    }
    else
    {
        m_sgoUpedge = 0;
        m_sgoLast = 0;
    }
    //talk������
    if(s_talk == 1)
    {
        if(m_talkLast == 0)
            m_talkUpedge = 1;
        else
             m_talkUpedge = 0;
        m_talkLast = 1;
    }
    else
    {
        m_talkUpedge = 0;
        m_talkLast = 0;
    }


    qDebug() << "Start.....................";
    qDebug() << "   s_saf:" << s_saf << "  s_flm:" << s_flm << "   s_fld:" << s_fld << "   s_sgo:" << m_sgoUpedge
             << "   s_talk:" << m_talkUpedge << " s_sf:" << s_sf << "         s_dip1:" << s_dip1 << " s_dip2:" << s_dip2
             << "   s_dip3:" << s_dip3 <<"  s_dip4:" << s_dip4;


    //��ͷ�͵�վ��
    if(s_sf)
    {
        emit sigArrowAction(0,0,false);
        qDebug() << "s_flm:"  <<  s_flm  << "s_fld:" << s_fld << "scroll: 0";
        if(m_sgoUpedge)
        {
            if(m_openingMessage)
            {
                qDebug() << "emit sMessageAudio(44)";
                emit sMessageAudio(44);
            }
        }
    }
    else
    {
        if(s_saf == 52)
        {
            m_scroll = true;
        }
        else if(s_saf == 53)
        {
            m_scroll = false;
        }
        emit sigArrowAction((bool)s_flm,(bool)s_fld,m_scroll);
        qDebug() << "s_flm:"  <<  s_flm  << "s_fld:" << s_fld << "scroll:" << m_scroll;
        if(m_sgoUpedge)
        {
            if(s_flm == 1 && s_flm ==0)
            {
                emit sMessageAudio(40);
                qDebug() << "emit sMessageAudio(40)";
            }
            else if(s_flm == 0 && s_flm ==1)
            {
                qDebug() << "emit sMessageAudio(41)";
                emit sMessageAudio(41);
            }
            else if(s_flm == 1 && s_flm ==1)
            {
                qDebug() << "emit sMessageAudio(42)";
                emit sMessageAudio(42);
            }

            if(m_openingMessage)
            {
                qDebug() << "emit sMessageAudio(44)";
                emit sMessageAudio(44);
            }

        }
    }
    //¥���¥�㱨վ

    if(s_saf >= 0 && s_saf <= 39)
    {
        if(s_saf != m_safLast)
        {
            if(m_talkUpedge)
            {
                qDebug() << QString("emit sMessageAudio(saf)").arg(s_saf);
                emit sigShowFuncLogo(s_saf,s_saf);
                m_safLast  = s_saf;
            }
        }
    }

//    40:���� Inspection
//    41:���� Access forbidden
//    46:���� fire mode
//    47:Liftier
//    48:cell CLEEULE
//    51:logo
//    �������⹦��ֻ��ͼ�꣬û����Ƶ����˲�����ظ���¥��֡�����ظ�����

    if(s_saf == 59)
    {
                       //��ʾ�����յ�Frame 59 ʱ����ʾ��һ��¥��֡��Ӧ�����ݣ�¥��֡��֡��Ϊ1-39,70��
        if(m_talkUpedge)
        {
            emit sMessageAudio(45);
            qDebug() << QString("emit sMessageAudio(45)");
        }
    }

    if(s_saf == 45)
    {
        //emit sMessageAudio(48);  //�ͻ��Ѿ�������Ƶ��Ϊ�������������֡��¥�㲥�����ظ�һ��
        emit sMessageAudio(m_safLast);
        qDebug() << "The floor broadcast repeated";
    }
    if(s_saf == 44)   //overload
    {
        emit sMessageAudio(43);            //bipbipbip
        emit sMessageAudio(m_safLast);
        qDebug() << "bipbipbip";
        qDebug() << "The floor broadcast repeated";
    }
    if(s_saf == 56)     //door close
    {
        emit sMessageAudio(43);      //bipbipbip
        emit sMessageAudio(m_safLast);
        qDebug() << "bipbipbip";
        qDebug() << "The floor broadcast repeated";
    }

//    60֡��ֻҪ��60֡������ʾͼ���Լ�������Ƶ������ҪSGO��TALK�źţ��������֡��¥�㲥�����ظ�һ�Ρ�
//    if(mSAFLine->mSafFrameNum == 60)
//    {
//        emit sMessageAudio(43);
//        emit sMessageAudio(mOldFrameNum);
//    }
      qDebug()    <<   "End.....................";

      s_parseFrameFlag = false;
}

void SafThread::my_signal_func(int signum)
{
    while(s_parseFrameFlag);
    s_parseFrameFlag = true;

    quint8 Value[10];
//    unsigned char Cnt;

//    char *str[] = {"ArrowEnable:","GongSta:","ArrowUp:","TalkSta:","Floor:",
//                   "ArrowDn:","DIP_4:","DIP_3:","DIP_2:","DIP_1:"};

    read(s_fd,Value,10);
//    for(Cnt = 0; Cnt < 10; Cnt++){
//        printf("%s%d ",str[Cnt],Value[Cnt]);
//    }
//    printf("/r/n")

    s_saf = Value[4];
    if(s_saf == 70)
        s_saf =0;

    if(Value[2] == 0)
        s_flm = 1;
    else
        s_flm = 0;
    if(Value[5] == 0)
        s_fld = 1;
    else
        s_fld = 0;
    if(Value[1] == 0)
        s_sgo = 1;
    else
        s_sgo = 0;

    if(Value[3] == 0)
        s_talk = 1;
    else
        s_talk = 0;
    s_sf = Value[0];

    if(Value[9] == 0)
        s_dip1 = 1;
    else
        s_dip1 = 0;
    if(Value[8] == 0)
        s_dip2 = 1;
    else
        s_dip2 = 0;
    if(Value[7] == 0)
        s_dip3 = 1;
    else
        s_dip3 = 0;
    if(Value[6] == 0)
        s_dip4 = 1;
    else
        s_dip4 = 0;
//    qDebug() << "   s_saf:" << s_saf << "  s_flm:" << s_flm << "   s_fld:" << s_fld << "   s_sgo:" << s_sgo
//             << "   s_talk:" << s_talk << " s_sf:" << s_sf << "         s_dip1:" << s_dip1 << " s_dip2:" << s_dip2
//             << "   s_dip3:" << s_dip3 <<"  s_dip4:" << s_dip4;
    s_gpioChangeFlag = true;
}

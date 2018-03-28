#include "mplayer.h"
//#ifdef LINUX
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/types.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
//#endif

#define D_MONITORDELAY  500
#define D_PROCESSWAIT   200

#define D_MAXBUFFERSIZE   10

#define MPLAYER_STD_BUFFER_LENGHT 1024

const  char* GET_CURRENT_POSITION = "get_percent_pos\n";
const  char* PAUSE_PLAYER = "pause\n";

int backgroundPlayer_fd;
int backgroundPlayerPipe[2];
QString backgroundMusicPath = "/opt/M0.mp3";
QString backgroundVolume = "0";

bool initBackgroundplayerProcess()
{
//#ifdef LINUX
    pid_t pid;
    unlink("/tmp/musicplayer_fifo");
    if(mkfifo("/tmp/musicplayer_fifo", O_CREAT|0666) == -1)
    {
        return false;
    }
    if(pipe(backgroundPlayerPipe) == -1)
    {
        return false;
    }
    if((pid = fork()) == -1)
    {
        return false;
    }
    else if(pid == 0)
    {
        close(backgroundPlayerPipe[0]);
        dup2(backgroundPlayerPipe[1], 1);
        execlp("mplayer", "mplayer", "-slave", "-quiet", "-input", "file=/tmp/musicplayer_fifo", \
               "-ao", "alsa", "-loop", "0", "-volume", backgroundVolume.toStdString().c_str(), \
               backgroundMusicPath.toStdString().c_str(), (char *)NULL);
    }
    else
    {
        if((backgroundPlayer_fd = open("/tmp/musicplayer_fifo", O_RDWR)) == -1)
        {
            return false;
        }
    }
//#endif
    return true;
}

Mplayer::Mplayer()
{
    m_DefDelay = 1000;
    m_AbortAudio = false;
    m_CurMedia = 0;
    m_BufferIndex = -1;
    m_BufferLoop = LOOP_NONE;
    m_backgroundPlayerAlive = false;
    m_backgroundPlayerPaused = true;
    m_TestTimer = new QTimer;
    QObject::connect(m_TestTimer, SIGNAL(timeout()), this, SLOT(backgroundVolumeKeeper()));
    m_TestTimer->setInterval(2000);
    m_backgroundVolume = 0;
}

Mplayer::~Mplayer()
{

}

void Mplayer::setBackgroundMuisc(QString  musicPath, int volume)
{
    if(QFile::exists(musicPath))
    {
        backgroundMusicPath = musicPath;
        ///IDE_TRACE_STR(backgroundMusicPath);
    }
    else
    {
        IDE_TRACE_STR(musicPath);
    }

    if(volume > 0 && volume < 100)
    {
        m_backgroundVolume = volume;
        backgroundVolume = QString("%1").arg(volume);
        //IDE_TRACE_STR(backgroundVolume);
    }
}

void Mplayer::startWork()
{
    IDE_TRACE();
    ///m_mainPlayerAlive = initMainplayerProcess();
    ///IDE_TRACE_STR(backgroundMusicPath);
    if(QFile::exists(backgroundMusicPath))
    {
        ///IDE_TRACE();
        m_backgroundPlayerAlive = initBackgroundplayerProcess();
    }
    else
    {
        IDE_TRACE();
    }
    start();
    ///setBackgroundPlayerVolume(10);
    ///playBackgroundPlayer();
    m_TestTimer->start();
}

void Mplayer::run()
{
    IDE_TRACE();
    while(1)
    {
        //playBackgroundPlayer();
        if(!updateMediaBuffer())   //���û�л�ȡ����Դ����ʾû������Դ
        {
            msleep(m_DefDelay);
            continue;
        }
        IDE_TRACE_STR(m_CurCmdString);
        system(m_CurCmdString.toStdString().c_str());
        //pauseBackgroundPlayer();
    }
}

bool Mplayer::updateMediaBuffer()
{

    if(m_CurMedia)  //>@�Ѿ������ֲ����ˣ����Ѿ����Ž���������Ҫ���ݴ����ֵ�ѭ�����Խ��в���
    {
        if(m_BufferLoop == LOOP_LIST)
        {
            int count = m_MediaBuffer.count();
            if(count == 0)
                m_BufferIndex = -1;
            else if(m_BufferIndex == (count-1))
                m_BufferIndex = 0;
            else
                m_BufferIndex++;
        }
        else
        {
            /// IDE_TRACE();
            delete m_CurMedia;
            m_CurMedia = 0;
        }
    }
    if(m_MediaBuffer.isEmpty())
    {
        //IDE_DEBUG(QString(" Media buffer is empty!"));
        return false;
    }
    //��ȡ������Դ
    if(m_BufferLoop == LOOP_LIST)
    {
        if(m_BufferIndex < 0)
            m_BufferIndex = 0;
        m_CurMedia = m_MediaBuffer.at(m_BufferIndex);
    }
    else
    {
        m_CurMedia = m_MediaBuffer.takeFirst();
    }

    if(!m_CurMedia)
    {
        IDE_DEBUG(QString("Current media is invalid!"));
        return false;
    }
    //���²���ָ��
    //    if(m_CurMedia->mVolume >= 0)
    //        m_Volume[0] = m_CurMedia->mVolume;
    if(!QFile::exists(m_CurMedia->mPath))
    {
        IDE_TRACE_STR(m_CurMedia->mPath);
        return false;
    }
    ///m_CurCmdString = QString("loadfile %1 -volume %2 1\n").arg(m_CurMedia->mPath).arg(m_Volume[0]);
    m_CurCmdString = QString("mplayer -quiet -ao alsa -volume %1 %2 > /dev/null\n").arg(m_CurMedia->mVolume).arg(m_CurMedia->mPath);

    //    QString volumeCmd;
    //    volumeCmd = QString("volume %1 1\n").arg(m_Volume[0]);
    //    write(mainPlayer_fd, volumeCmd.toStdString().c_str(), volumeCmd.size());
    //    IDE_DEBUG(volumeCmd);
    ///IDE_DEBUG(m_CurCmdString);
    if(m_CurCmdString.isEmpty())
    {
        IDE_DEBUG(QString("Current command is invalid!"));
        return false;
    }
    return true;
}

bool Mplayer::addMediaToBuffer(PLAY_MEDIA_INFO *pMeida, bool pPrior)
{
    if(!pMeida)
        return false;

    int tmpMediacount = m_MediaBuffer.count();
    if(tmpMediacount > D_MAXBUFFERSIZE)
    {
        IDE_DEBUG("Media buffer is overload!");
        clearMediaBuffer();
    }

    if(pPrior)
    {
        m_MediaBuffer.push_front(pMeida);
    }
    else
    {
        ///���������Ǹ�ѭ�����ŵ���Ƶ������Ҫȷ��֮ǰ��һ����Ƶ�Ƿ�ҲΪ��ͬ��ѭ����Ƶ��
        ///����������������ڵ���ͬѭ������
        if(pMeida->mLoop == LOOP_EVER)
        {
            PLAY_MEDIA_INFO *tmpMeida = 0;
            if(!m_MediaBuffer.isEmpty())
                tmpMeida = m_MediaBuffer.last();
            else
                tmpMeida = m_CurMedia;
            if(tmpMeida && (tmpMeida->mLoop == LOOP_EVER) && (!tmpMeida->mPath.compare(pMeida->mPath)))
                return false;
            m_MediaBuffer.push_back(pMeida);
        }
        else
        {
            m_MediaBuffer.push_back(pMeida);
        }
    }
    ///��������Ƶ����ʱ�������ǰ���ŵ���ƵΪѭ�����ţ����˳�ѭ������ģʽ���Ա��ڲ�����һ��

    ///IDE_DEBUG(QString("Media buffer size is %1 !").arg(m_MediaBuffer.count()));
    return true;
}

bool Mplayer::clearMediaBuffer()
{
    IDE_DEBUG(QString("Stop and clear Media buffer!"));
    PLAY_MEDIA_INFO *tmpMedia = 0;
    foreach(tmpMedia, m_MediaBuffer)
    {
        if(tmpMedia)
            delete tmpMedia;
    }
    m_MediaBuffer.clear();
    if(m_CurMedia)
        m_CurMedia->mLoop = LOOP_NONE;
    return true;
}

void Mplayer::pauseBackgroundPlayer()
{
    if(m_backgroundPlayerAlive && !m_backgroundPlayerPaused)
    {
        m_backgroundPlayerPaused = true;
        write(backgroundPlayer_fd, PAUSE_PLAYER, strlen(PAUSE_PLAYER));
    }
}

void Mplayer::playBackgroundPlayer()
{
    if(m_backgroundPlayerAlive && m_backgroundPlayerPaused)
    {
        m_backgroundPlayerPaused = false;
        write(backgroundPlayer_fd, PAUSE_PLAYER, strlen(PAUSE_PLAYER));
    }
}

void Mplayer::setBackgroundPlayerVolume(int volume)
{
    ///IDE_TRACE();
    if(volume < 0 || volume > 100)
        return;
    m_backgroundVolume = volume;
    //if(m_backgroundPlayerAlive ){
    QString volumeCmd = QString("volume %1 1\n").arg(volume);
    ///IDE_DEBUG(volumeCmd);
    write(backgroundPlayer_fd, volumeCmd.toStdString().c_str(), volumeCmd.size());
    //}
}

void Mplayer::delayTimeMs(quint32 pTimeout)
{
    quint32 i = 0;
    while(i < pTimeout )
    {
        msleep(10);
        i += 10;
    }
}

void Mplayer::backgroundVolumeKeeper()
{
    //    m_volumeTest +=1;
    //    if(m_volumeTest >= 85)
    //        m_volumeTest = 0;
    //    IDE_TRACE_INT(m_volumeTest);
    //    PLAY_MEDIA_INFO *tmpMeida;
    //    tmpMeida = new PLAY_MEDIA_INFO;
    //    tmpMeida->mType = (AO_TYPE)AO_BEEP;
    //    tmpMeida->mPath = QString("/opt/media/M%1.wav").arg(m_volumeTest);
    //    //tmpMeida->mPath = QString("/opt/media/M0.mp3");
    //    tmpMeida->mLoop = LOOP_NONE;
    //    tmpMeida->mVolume = m_volumeTest/2;
    //    tmpMeida->mEmitSig = false;
    //    addMediaToBuffer(tmpMeida, true);

    setBackgroundPlayerVolume(m_backgroundVolume);
}

bool Mplayer::addMedia(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume)
{
    if(QFile::exists(pSrc))
    {
        //IDE_TRACE_INT(pVolume);
        PLAY_MEDIA_INFO *tmpMeida = new PLAY_MEDIA_INFO;
        tmpMeida->mType = (AO_TYPE)pAoType;
        tmpMeida->mPath = pSrc;
        tmpMeida->mLoop = pLoop;
        tmpMeida->mVolume = pVolume;
        //tmpMeida->mEmitSig = pEmitSig;
        if(addMediaToBuffer(tmpMeida, pPrior))
            return true;
        delete tmpMeida;
    }
    return false;
}


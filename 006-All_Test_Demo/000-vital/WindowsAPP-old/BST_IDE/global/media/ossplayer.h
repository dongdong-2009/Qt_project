#ifndef OSSPLAYER_H
#define OSSPLAYER_H

#include "playerbase.h"

#ifdef LINUX
#include <sys/soundcard.h>
#include <sys/poll.h>
#endif
#if (defined(UBUNTU) || defined(LINUX))
#include <signal.h>
#include <sys/types.h>
#endif
#include <math.h>

class MplayerThread;

#define D_MONITORDELAY  500   //>@��ʱ���100ms
#define D_PROCESSWAIT   200   //>@�ȴ�

#define SPU_ANALOG_VOLUME		32

/*
����׼��
    1�����ֲ���ģʽ������ѭ��������ѭ��n�Ρ�˳�򲥷š����Ȳ���
    2���������Ȳ��Ż��Ƿ����ȣ������뽫��ǰ���ֲ�������ڲ�����һ��
    3��������µ���Ƶ���������������Ȳ��ŵģ��򽫵�ǰ���ֵĲ���ģʽǿ������Ϊ˳�򲥷�
*/
class ossplayer : public QThread
{
    Q_OBJECT
public:
    ossplayer(QObject *parent = 0);
    ~ossplayer();

    void ForceQuit() {m_Quit = true;}
    void Wait(int pTimeout);
    void run();

    void SetPlayer(QString pExeFile) {}

    void SetEnable(bool pEnable) {m_Enable = pEnable;}

    void SetTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 GetTrackNum() {return m_TrackNum;}

    //>@
    bool IsRunning();

    bool OpenPlayer();
    bool ClosePlayer();

    //>@
    void SetLoop(LOOP_TYPE pLoopType) {m_BufferLoop = pLoopType;}
    bool AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    PLAY_MEDIA_INFO *GetMedia();
    int  GetMediaBufferSize();
    bool FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall=false);
    bool CanPlay(QString pFile);
    bool UpdateMedia();   //>@ÿ����Ƶ���������Ҫִ�д˺��������ڴӻ�������ȡ��������Դ

    //>@��Ƶ����
    void AudioFinished();   //>@�û��ָ���Ƶ�ֳ�

    void SetMixerDriver(QString pDriver); //>@"/dev/dsp"  "/dev/mixer"
    void SetAoDriver(QString pDriver);

    void SetConfFile(QString pConf) {}
    void SetVoDriver(QString pDriver) {}
    void SetRect(QRect pRect, int pRotate = 0) {}

    void SetVolume(int pVolume);
    void SetVolume(int pAoType, int pVolume, bool pSync=false);
    void Abort();  //>@ָ��ֹ��ǰ��Ƶ������������һ��
    void Seek(int pMs);
    int  GetPlayPercent();
    bool Pause(bool pEnable);
    bool isPause() {return m_Pause;}
    bool Clear();
    bool Stop(bool pEnable, int pType = 0);  //>@ָֹͣ��������������������

    bool TestMedia(int pAoType, QString pSrc, int pVolume);  //>@������Ƶ����

    bool Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig=false);
    void Playback(LOOP_TYPE pLoop=LOOP_NONE);  //>@����pLoop���²��ŵ�ǰ��Ƶ
    void StopOne(QString pFile);
    void PlayNext();
    void PlayHead();
    void PlayEnd();

signals:
    void sNull();   //>@ֻ���ڵ�ǰ������ȫ��������󣬲ŷ������źš�
    void sStarted(); //>@������Ƶ���ſ�ʼ����ʱ����
    void sFinished();   //>@������Ƶ������Ϻ���

public:
    bool                       m_Enable;
    quint32                    m_TrackNum;  //>@

    int                        m_DefDelay;
    bool                       m_Quit;
    bool                       m_AbortAudio;  //>@������ֹ��ǰ���ŵ����֣��̶�������һ��

    AO_MODE                    m_AoMode; //>@

    //>@���ڱ�������ʹ��Ƶ�жϵ��ж�Դ�����������ж�Դ��ɾ����ſ��������ָ���Ƶ��
    QList<int>                 m_StopBuffers;

    //>@���Ż������
    int                        m_BufferIndex;
    LOOP_TYPE                  m_BufferLoop;

    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;
    PLAY_MEDIA_INFO*           m_CurMedia;

    //>@����������
    int                        m_PlayerFd;
    int                        m_MixerFd;
    QFile                      m_AudioFile;

    int                        m_PlayPos;  //>@ms
    bool                       m_Pause;

    QString                    m_MixerDriver; //>@

    QString                    m_AoDriver; //>
    int                        m_Volume[2]; //>@0:��ʾ��ǰ������1�����ڱ��浱ǰϵͳ���õ�����

    quint64                    m_ElapseTime;
};

#endif // OSSPLAYER_H

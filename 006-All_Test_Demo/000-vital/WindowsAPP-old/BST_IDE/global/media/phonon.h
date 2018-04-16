#ifndef PHONON_H
#define PHONON_H

#include "playerbase.h"
#include <QtMultimedia>

class phonon : public QThread
{
    Q_OBJECT
public:
    phonon(QObject *parent = 0);
    ~phonon();

    void ForceQuit() {m_Quit = true;}
    void Wait(int pTimeout);
    void run();

    void SetPlayer(QString pExe) {}

    void SetEnable(bool pEnable) {m_Enable = pEnable;}
    void SetMode(MD_MEDIA pMode) {m_MediaMode = pMode;}

    void SetTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 GetTrackNum() {return m_TrackNum;}

    bool IsRunning();

    //>@����������
    void SetLoop(LOOP_TYPE pLoopType) {m_BufferLoop = pLoopType;}
    bool AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    PLAY_MEDIA_INFO *GetMedia();
    int  GetMediaBufferSize();
    bool FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall=false);
    bool CanPlay(QString pFile);
    bool UpdateMedia();   //>@ÿ����Ƶ���������Ҫִ�д˺��������ڴӻ�������ȡ��������Դ

    void SetConfFile(QString pFile);
    void SetMixerDriver(QString pDriver);
    void SetVoDriver(QString pDriver);
    void SetAoDriver(QString pDriver);

    void AudioFinished();  //>@�û��ָ���Ƶ�ֳ�

    void SetRect(QRect pRect, int pRotate = 0);
    void SetVolume(int pAoType, int pVolume, bool pSync=false);
    void SetBalance(int pBalance);

    void Abort();  //>@ָ��ֹ��ǰ��Ƶ������������һ��
    void Restore() {}
    bool Clear();
    bool Stop(bool pEnable, int pType = 0);  //>@ָֹͣ������

    void Seek(int pMs);
    int  GetPlayPercent();

    bool Pause(bool pEnable);
    bool isPause() {return m_Pause;}

    bool Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig=false);
    void Playback(LOOP_TYPE pLoop=LOOP_NONE);  //>@����pLoop���²��ŵ�ǰ��Ƶ
    void PlayNext();
    void PlayHead();
    void PlayEnd();
    void StopOne(QString pFile);
    bool TestMedia(int pAoType, QString pSrc, int pVolume);  //>@������Ƶ����

signals:
    void sNull();   //>@ֻ���ڵ�ǰ������ȫ��������󣬲ŷ������źš�
    void sKilled(); //>@ͨ��kill��ʽɱ��mplayerʱ����
    void sStarted(); //>@������Ƶ���ſ�ʼ����ʱ����
    void sFinished();   //>@������Ƶ������Ϻ���
    void sAborted();   //>@��Ƶ��ֹͣʱ����
    void sStopped();  //>@������ֹͣʱ����

public slots:
    void slot_StateChanged(QAudio::State pState);

public:
    bool                       m_Enable;
    quint32                    m_TrackNum;  //>@�������
    quint32                    m_KillCount;

    MD_MEDIA                   m_MediaMode;

    //>@�̲߳���
    int                        m_DefDelay;
    bool                       m_Quit;
    bool                       m_AbortAudio;    //>@������ֹ��ǰ���ŵ����֣��̶�������һ��

    //>@���Ż������
    int                        m_BufferIndex;
    LOOP_TYPE                  m_BufferLoop;

    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;
    PLAY_MEDIA_INFO*           m_CurMedia;

    //>@���ڱ�������ʹ��Ƶ�жϵ��ж�Դ�����������ж�Դ��ɾ����ſ��������ָ���Ƶ��
    QList<int>                 m_StopBuffers;

    //>@����������
    bool                       m_Pause;
    int                        m_VoRotate;
    int                        m_Volume[2];   //>@0:��ʾ��ǰ������1�����ڱ��浱ǰϵͳ���õ�����

    QAudioOutput              *m_Player;
    QAudioFormat               m_PlayFormat;

    QAudioInput               *m_Record;
    QAudioFormat               m_RecoFormat;
    WAVContainer_t             m_WavHeader;
};

#endif // PHONON_H

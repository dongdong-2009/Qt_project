#ifndef BASEMEDIA_H
#define BASEMEDIA_H

#include "global.h"

#ifdef MPLAYER
    #include "mplayer.h"
    typedef mplayer MEDIA_PLAYER;
#endif
#ifdef PHONON
    #include "phonon.h"
    typedef phonon MEDIA_PLAYER;
#endif
#ifdef GSTREAMER
    #include "gstreamer.h"
    typedef gstreamer MEDIA_PLAYER;
#endif
#ifdef OSS
    #include "ossplayer.h"
    typedef ossplayer MEDIA_PLAYER;
#endif
#ifdef QPLAYER
    #include "qplayer.h"
    typedef qplayer MEDIA_PLAYER;
#endif

inline void ClosePlayer()
{
#ifdef MPLAYER
    #if (defined(UBUNTU) || defined(LINUX))
        SYSTEM_CMD::KillProcess("mplayer");
    #else
        SYSTEM_CMD::KillProcess("mplayer.exe");
    #endif
#endif
}

#define D_REPEAT_EN         1
#define D_SOLOTRACK_EN      2
#define D_VOLUME_NUM        3

class basemedia : public QObject
{
    Q_OBJECT
public:
    basemedia(QObject *parent = 0);
    ~basemedia();

    void ClearPlayers();

    MEDIA_PLAYER *AddPlayer(int pID, QString pConfFile);
    MEDIA_PLAYER *AddPlayer(int pID, QString pVoDriver, QRect pVoRect, int pVoRotate, QString pAoDriver, int pVolume);
    MEDIA_PLAYER *AddPlayer(int pID, QString pAoDriver, int pVolume);

    MEDIA_PLAYER *GetPlayer(int pID);

signals:
    void sFinished(QObject *pPlayer);

public:
    QHash<int, MEDIA_PLAYER*>        m_PlayerList;
    QSignalMapper                   *m_PlayerMapper;
};

/*
    RIFF WAVE Chun 12
    ==================================
    |       |��ռ�ֽ���|  ��������   |
    ==================================
    | ID    |  4 Bytes |   'RIFF'    |
    ----------------------------------
    | Size  |  4 Bytes |             |
    ----------------------------------
    | Type  |  4 Bytes |   'WAVE'    |
    ----------------------------------
    */
    /*
    Format Chunk 26
    ====================================================================
    |               |   �ֽ���  |              ��������                |
    ====================================================================
    | ID            |  4 Bytes  |   'fmt '                             |
    --------------------------------------------------------------------
    | Size          |  4 Bytes  | ��ֵΪ16��18��18������ָ�����Ϣ     |
    --------------------------------------------------------------------  ----
    | FormatTag     |  2 Bytes  | ���뷽ʽ��һ��Ϊ0x0001               |     |
    --------------------------------------------------------------------     |
    | Channels      |  2 Bytes  | ������Ŀ��1--��������2--˫����       |     |
    --------------------------------------------------------------------     |
    | SamplesPerSec |  4 Bytes  | ����Ƶ��                             |     |
    --------------------------------------------------------------------     |
    | AvgBytesPerSec|  4 Bytes  | ÿ�������ֽ���                       |     |===> WAVE_FORMAT
    --------------------------------------------------------------------     |
    | BlockAlign    |  2 Bytes  | ���ݿ���뵥λ(ÿ��������Ҫ���ֽ���) |     |
    --------------------------------------------------------------------     |
    | BitsPerSample |  2 Bytes  | ÿ��������Ҫ��bit��                  |     |
    --------------------------------------------------------------------     |
    |         bsp;    |  2 Bytes  | ������Ϣ����ѡ��ͨ��Size���ж����ޣ� |     |
    --------------------------------------------------------------------  ----
    */
    /*
    Fact Chunk (��ѡ)
    ==================================
    |       |��ռ�ֽ���|  ��������   |
    ==================================
    | ID    |  4 Bytes |   'fact'    |
    ----------------------------------
    | Size  |  4 Bytes |   ��ֵΪ4   |
    ----------------------------------
    | data  |  4 Bytes |             |
    ----------------------------------
    */
    /*
    Data Chunk 8
    ==================================
    |       |��ռ�ֽ���|  ��������   |
    ==================================
    | ID    |  4 Bytes |   'data'    |
    ----------------------------------
    | Size  |  4 Bytes |             |
    ----------------------------------
    | data  |          |             |
    ----------------------------------
*/

#endif // BASEAUDIO_H

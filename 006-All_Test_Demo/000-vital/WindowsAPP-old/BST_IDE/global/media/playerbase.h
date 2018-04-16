#ifndef PLAYBASE_H
#define PLAYBASE_H

#include "global.h"
#include "wav_parser.h"
#include "mp3_parser.h"

typedef enum{MD_PLAYBACK=0,
             MD_RECORD
} MD_MEDIA;

typedef enum{AO_LIFTFLR=0,
             AO_LIFTARV=1,
             AO_BEEP=2,
             AO_VIDEO=3,
             //>@
             AO_ADMUSIC=4,
             AO_APSRC,      //>@��Ƶ�Խ��ɼ�������
             AO_APDST,      //>@��Ƶ�Խ����ղ���ʾ
             AO_VPSRC,      //>@��Ƶ�Խ��ɼ�������
             AO_VPDST,      //>@��Ƶ�Խ����ղ���ʾ
             AO_NONE
} AO_TYPE;

typedef struct _PLAY_MEDIA_INFO
{
    _PLAY_MEDIA_INFO()
    {
        mType = AO_NONE;
        mLoop = 0;
        mVolume = -1;
        mEmitSig = false;
    }
    AO_TYPE         mType;
    QString         mPath;
    QString         mPara;
    int             mLoop;  //>@-1 ��ʾ����ѭ��
    int             mVolume;  //>@Ϊ-1ʱ��ʾʹ��ʹ�õ�ǰ��Ĭ������
    bool            mEmitSig; //>@��ʾ�Ƿ��Ϳ�ʼ���š��������š�ֹͣ�����ź�
} PLAY_MEDIA_INFO;

typedef enum{
    LOOP_EVER = -1,
    LOOP_NONE = 0,
    LOOP_LIST = 1
} LOOP_TYPE;

class AO_PARA
{
public:
    AO_PARA() {mEnable[0] = mEnable[1] = false; mScaler = 1;}

    int GetVolume() { return mVolume.mNum * mScaler; }

    NUM_INFO mVolume;
    qreal    mScaler;  //>@��Ƶ���Ų�����
    bool     mEnable[2]; //>@����ĳ������Ƶ�Ƿ��ֹ���� 0:��ʾ��Ч�ģ�1����ʾ���ݵġ�
};


#define AOTRACK_MAXNUM      2
#define AOTYPE_SPLIT        QString(',')
#define AOTYPECONF_SPLIT    QString('@')

#define D_HALFBUFFERSIZE  4
#define D_MAXBUFFERSIZE   50

#define AO_MAXNUM 5

//#define D_CMDCONTROL        //>@ʹ�ÿ���ʹ�������н��п�����Ƶ

inline uint qHash(AO_TYPE pType) {  return qHash((int)pType); }

inline AO_TYPE GetAoType(QString pType)
{
    if(pType.isEmpty())
        return AO_NONE;
    if(!pType.compare("LiftFlr", Qt::CaseInsensitive))
        return AO_LIFTFLR;
    else if(!pType.compare("LiftArv", Qt::CaseInsensitive))
        return AO_LIFTARV;
    else if(!pType.compare("Beep", Qt::CaseInsensitive))
        return AO_BEEP;
    else if(!pType.compare("AdMusic", Qt::CaseInsensitive))
        return AO_ADMUSIC;
    else if(!pType.compare("Video", Qt::CaseInsensitive))
        return AO_VIDEO;
    else if(!pType.compare("APSrc", Qt::CaseInsensitive))
        return AO_APSRC;
    else if(!pType.compare("APDst", Qt::CaseInsensitive))
        return AO_APDST;
    else if(!pType.compare("VPSrc", Qt::CaseInsensitive))
        return AO_VPSRC;
    else if(!pType.compare("VPDst", Qt::CaseInsensitive))
        return AO_VPDST;
    return AO_NONE;
}

inline QString GetAoString(AO_TYPE pType)
{
    if(pType == AO_NONE)
        return QString();
    if(pType == AO_LIFTFLR)
        return QString("LiftFlr");
    else if(pType == AO_LIFTARV)
        return QString("LiftArv");
    else if(pType == AO_BEEP)
        return QString("Beep");
    else if(pType == AO_ADMUSIC)
        return QString("AdMusic");
    else if(pType == AO_VIDEO)
        return QString("Video");
    else if(pType == AO_APSRC)
        return QString("APSrc");
    else if(pType == AO_APDST)
        return QString("APDst");
    else if(pType == AO_VPSRC)
        return QString("VPSrc");
    else if(pType == AO_VPDST)
        return QString("VPDst");
    return QString();
}

typedef enum{ AO_IDEL=0,
              AO_STARTING,
              AO_PLAYING,
              AO_PAUSE,
              AO_ABORT,
              AO_RCERROR,
              AO_DRVERROR,
              AO_FMTERROR,
              AO_MEMERROR,
              AO_FINISHED,
              AO_UNKNOWERROR} AO_MODE;

#endif // CMDPLAYER_H

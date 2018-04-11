#include "ossplayer.h"

ossplayer::ossplayer(QObject *parent) :
    QThread(parent)
{
    m_DefDelay = 100;
    m_Quit = false;

    m_PlayerFd = m_MixerFd = 0;
    m_CurMedia = 0;
    m_BufferIndex = -1;
    m_BufferLoop = LOOP_NONE;
    m_Pause = false;
    m_Enable = false;
    m_PlayPos = 0;
    m_Volume[0] = m_Volume[1] = 0;

    m_StopBuffers.clear();

    AudioFinished();
    start(LowestPriority);
}

ossplayer::~ossplayer()
{
    ForceQuit();
    wait(1000);
    Clear();
}

void ossplayer::Wait(int pTimeout)
{
    quint32 i=0;
    while((i+=10) < pTimeout && !m_Quit)
        msleep(10);
}

void ossplayer::run()
{
    while(!m_Quit)
    {
        if(!m_Enable || m_Pause || !m_StopBuffers.isEmpty())
        {
            Wait(m_DefDelay);
            continue;
        }

        //>@��ȡ��������������Դ
        if(!UpdateMedia())  //>@���û�л�ȡ����Դ����ʾû������Դ
        {
            Wait(m_DefDelay);
            continue;
        }

        //>@����Ƶ����
        if(!m_PlayerFd || !m_MixerFd)
        {
            if(!OpenPlayer())
            {
                IDE_DEBUG(QString("Track[%1] : open driver error!").arg(m_TrackNum));
                m_AoMode = AO_DRVERROR;
                Wait(m_DefDelay);
                continue;
            }
        }

        //>@�ָ���Ƶ�ֳ�
        AudioFinished();

        //>@����Ƶ
        if(m_AudioFile.isOpen())
            m_AudioFile.close();
        m_AudioFile.setFileName(m_CurMedia->mPath);
        if(!m_AudioFile.open(QIODevice::ReadOnly))
        {
            IDE_DEBUG(QString("Track[%1] : open %2 error!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            m_AoMode = AO_RCERROR;
            Wait(m_DefDelay);
            continue;
        }
        IDE_TRACE_STR(m_CurMedia->mPath);

        //>@��ȡ����
        int tmpWavHeaderSize = sizeof(WAVContainer_t);
        //IDE_TRACE_INT(tmpWavHeaderSize);
        WAVContainer_t *tmpAudioInfo = (WAVContainer_t*)malloc(tmpWavHeaderSize);
        if(WAV_ReadHeader(m_AudioFile.handle(), tmpAudioInfo) == -1)
        {
            m_AoMode = AO_FMTERROR;
            IDE_DEBUG(QString("Track[%1] : %2 is invalid wav file!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            free(tmpAudioInfo);
            m_AudioFile.close();
            Wait(m_DefDelay);
            continue;
        }
        WAV_P_PrintHeader(tmpAudioInfo);

        ioctl(m_PlayerFd, SNDCTL_DSP_SETFMT, &tmpAudioInfo->format.format);     //AFMT_S16_LE;/*standard 16bit little endian format, support this format only*/
        ioctl(m_PlayerFd, SNDCTL_DSP_SPEED, &tmpAudioInfo->format.sample_rate);
        //ioctl(m_PlayerFd, SNDCTL_DSP_CHANNELS, &tmpAudioInfo->format.channels);
        //IDE_TRACE_INT(tmpAudioInfo->format.channels);

        //�������� //0~0x1f1f(�ֱ�Ϊ��������);
        if(m_CurMedia->mVolume < 0)
            m_CurMedia->mVolume = m_Volume[0];
        //IDE_TRACE_INT(m_CurMedia->mVolume);
        SetVolume(m_CurMedia->mVolume);

        //>@��ȡ����
        int tmpDspBlkSize;
        ioctl(m_PlayerFd, SNDCTL_DSP_GETBLKSIZE, &tmpDspBlkSize);
        if(tmpDspBlkSize < 1024)
        {
            m_AoMode = AO_UNKNOWERROR;
            IDE_DEBUG(QString("Track[%1] : can't get dsp blk size!").arg(m_TrackNum));
            free(tmpAudioInfo);
            m_AudioFile.close();
            Wait(m_DefDelay);
            continue;
        }
        char *buffer = (char *)malloc(tmpDspBlkSize);
        if(buffer == NULL)
        {
            m_AoMode = AO_MEMERROR;
            IDE_DEBUG(QString("Track[%1] : Can't alloc memory!").arg(m_TrackNum));
            free(tmpAudioInfo);
            m_AudioFile.close();
            Wait(m_DefDelay);
            continue;
        }
        //IDE_TRACE_INT(tmpDspBlkSize);

        int tmpSize = 0;
        //>@��ʼ������Ƶ
        m_AoMode = AO_PLAYING;
        if(m_CurMedia->mEmitSig)
        {
            //IDE_DEBUG(QString("Track[%1] : %2 started!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            emit sStarted();
        }
        //>@����pCM���ݶΣ�ֻҪ����ͷ��Ϣ�е�data�ֶΣ�Ȼ��6���ֽں�������ݼ�ΪPCM����
        m_AudioFile.seek(0);
        QByteArray tmpArray = m_AudioFile.read(1024);
        int index = tmpArray.indexOf("data")+6;
        while(!m_AbortAudio)
        {
            IDE_DEBUG(QString("Track[%1] : playing %2 times!").arg(m_TrackNum).arg(m_CurMedia->mLoop));
            bool ret = m_AudioFile.seek(index);
            while(!m_AudioFile.atEnd() && !m_AbortAudio)
            {
                while(m_Pause && !m_AbortAudio)
                {
                    //IDE_DEBUG(QString("Track[%1] : pause ...").arg(m_TrackNum));
                    m_AoMode = AO_PAUSE;
                    msleep(10);
                }
                m_AoMode = AO_PLAYING;
                if(m_AbortAudio)
                    break;

                memset(buffer, 0, tmpDspBlkSize);
                if((tmpSize = m_AudioFile.read(buffer, tmpDspBlkSize)) <= 0)
                    break;
                //IDE_DEBUG(QString("Track[%1] : playing size %2 ...").arg(m_TrackNum).arg(tmpSize));

                int count = 0;
                audio_buf_info info;
                do{
                    ioctl(m_PlayerFd , SNDCTL_DSP_GETOSPACE , &info);
                    usleep(100);
                    //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                }while((info.bytes < tmpDspBlkSize) && (count++ < 100));

                fd_set writefds;
                struct timeval tv;
                tv.tv_sec       = 0;
                tv.tv_usec      = 0;
                FD_ZERO( &writefds );
                FD_SET( m_PlayerFd , &writefds );
                tv.tv_sec       = 0;
                tv.tv_usec      = 0;
                //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                select( m_PlayerFd + 1 , NULL , &writefds , NULL, &tv );
                //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                if( FD_ISSET( m_PlayerFd, &writefds ))
                {
                    //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                    write(m_PlayerFd, buffer, tmpDspBlkSize);
                }
                usleep(100);
            }

            int bytes;
            ioctl(m_PlayerFd,SNDCTL_DSP_GETODELAY,&bytes);
            int delay = bytes / (tmpAudioInfo->format.sample_rate * 2 * tmpAudioInfo->format.channels);
            //IDE_TRACE_INT(delay);
            sleep(delay);

            IDE_DEBUG(QString("Track[%1] : %2 is end!").arg(m_TrackNum).arg(m_CurMedia->mPath));

            //>@ѭ�����Ų���
            if(m_CurMedia->mLoop == -1)
            {
                usleep(10000);
                continue;
            }
            else if(m_CurMedia->mLoop == 0)
            {
                break;
            }
            else
            {
                usleep(10000);
                m_CurMedia->mLoop--;
            }
        }

        free(buffer);
        free(tmpAudioInfo);
        m_AudioFile.close();

        if(m_AbortAudio)
            m_AoMode = AO_ABORT;
        else
            m_AoMode = AO_FINISHED;
        if(m_CurMedia->mEmitSig)  //>@�ɹ����ͽ����źţ�ʧ�ܷ�����ֹ�ź�
        {
            IDE_DEBUG(QString("Track[%1] : %2 finished!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            emit sFinished();
        }
    }
}

bool ossplayer::AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior)
{
    if(!pMeida)
        return false;

    //>@��������
    //>@�鿴�Ƿ񳬳���󻺳���
    int tmpMediacount = m_MediaBuffer.count();
    if(tmpMediacount > D_MAXBUFFERSIZE)  //>@��������󻺳�ʱ��Ϊ�ǲ�������������ջ�����
    {
        IDE_DEBUG(QString("Track[%1] : Media buffer is overload!").arg(m_TrackNum));
        Clear();
    }

    if(pPrior) //>@������µ�����������
    {
        m_MediaBuffer.push_front(pMeida);
    }
    else
    {
        //>@���������Ǹ�ѭ�����ŵ���Ƶ������Ҫȷ��֮ǰ��һ����Ƶ�Ƿ�ҲΪ��ͬ��ѭ����Ƶ������������������ڵ���ͬѭ������
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

    //>@��������Ƶ����ʱ�������ǰ���ŵ���ƵΪѭ�����ţ����˳�ѭ������ģʽ���Ա��ڲ�����һ��
    if(m_CurMedia)
    {
        if(m_CurMedia->mLoop != LOOP_NONE)
        {
            m_AbortAudio = true;
        }
    }

    IDE_DEBUG(QString("Track[%1] : Media buffer size is %2 !").arg(m_TrackNum).arg(m_MediaBuffer.count()));
    return true;
}

bool ossplayer::UpdateMedia()
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
            delete m_CurMedia;
            m_CurMedia = 0;
        }
    }
    if(m_MediaBuffer.isEmpty())
    {
        //IDE_DEBUG(QString("Track[%1] : Media buffer is empty, emit finish signals!").arg(m_TrackNum));
        emit sNull();
        return false;
    }
    //>@��ȡ������Դ
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
        IDE_DEBUG(QString("Track[%1] : Current media is invalid!").arg(m_TrackNum));
        return false;
    }
    return true;
}

bool ossplayer::OpenPlayer()
{
    ClosePlayer();
    m_PlayerFd = open(m_AoDriver.toLatin1().data(), O_RDWR);
    if(m_PlayerFd < 0)
    {
        IDE_DEBUG("Open dsp error");
        return false;
    }
    m_MixerFd = open(m_MixerDriver.toLatin1().data(), O_RDWR);
    if(m_MixerFd < 0)
    {
        IDE_DEBUG("Open mixer error");
        return false;
    }
    return true;
}

bool ossplayer::ClosePlayer()
{
    if(m_PlayerFd)
    {
        close(m_PlayerFd);
        m_PlayerFd = 0;
    }
    else if(m_MixerFd)
    {
        close(m_MixerFd);
        m_MixerFd = 0;
    }
    return true;
}

bool ossplayer::FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall)
{
    pResultList.clear();
    //>@���ȿ���ǰ��Ƶ��������Ƿ�Ϊ�գ������Ϊ�գ���鿴�����Լ���ǰý�����Ƿ������ǰ��վ�ӣ������ѯ��ǰ�������е��Ƿ�Ϊ��ǰ��վ��
    if(IsRunning() && m_CurMedia && !pAudioFile.compare(m_CurMedia->mPath))
    {
        pResultList.append(m_CurMedia);
        return true;
    }
    bool flag = false;  //>@Ĭ��û���ҵ�
    if(!m_MediaBuffer.isEmpty())
    {
        PLAY_MEDIA_INFO *tmpMedaiInfo = 0;
        foreach(tmpMedaiInfo, m_MediaBuffer)
        {
            if(!tmpMedaiInfo)
                continue;
            if(pAudioFile.compare(tmpMedaiInfo->mPath)==0)
            {
                pResultList.append(tmpMedaiInfo);
                flag = true;
                if(!pFindall)
                    break;
            }
        }
    }
    return flag;
}

//>@��ѯ�Ƿ���Բ���ĳ����Ƶ
bool ossplayer::CanPlay(QString pFile)
{
    QList<PLAY_MEDIA_INFO*> tmpResultList;
    return !FindInMediaBuffer(pFile, tmpResultList);
}

PLAY_MEDIA_INFO *ossplayer::GetMedia()
{
    PLAY_MEDIA_INFO *tmpMedia = m_CurMedia;
    return tmpMedia;
}

int ossplayer::GetMediaBufferSize()
{
    int count = m_MediaBuffer.count();
    return count;
}

bool ossplayer::Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig)
{
    if(QFile::exists(pSrc))
    {
        //IDE_TRACE_INT(pVolume);
        PLAY_MEDIA_INFO *tmpMeida = new PLAY_MEDIA_INFO;
        tmpMeida->mType = (AO_TYPE)pAoType;
        tmpMeida->mPath = pSrc;
        tmpMeida->mLoop = pLoop;
        tmpMeida->mVolume = pVolume;
        tmpMeida->mEmitSig = pEmitSig;
        if(AddMedia(tmpMeida, pPrior))
            return true;
        delete tmpMeida;
    }
    return false;
}

bool ossplayer::TestMedia(int pAoType, QString pSrc, int pVolume)
{
    return Add(pAoType, pSrc, LOOP_NONE, true, pVolume, false);
}

//>@ֹͣĳһ����Ƶ�ļ��Ĳ���
void ossplayer::StopOne(QString pFile)
{
    //>@���ȿ���ǰ��Ƶ��������Ƿ�Ϊ�գ������Ϊ�գ���鿴�����Լ���ǰý�����Ƿ������ǰ��վ�ӣ������ѯ��ǰ�������е��Ƿ�Ϊ��ǰ��վ��
    if(!m_MediaBuffer.isEmpty())
    {
        for(int i=m_MediaBuffer.count()-1;i>=0;i--)
        {
            PLAY_MEDIA_INFO *tmpMedia = m_MediaBuffer.at(i);
            if(tmpMedia && !(pFile.compare(tmpMedia->mPath)))
            {
                m_MediaBuffer.takeAt(i);
                delete tmpMedia;
            }
        }
    }
    if(m_CurMedia && !(pFile.compare(m_CurMedia->mPath)) && IsRunning())
    {
        m_AbortAudio = true;
    }
}

void ossplayer::Playback(LOOP_TYPE pLoop)
{
    if(m_CurMedia)
        m_CurMedia->mLoop = pLoop;
}

void ossplayer::PlayNext()
{
    Stop(true);//>@�رյ�ǰ����
    Stop(false);
}

void ossplayer::PlayHead()
{
    Stop(true);//>@�رյ�ǰ����
    m_BufferIndex = -1;
    Stop(false);
}

void ossplayer::PlayEnd()
{
    Stop(true);//>@�رյ�ǰ����
    int count = m_MediaBuffer.count();
    if(count >= 1)
        m_BufferIndex = count - 2;
    Stop(false);
}

bool ossplayer::IsRunning()
{
    if(m_AoMode == AO_PLAYING)
    {
        IDE_DEBUG(QString("Track[%1] : Running!").arg(m_TrackNum));
        return true;
    }
    IDE_DEBUG(QString("Track[%1] : NoRunning - %2!").arg(m_TrackNum).arg(m_AoMode));
    return false;
}

int ossplayer::GetPlayPercent()
{
    int tmpPercent = -1;
    if(IsRunning())
    {
        if(m_AudioFile.isOpen())
        {
            return (qreal)m_AudioFile.pos() * 100.0 / (qreal)m_AudioFile.size();
        }
    }
    return tmpPercent;
}

void ossplayer::SetMixerDriver(QString pDriver)
{
    m_MixerDriver = pDriver;
}

//>@��������������
void ossplayer::SetAoDriver(QString pDriver)
{
    m_AoDriver = pDriver;
}

void ossplayer::SetVolume(int pVolume)
{
    if(m_MixerFd == NULL)
    {
        IDE_DEBUG(QString("Track[%1] : mixer is invalide!").arg(m_TrackNum));
        return;
    }
#ifdef N329
//    quint32 tmpPcmVol = 0x5050;
//    ioctl(m_MixerFd, MIXER_WRITE(SOUND_MIXER_PCM), &tmpPcmVol);
    //>@SPU_ANALOG_VOLUMEָ����Codec��������ÿ��������ΧΪ0~0x1f
    quint32 tmpCodecVol = 0x1f1f;
    ioctl(m_MixerFd, MIXER_WRITE(SPU_ANALOG_VOLUME), &tmpCodecVol);
    //>@ʹ��127.5*(tan((x/200-0.25) * pi)+1)
    //quint32 tmpSoloVol = 127.5 * (tan(((qreal)pVolume/200 - 0.25)*PI) + 1);
    quint32 tmpSoloVol = (qreal)pVolume * 0.6375;  //>@���ֵΪ64�������
    quint32 tmpVolume = tmpSoloVol + (tmpSoloVol<<8);
    int channels;
    //>@SOUND_MIXER_LINE/SOUND_MIXER_VOLUMEָ����ÿ�������ĵ�������ÿ��������ΧΪ0~0x7f
    if(!m_MixerDriver.compare("/dev/mixer", Qt::CaseInsensitive))
    {
        channels = 0;
        ioctl(m_MixerFd, MIXER_WRITE(SOUND_MIXER_VOLUME), &tmpVolume);  //>@ִ�к󣬻ὫtmpVolume���㡣
        ioctl(m_PlayerFd, SNDCTL_DSP_CHANNELS, channels);
    }
    else// if(!m_MixerDriver.compare("/dev/mixer2", Qt::CaseInsensitive))
    {
        channels = 2;
        ioctl(m_MixerFd, MIXER_WRITE(SOUND_MIXER_VOLUME), &tmpVolume);  //>@ִ�к󣬻ὫtmpVolume���㡣
        ioctl(m_PlayerFd, SNDCTL_DSP_CHANNELS, channels);
    }
    IDE_DEBUG(QString("Track[%1] : set volume %2, realvalue = %3!").arg(m_TrackNum).arg(pVolume).arg(tmpSoloVol));
#endif
}

void ossplayer::SetVolume(int pAoType, int pVolume, bool pSync) //>@pSync��ʾ�Ƿ�ͬ���޸�ϵͳ���������ֵΪ0x1f1f
{
    if(pVolume < 0)
        m_Volume[0] = 0;
    else if(pVolume > 100)
        m_Volume[0] = 100;
    else
        m_Volume[0] = pVolume;
    if(pSync)
        m_Volume[1] = m_Volume[0];
    //>@ͬ�����õ�ǰ���л������е��������������ֵ���һ��������������ǰ����Ļ����������ǰ���֮ǰ���õ��������ŵ�bug
    for(int i=0;i<m_MediaBuffer.count();i++)
    {
        PLAY_MEDIA_INFO *tmpInfo = m_MediaBuffer.at(i);
        if(tmpInfo && ((pAoType == AO_NONE) || (pAoType == tmpInfo->mType)))  //>@��pAoTypeΪNone��������Ƶ������ͬʱ�Ž��и���
            tmpInfo->mVolume = m_Volume[0];
    }
    if(m_CurMedia && ((pAoType == AO_NONE) || (pAoType == m_CurMedia->mType)))
    {
        m_CurMedia->mVolume = m_Volume[0];
    }
    SetVolume(m_Volume[0]);
}

void ossplayer::Seek(int pMs)
{
    if(m_Pause)
        return;
}

bool ossplayer::Pause(bool pEnable)
{
    SetEnable(!pEnable);
    m_Pause = pEnable;
    return true;
}

void ossplayer::Abort()  //>@ֻ���߳���ʹ��
{
    m_AbortAudio = true;
}

bool ossplayer::Stop(bool pEnable, int pType)
{
    if(pEnable)
    {
        if(!m_StopBuffers.contains(pType))
            m_StopBuffers.append(pType);
        SetEnable(false);
        Abort();
    }
    else
    {
        m_StopBuffers.removeOne(pType);
        //IDE_TRACE_INT(m_TrackNum);
        SetEnable(true);
    }
    return true;
}

//>@Clearִ�к�Ὣ�µ���Ƶ����������ȥ����˲���Ҫִ��slot_finished.
bool ossplayer::Clear()
{
    IDE_DEBUG(QString("Track[%1] : Stop and clear Media buffer!").arg(m_TrackNum));
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

void ossplayer::AudioFinished()
{
    m_AoMode = AO_IDEL;
    m_AbortAudio = false;
    m_Pause = false;
}


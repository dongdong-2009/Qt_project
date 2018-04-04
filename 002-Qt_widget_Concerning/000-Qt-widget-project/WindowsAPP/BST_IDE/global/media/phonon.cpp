#include "phonon.h"

phonon::phonon(QObject *parent) :
    QThread(parent)
{
    m_DefDelay = 100;
    m_Quit = false;

    m_CurMedia = 0;
    m_BufferIndex = -1;
    m_BufferLoop = LOOP_NONE;
    m_Pause = false;
    m_Enable = false;
    m_Volume[0] = m_Volume[1] = 0;

    m_StopBuffers.clear();

    m_AbortAudio = false;
    m_MediaMode = MD_PLAYBACK;
    m_VoRotate = 0;

    m_Player = 0;
    m_Record = 0;

    AudioFinished();
    start(LowestPriority);
}

phonon::~phonon()
{
    ForceQuit();
    wait(1000);
    Clear();
}

void phonon::Wait(int pTimeout)
{
    quint32 i=0;
    while((i+=10) < pTimeout && !m_Quit)
        msleep(10);   //>@ʹ��PHONON����ʱ���˴�����ʹ��QtSleep��Ŀǰ����֪��ԭ��
}

void phonon::run()
{
    while(!m_Quit)
    {
        if(m_MediaMode == MD_PLAYBACK)
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

            //>@�ָ���Ƶ�ֳ�
            AudioFinished();

            //>@��ʼ������Ƶ
            QFile tmpFile(m_CurMedia->mPath);
            if(!tmpFile.exists() || !tmpFile.open(QIODevice::ReadOnly))
            {
                Wait(m_DefDelay);
                continue;
            }
            QByteArray tempBlock,tempInfoBlock;
            tempBlock = tmpFile.read(1024);  //>@���������ֻ��Ҫ��ǰ38�ֽڼ���
            if(tempBlock.count() < 38)
            {
                Wait(m_DefDelay);
                continue;
            }
            tempInfoBlock = tempBlock.mid(24,4);
            quint32 FrequencyIntHH =  (quint8)tempInfoBlock.at(3);
            quint32 FrequencyIntHL =  (quint8)tempInfoBlock.at(2);
            quint32 FrequencyIntLH =  (quint8)tempInfoBlock.at(1);
            quint32 FrequencyIntLL =  (quint8)tempInfoBlock.at(0);
            quint32 FrequencyInt = (FrequencyIntHH<<24)+(FrequencyIntHL<<16)+(FrequencyIntLH<<8)+FrequencyIntLL;
            IDE_TRACE_INT(FrequencyInt);
            m_PlayFormat.setSampleRate(FrequencyInt);
            tempInfoBlock.clear();

            tempInfoBlock = tempBlock.mid(22,2);
            quint16 ChannelsIntH =  (quint8)tempInfoBlock.at(1);
            quint16 ChannelsIntL =  (quint8)tempInfoBlock.at(0);
            quint16 ChannelsInt = (ChannelsIntH<<8)+ChannelsIntL;
            IDE_TRACE_INT(ChannelsInt);
            m_PlayFormat.setChannelCount(ChannelsInt);
            tempInfoBlock.clear();

            tempInfoBlock = tempBlock.mid(34,2);
            quint16 SampleSizeIntH =  (quint8)tempInfoBlock.at(1);
            quint16 SampleSizeIntL =  (quint8)tempInfoBlock.at(0);
            quint16 SampleSizeInt = (SampleSizeIntH<<8)+SampleSizeIntL;
            IDE_TRACE_INT(SampleSizeInt);
            m_PlayFormat.setSampleSize(SampleSizeInt);
            tempInfoBlock.clear();

            m_PlayFormat.setCodec("audio/pcm");
            m_PlayFormat.setByteOrder(QAudioFormat::LittleEndian);
            m_PlayFormat.setSampleType(QAudioFormat::SignedInt);

            QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
            if (!info.isFormatSupported(m_PlayFormat))
            {
                Wait(m_DefDelay);
                continue;
            }

            if(m_CurMedia->mEmitSig)
                emit sStarted();
            IDE_DEBUG(QString("Track[%1] Playing - %2!").arg(m_TrackNum).arg(m_CurMedia->mPath));

            m_Player = new QAudioOutput(info, m_PlayFormat);
            connect(m_Player, SIGNAL(stateChanged(QAudio::State)), this, SLOT(slot_StateChanged(QAudio::State)));
            m_Player->setBufferSize(51200);

            int index = tempBlock.indexOf("data")+6;
            tempBlock.clear();

            while(!m_AbortAudio)
            {
                if(!tmpFile.seek(index))
                    break;

                IDE_TRACE();
                m_Player->start(&tmpFile);
                if(m_Player->error() == QAudio::NoError)
                {
                    IDE_TRACE();
                    //>@�ȴ������
                    while(!m_AbortAudio && IsRunning())
                    {
                        QtSleep(1);
                    }
                }
                else
                {
                    //>@���²��ŵ�ǰ���������
                    m_MediaBuffer.prepend(m_CurMedia);
                    m_CurMedia = 0;
                    m_Quit = true;
                    break;
                }
                //m_Player->stop();

                if(m_AbortAudio)
                    break;

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

            if(m_Player)
            {
                m_Player->deleteLater();
                m_Player = 0;
            }

            if(m_CurMedia && m_CurMedia->mEmitSig)  //>@�ɹ����ͽ����źţ�ʧ�ܷ�����ֹ�ź�
            {
                IDE_DEBUG(QString("Track[%1] : audio finished!").arg(m_TrackNum));
                emit sFinished();
            }
        }
        else  //>@¼��
        {
            if(!m_Enable)
            {
                Wait(m_DefDelay);
                continue;
            }
            QString tmpPath = D_FSRCPATH + QString("TMP/tmprecord.wav");
            QFile tmpFile(tmpPath);
            if(!tmpFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                Wait(m_DefDelay);
                continue;
            }
            if(!m_RecoFormat.isValid())
            {
                m_RecoFormat.setSampleRate(8000);
                m_RecoFormat.setChannelCount(2);
                m_RecoFormat.setSampleSize(16);
                m_RecoFormat.setCodec("audio/pcm");
                m_RecoFormat.setByteOrder(QAudioFormat::LittleEndian);
                m_RecoFormat.setSampleType(QAudioFormat::SignedInt);
            }
            QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
            if (!info.isFormatSupported(m_RecoFormat))
            {
                IDE_TRACE();
                m_RecoFormat = info.nearestFormat(m_RecoFormat);
            }

            qstrcpy((char*)(m_WavHeader.header.magic), "RIFF");
            qstrcpy((char*)(m_WavHeader.header.type), "WAVE");
            qstrcpy((char*)(m_WavHeader.format.magic), "fmt ");

            if(!WAV_WriteHeader(tmpFile.handle(), &m_WavHeader))
            {
                Wait(m_DefDelay);
                continue;
            }

            m_Record = new QAudioInput(m_RecoFormat);
            m_Record->setBufferSize(51200);
            connect(m_Record, SIGNAL(stateChanged(QAudio::State)), this, SLOT(slot_StateChanged(QAudio::State)));
            m_Record->start(&tmpFile);

            //>@�ȴ������
            while(!m_AbortAudio && (m_Record->state() != QAudio::ActiveState))
            {
                QtSleep(1);
            }

            m_Record->stop();
            tmpFile.close();

            AudioFinished();

            m_Record->deleteLater();
            m_Record = 0;
        }
    }
    Abort();
}

bool phonon::AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior)
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

    //>@�鿴��ǰ�߳��Ƿ�������
    if(!isRunning())
    {
        m_Quit = false;
        start(LowestPriority);
    }

    IDE_DEBUG(QString("Track[%1] : Media buffer size is %2 !").arg(m_TrackNum).arg(m_MediaBuffer.count()));
    return true;
}

bool phonon::UpdateMedia()
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

bool phonon::FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall)
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
bool phonon::CanPlay(QString pFile)
{
    QList<PLAY_MEDIA_INFO*> tmpResultList;
    return !FindInMediaBuffer(pFile, tmpResultList);
}

PLAY_MEDIA_INFO *phonon::GetMedia()
{
    PLAY_MEDIA_INFO *tmpMedia = m_CurMedia;
    return tmpMedia;
}

int phonon::GetMediaBufferSize()
{
    int count = m_MediaBuffer.count();
    return count;
}

bool phonon::Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig)
{
    if(QFile::exists(pSrc))
    {
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

bool phonon::TestMedia(int pAoType, QString pSrc, int pVolume)
{
    return Add(pAoType, pSrc, LOOP_NONE, true, pVolume, false);
}

//>@ֹͣĳһ����Ƶ�ļ��Ĳ���
void phonon::StopOne(QString pFile)
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

void phonon::Playback(LOOP_TYPE pLoop)
{
    if(m_CurMedia)
        m_CurMedia->mLoop = pLoop;
}

bool phonon::IsRunning()
{
    if(m_MediaMode == MD_PLAYBACK)
    {
        if(!m_Player)
            return false;
        return (m_Player->state() == QAudio::ActiveState);
    }
    else
    {
        if(!m_Record)
            return false;
        return (m_Record->state() == QAudio::ActiveState);
    }
    return false;
}

void phonon::SetConfFile(QString pFile)
{
}

void phonon::SetVoDriver(QString pDriver)
{
}

//>@��������������
void phonon::SetAoDriver(QString pDriver)
{
}

void phonon::SetMixerDriver(QString pDriver)
{
}

void phonon::SetRect(QRect pRect, int pRotate)
{
}

void phonon::SetVolume(int pAoType, int pVolume, bool pSync) //>@pSync��ʾ�Ƿ�ͬ���޸�ϵͳ����
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
    //m_Player->setVolume(m_Volume[0]);
}

void phonon::Seek(int pMs)
{
    if(m_Pause)
        return;
    if(!m_Player)
        return;
}

/*1   ������
  -1  ������*/
void phonon::SetBalance(int pBalance)
{
}

bool phonon::Pause(bool pEnable)  //>@m_AudioOutput->resume();
{
    if(!IsRunning())
    {
        m_Pause = false;
        return false;
    }
    m_Pause = pEnable;
    return true;
}

void phonon::Abort()  //>@ֻ���߳���ʹ��
{
    m_AbortAudio = true;
}

void phonon::PlayHead()
{
    Stop(true);//>@�رյ�ǰ����
    m_BufferIndex = -1;
    Stop(false);
}

void phonon::PlayEnd()
{
    Stop(true);//>@�رյ�ǰ����
    int count = m_MediaBuffer.count();
    if(count >= 1)
        m_BufferIndex = count - 2;
    Stop(false);
}

void phonon::PlayNext()
{
    Stop(true);//>@�رյ�ǰ����
    Stop(false);
}

bool phonon::Stop(bool pEnable, int pType)
{
    if(m_MediaMode == MD_PLAYBACK)
    {
        if(pEnable)
        {
            if(!m_StopBuffers.contains(pType))
                m_StopBuffers.append(pType);
            IDE_TRACE();
            SetEnable(false);
            Abort();
        }
        else
        {
            m_StopBuffers.removeOne(pType);
            IDE_TRACE_INT(m_TrackNum);
            SetEnable(true);
        }
    }
    else
    {
        if(pEnable)
        {
            SetEnable(false);
            Abort();
        }
        else
        {
            SetEnable(true);
        }
    }
    return true;
}

//>@Clearִ�к�Ὣ�µ���Ƶ����������ȥ����˲���Ҫִ��slot_finished.
bool phonon::Clear()
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

void phonon::AudioFinished()
{
    m_AbortAudio = false;
    m_Pause = false;
}

void phonon::slot_StateChanged(QAudio::State pState)
{

}

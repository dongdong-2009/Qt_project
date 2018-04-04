#ifndef PHYSICALPLAYER_H
#define PHYSICALPLAYER_H

#include "physicalbase.h"
#include "basemedia.h"

/*
    ������������Ƶ�Ƚ����⣺
        ��Ƶ��������ַ�������ļ�����+ý���ļ���
        ����������һ����ַ��ý���ļ���
    ý���ļ��п����ڿ��ƶ������У�Ҳ�������κ�һ�����Ե�ַ�С�

    ����ھ��Ե�ַ�У���������ʱ����ز�ѭ���б��š�
    ����ڿ��ƶ������У�����Ҫ���ƶ����̲���ʱ�ż��ز�ѭ�����š�
*/

class PhysicalPlayer : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalPlayer(QObject *parent = 0);
    ~PhysicalPlayer();

    void InitPlayer();
    bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);
    bool isTrackEnable(int pId);

    //>@���е�վ��ʱ����ֹ¥���������ŵ�վ��������¥�����Լ���վ�����Ὣ�������Ĵ�С��Ϊ0�����ڽ��������»ָ�������
    bool Play(int pAoType, QString pFile, int pLoop=1, bool pPrior=false, bool pTrack=true);
    void Restore(int pAoType, int pStopType = 0);
    void Restore(int pStopType = 0);
    void Abort(int pAoType, int pStopType = 0);  //pStopType��ʾֹͣ��Ƶ��Դ����, һ��0��ʾEnable�Ȳ�����1��ʾ����ģʽ��2��ʾpre-operationģʽ
    void Abort(int pStopType = 0);
    void SetVolume(int pId, int pVolume);
    void SetScaler(qreal pScaler, bool pSync = true);//>@pSync��ʾ�Ƿ�ֱ��ͬ�������յĲ������
    void SetScaler(int pAoType, qreal pScaler, bool pSync = true);
    int GetVolume(int pAoType);

    void Clear(int pId);

    MEDIA_PLAYER *GetMediaPlayer(int pAoType) { return mAoRelationHash.value(pAoType); }
    QString GetCurMedia(int pId, bool &pRunning);  //>@���ص�ǰ��Ƶ�Ƿ��ڲ���
    quint32 GetCurMedia(QString pAudio);

    quint32 InitPara();
    QString GetInformation() {return QString("%1>").arg(mDevName);}

    bool SetValue(QString pValue);

    void SpotsAdMedia(bool pPrior=false, bool pImmediately=false);

public slots:
    void slot_Play(QString pBeepName);
    void slot_Play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pMultiTrack);
    void slot_Play(int pAoType, QString pFile, int pVolume, int pLoop, bool pPrior, bool pMultiTrack);
    void UpdatePara(QObject* pObject);
    void slot_Initialize();
    void slot_AudioStarted(int pTrack);
    void slot_AudioFinished(int pTrack);

    void slot_MobileDiskChange();

public:
    QString                     mAdAddress;  //>@����ַ
    QStringList                 mAdRcList;

    QSignalMapper              *mStartMapper;
    QSignalMapper              *mFinishMapper;

    QString                     mAoDriver;

    MEDIA_PLAYER*               mPlayerList[AOTRACK_MAXNUM];

    AO_PARA                     mAoParas[AO_MAXNUM];
    QHash<int, MEDIA_PLAYER*>   mAoRelationHash;
    QMultiHash<int, int>        mTrackRelationHash;

    bool                        mSoundOn;

    int                         mAoVolsBk[AO_MAXNUM];
};

#endif // PHYSICALPLAYER_H

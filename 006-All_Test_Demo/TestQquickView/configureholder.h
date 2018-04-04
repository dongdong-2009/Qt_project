#ifndef CONGFIGUREHOLDER_H
#define CONGFIGUREHOLDER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>

#include <QStringList>
#include <QVariant>

class ConfigureHolder : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureHolder(QObject *parent = 0);

    void InitDoc();

signals:
    void copyFileProgress(int pro);
    void resetDefaultClicked();
    void resetCanceled();
    void showPreviewSignal();
    void previewCheckedFalse();
    void startPicAnimation();
    void isFullScreenChanges(bool isfull);
    void horizontalAudioAutoPlay();
    void horizontalFullscreenAudioAutoPlay();
    void verticalAudioAutoPlay();
    void verticalFullscreenAudioAutoPlay();
    void stopAllAudioPlay();
    void startPlayAfterCloseNormal();
    void startPlayAfterCloseFullScreen();
    void checkFullScreen();

public slots:
    bool createXml(QString dir);
    void clearParameters();    
//    void startJobs();

    void setDelDirPath(const QString path);
    bool delDir(const QString path);
    void startDelDir();
    void orderFile(QString dir);
    bool copyReourceFile(QString src, QString dst);
//    bool copyFileMulti(QString src, QString dst);

    QString GetFilePath(QString pFilePath);

//    void copyProgressMult(int id, qint64 copyBytes);
//    void jobFinished(int id);


    bool resetScreen(bool isReset); //�Ƿ������豸Ĭ����Դ����
    void sendCheckedSignal();
    void sendCanceledSignal();
    void sendShowPreview();
    void sendPreviewCheckedFalse();
    void sendStartPicAnimation();
    void sendIsFullScreenSignal(bool isflag);
    void sendCheckFullScreenSignal(bool isflag);
    void sendHorizontalAutoPlay(bool isfull);
    void sendVerticalAutoPlay(bool isfull);
    void sendStopAudioPlay();
    void sendPlayAfterClose(bool isfull);
    void setBrightOrVolumeChanged(int para);
    bool updateMultiMedia(bool isMediaContentEnabled,//true���¶�ý��,false�����¶�ý��
                          bool videoOrPicture, //true��Ƶ,falseͼƬ
                          bool isAudio,
                          bool isfullscreen,//�Ƿ�ȫ��
                          QString paths);//true��Ƶ,falseͼƬ
    QVariant retPicturePath(QString paths);
    bool updateParameterBasic(bool bConfigureParameter,//�Ƿ������˲���
                              bool bBrightnessVolume,//�Ƿ����������Ⱥ�����
                              bool bScrollText,//�Ƿ������˹�����Ļ
                              bool bTitle,//�Ƿ������˱���
                              bool bDateTime,//�Ƿ�������ʱ�����ڸ�ʽ
                              bool bStandby, //�Ƿ������˽���ģʽ
                              bool isScrollText,
                              bool isTitle,
                              bool isDateTime,
                              bool audioSel);

    bool updateParameterBasic1(bool bConfigureParameter,
                               bool bBrightnessVolume,
                               bool bScrollText,
                               bool bTitle,
                               bool isScrollText,
                               bool isTitle,
                               int scrollFlag,
                               int titleFlag);
    bool updateArea1ParaAudio(bool audioOn, bool audioOff);

    bool updateParameterBasic2(bool bConfigureParameter,
                               bool bDateTime,
                               bool bStandby,
                               bool isDateTime,
                               bool audioSel,
                               bool audiooff,
                               int timeFlag,
                               int audioFlag);

    bool updateBrightnessVolume(qint32 vBrightness,//����
                              qint32 vVolume);//����

    int judgeString(QString str);
    bool updateScrollText(QString vScrollText);//������Ļ

    bool updateTitle(QString vTitle);//����

    bool updateDateTime(QString vTimeFormat,QString vDateFormat);//ʱ��,���ڸ�ʽ

    bool updateStandby(quint32 vStage1Interval,//��һ�׶ν���ʱ��
                        quint8 vStage1Brightness,//��һ�׶�����
                        quint32 vStage2Interval,//�ڶ��׶ν���ʱ��
                       quint8 vStage2Brightness);//�ڶ��׶�����
    bool updatePictureInterval(qint32 vPictureInterval);
    int getPicInterval();
    void setSystimeText(QString ptext, bool sysTimeOn);
    void setCountryIndex(int pCountryIndex, int pYysIndex);
    void setApnFlag(bool pApnOn);
    void setApnParameter(QString papnYysText, QString papnJrdText,
                         QString papnMccText, QString papnMncText,
                         QString papnUNameText, QString papnPasswdText);
    void setIpParameter(bool pIsAutoSettingFlag, QString pIpAddressText, QString psubnetText,
                        QString pdefaultGateWay, QString pdnsService);

public:
    ///�Ƿ������豸Ĭ����Դ����
    bool isReset;

    ///��ý������ѡ������
    bool isMediaContentEnabled; //true ���� ���¶�ý�壬false ���� �����¶�ý��
    bool isfullscreen;
    bool videoOrPicture; //true ���� ��Ƶ��false ���� ͼƬ
    bool audioSelect;    // ��Ƶ

    QString videoPath;
    QStringList pictureList;
    QStringList pictureList2;

    ///��������ѡ������
    bool bConfigureParameter;
    bool bBrightnessVolume;
    int mBrightOrVolumeChanged;
    bool bScrollText;
    bool bTitle;
    bool bDateTime;
    bool bStandby;
    bool audioSwither;
    bool audioSwitherOff;
    int audioHasSelect;
    int timeHasSelect;

    //bool bBrightnessVolume;
    bool isScrollText;
    bool isTitle;
    bool isDateTime;
    int scrollHasSelect;
    int titleHasSelect;
    //bool bStandby;

    //bool isBrightnessVolume;
    quint8 vVolume;
    quint8 vBrightness;

    //bool isScrollText;
    QString vScrollText;

    //bool isTitle;
    QString vTitle;

    //bool isDateTime;
    QString vTimeFormat;
    QString vDateFormat;

    //bool isStandby;
    quint32 vStage1Interval;
    quint8 vStage1Brightness;
    quint32 vStage2Interval;
    quint8 vStage2Brightness;    
    qint32 vPicInterval;
//    QMap<int,CopyJob*> jobsMap;
    quint32 appropriateJobNumber;

    bool isMultiThreadCopying;
    qint64 fileTotalBytes;
    qint64 copyedBytes;
    qreal copyPercent;
    qint64 *perJobCopyedBytes;

    qint64 perJobLoad;
    qint64 lastJobLoad;


    QString toWindwosPath(QString path);
    QString usbPath;
    qreal pro;

    QString sysTimeText;
    bool msysTimeOn;
    int apnCountryIndex;
    int apnYysIndex;
    QString apnYysText;
    QString apnJrdText;
    QString apnMccText;
    QString apnMncText;
    QString apnUNameText;
    QString apnPasswdText;
    bool apnOn;
    bool isAutoSettingFlag;
    QString IpAddressText;
    QString subnetText;
    QString defaultGateWay;
    QString dnsService;

};

#endif // CONGFIGUREHOLDER_H

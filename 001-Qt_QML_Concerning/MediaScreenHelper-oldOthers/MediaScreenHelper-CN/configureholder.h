#ifndef CONGFIGUREHOLDER_H
#define CONGFIGUREHOLDER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <windows.h>
#include "copyjob.h"

class ConfigureHolder : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureHolder(QObject *parent = 0);


    void InitDoc();
    void GetAppropriateJobNumber();

signals:
    void copyFileProgress(int pro);
public slots:
    bool createXml(QString dir);
    void clearParameters();    
    void startJobs();

    void orderFile(QString dir);
    bool copyReourceFile(QString src, QString dst);
    bool copyFileMulti(QString src, QString dst);

    QString GetFilePath(QString pFilePath);

    void copyProgressMult(int id, qint64 copyBytes);
    void jobFinished(int id);


    bool resetScreen(bool isReset); //�Ƿ������豸Ĭ����Դ����

    bool updateMultiMedia(bool isMediaContentEnabled,//true���¶�ý��,false�����¶�ý��
                          bool videoOrPicture, //true��Ƶ,falseͼƬ
                          bool isfullscreen,//�Ƿ�ȫ��
                          QString paths);//true��Ƶ,falseͼƬ

    bool updateParameterBasic(bool bConfigureParameter,//�Ƿ������˲���
                              bool bBrightnessVolume,//�Ƿ����������Ⱥ�����
                              bool bScrollText,//�Ƿ������˹�����Ļ
                              bool bTitle,//�Ƿ������˱���
                              bool bDateTime,//�Ƿ�������ʱ�����ڸ�ʽ
                              bool bStandby,
                              bool isScrollText,
                              bool isTitle,
                              bool isDateTime);//�Ƿ������˽���ģʽ

    bool updateBrightnessVolume(qint32 vBrightness,//����
                              qint32 vVolume);//����

    bool updateScrollText(QString vScrollText);//������Ļ

    bool updateTitle(QString vTitle);//����

    bool updateDateTime(QString vTimeFormat,QString vDateFormat);//ʱ��,���ڸ�ʽ

    bool updateStandby(quint32 vStage1Interval,//��һ�׶ν���ʱ��
                        quint8 vStage1Brightness,//��һ�׶�����
                        quint32 vStage2Interval,//�ڶ��׶ν���ʱ��
                       quint8 vStage2Brightness);//�ڶ��׶�����

public:
    ///�Ƿ������豸Ĭ����Դ����
    bool isReset;

    ///��ý������ѡ������
    bool isMediaContentEnabled; //true ���� ���¶�ý�壬false ���� �����¶�ý��
    bool isfullscreen;
    bool videoOrPicture; //true ���� ��Ƶ��false ���� ͼƬ
    QString videoPath;
    QStringList pictureList;

    ///��������ѡ������
    bool bConfigureParameter;
    bool bBrightnessVolume;
    bool bScrollText;
    bool bTitle;
    bool bDateTime;
    bool bStandby;

    //bool bBrightnessVolume;
    bool isScrollText;
    bool isTitle;
    bool isDateTime;
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

    QMap<int,CopyJob*> jobsMap;
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
};

#endif // CONGFIGUREHOLDER_H

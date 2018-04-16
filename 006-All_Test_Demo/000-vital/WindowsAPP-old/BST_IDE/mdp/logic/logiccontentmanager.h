#ifndef LogicContentManager_H
#define LogicContentManager_H

#include "logicbase.h"
#include <QMultiHash>
#ifndef PHONE
#include "modem.h"
#endif
#include "physicaldisk.h"

class GraphicsScene;

/*
  ���ڹ�����������豸�������豸��������
    Ŀ¼��
       BST.cmd
       DISK
         BOOT
         THEME
         SYSTEM
         EXE
       COM
         Dekstop
         Logo
         Video
         Image
         Text
*/

class LogicContentManager : public LogicDevice
{
    Q_OBJECT
public:
    LogicContentManager(QObject *parent = 0);
    ~LogicContentManager();

public:
    quint32 InitPara(); //>@���ش���Ĳ�������

signals:
    void sSetMenu(bool pUpgradeMode);

public slots:
    void UpdatePara(QObject* pObject);
    void DeviceStateChanged();
    void slot_Initialize();

public:
    QPointer<PhysicalUpgrade> mUpgradeDev;
    QPointer<PhysicalLCD>     mLCD;
    QPointer<PhysicalDisk>    mDisk;

    bool                      mBackStage;
    QString                   mSystem;
    QString                   mUserLogo;
    QString                   mApplication;

#ifdef SUPPORT_MUTEX
    QMutex                    *mUpgradeMutex;
#else
    bool                       mUpgradeMutex;
#endif
};

#endif // PARSERUPGRADE_H

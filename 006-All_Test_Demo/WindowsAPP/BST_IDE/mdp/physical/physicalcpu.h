#ifndef PHYSICALCPU_H
#define PHYSICALCPU_H

#include "physicalbase.h"

class PhysicalCPU : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalCPU(QObject *parent = 0);
    ~PhysicalCPU();

    quint32 InitPara();
    QString GetInformation() { return QString("%1>%2  %3%").arg(mDevName).arg(mCpuModel).arg(mUsageRate); }

    bool GotoIdel();
    bool LeaveIdel();

    quint32 Monitor(quint32 pCount);

public slots:
    void UpdatePara(QObject* pObject);

public:
    QString         mCpuModel;
    QString         mUsbScanPath;  //>@USB�豸ɨ��Ŀ¼�����ڿ����״�ɨ���Ѵ��ڵ�USB�豸
    bool            mAutoRelease;
    NUM_INFO        mThreshold;

    int             mUsageRate;

    QFile          *m_File;
    bool            mIsIdelState;
};

#endif // PHYSICALCPU_H

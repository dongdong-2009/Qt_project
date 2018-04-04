#ifndef PHYSICALLCD_H
#define PHYSICALLCD_H

#include "physicalbase.h"
#include "srnsaveform.h"

#ifdef WINDOWS
#include <TChar.h>
class CGammaRamp
{
protected:
    HMODULE hGDI32;
    HDC hScreenDC;
    typedef BOOL (WINAPI *Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);

    Type_SetDeviceGammaRamp pGetDeviceGammaRamp;
    Type_SetDeviceGammaRamp pSetDeviceGammaRamp;

public:

    CGammaRamp();
    ~CGammaRamp();
    BOOL LoadLibrary();
    void FreeLibrary();
    BOOL LoadLibraryIfNeeded();
    BOOL SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
    BOOL GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
    BOOL SetBrightness(HDC hDC, WORD wBrightness);
};
#elif LINUX
#ifdef N329
#define IOCTL_LCD_BRIGHTNESS	_IOW('v', 27, unsigned int)	//brightness control
#endif
#endif

#ifdef DISPLAY
class PhysicalCOM;
#endif

class PhysicalLCD : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalLCD(QObject *parent = 0);
    ~PhysicalLCD();

    quint32 InitPara();
    bool SetValue(QString pValue);
    bool SetLuminance(int pValue);

    quint32 Monitor(quint32 pCount);

    bool Open();
    bool Close();

    void GotoMode(bool pEnable);

    bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);

    bool SetSrnSaveEn(bool pFlag);
    bool SetStandbyEn(bool pFlag);

    void ReCalibrate(bool pFroce = false);

signals:
    void sStandby(bool pFlag);
    void sSrnSaveMode(bool pEnable);

public slots:
    void slot_Initialize();
    void UpdatePara(QObject* pObject);
    void slot_FeedLCD();
    void slot_SrnSaveMode(bool pEnable);

public:
    //>@LCD StandBy
    int             mStandbyMode;  //>@0:��ʾ�Ѿ�����standbyģʽ��1����δ����standbyģʽ
    bool            mStandbyEn;  //>@��ʾʹ��standbyģʽ
    bool            mTouchEn;
    QStringList     mOverlayers;
    int             mRotation;
#ifdef WINDOWS
    CGammaRamp     *mGammaRamp;
#elif LINUX
    QFile          *m_File;
#endif
    NUM_INFO        mLuminance;
    NUM_INFO        mStandBy;

    int             mMonitorCount;
    int             mStandByTimeoutCount;

    NUM_INFO        mDarkLevel;
    int             mCurLevel;

    bool            mOpened;

    bool            mMode;   //>@����ģʽ

    //>@LCD����
    bool            mSrnSaveEn;

    NUM_INFO        mSrnSave;         //>@�ȴ�����������ʱ��
    NUM_INFO        mSrnSaveHold;     //>@������ʾ��ʱ��

    int             mSrnSaveMode;     //>@0:��ʾ�Ѿ���������ģʽ��1����δ��������ģʽ
    int             mSrnSaveTimeoutCount;   //>@�ȴ���������
    int             mSrnSaveHoldCount;      //>@����ʱ��
    SrnSaveForm    *mSrnSaveForm;

    bool            mSrnSaveModeFlag;

#ifdef DISPLAY
    QPointer<PhysicalCOM>       m_Com;
#endif

    //>@bk,���ڴ��౸�ݲ�������initparaʱ��Ҫ��ʼ���������罨������ʱ��Ҫ���浱ǰֵ���ڶϿ�����ʱ�ָ���ֵ
    int             mLuminanceBk;
    int             mStandByBk;
    int             mDarkLevelBk;
    int             mRotationBk;
};

#endif // NODELCD_H

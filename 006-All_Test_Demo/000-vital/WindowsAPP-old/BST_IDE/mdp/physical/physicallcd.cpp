#include "physicallcd.h"
#include "deviceinc.h"
#include "logicthememanager.h"

#ifdef LINUX
#define VIDEO_DISPLAY_ON		_IOW('v', 24, unsigned int)
#define VIDEO_DISPLAY_OFF		_IOW('v', 25, unsigned int)

#define D_FB_DEV        QString("/dev/fb0")
#endif

#ifdef WINDOWS
CGammaRamp::CGammaRamp()
{
    //Initialize all variables.
    hGDI32 = NULL;
    hScreenDC = NULL;
    pGetDeviceGammaRamp = NULL;
    pSetDeviceGammaRamp = NULL;
}

CGammaRamp::~CGammaRamp()
{
    FreeLibrary();
}


BOOL CGammaRamp::LoadLibrary()
{
    BOOL bReturn = FALSE;

    FreeLibrary();
    //Load the GDI library.
    hGDI32 = ::LoadLibrary(L"gdi32.dll");
    if (hGDI32 != NULL)
    {
        //Get the addresses of GetDeviceGammaRamp and SetDeviceGammaRamp API functions.
        pGetDeviceGammaRamp = (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "GetDeviceGammaRamp");
        pSetDeviceGammaRamp = (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "SetDeviceGammaRamp");

        //Return TRUE only if these functions exist.
        if (pGetDeviceGammaRamp == NULL || pSetDeviceGammaRamp == NULL)
            FreeLibrary();
        else
            bReturn = TRUE;
    }

    return bReturn;
}


void CGammaRamp::FreeLibrary()
{
    //Free the GDI library.
    if (hGDI32 != NULL)
    {
        ::FreeLibrary(hGDI32);
        hGDI32 = NULL;
    }
}


BOOL CGammaRamp::LoadLibraryIfNeeded()
{
    BOOL bReturn = FALSE;

    if (hGDI32 == NULL)
        LoadLibrary();

    if (pGetDeviceGammaRamp != NULL && pSetDeviceGammaRamp != NULL)
        bReturn = TRUE;

    return bReturn;
}


BOOL CGammaRamp::SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    //Call to SetDeviceGammaRamp only if this function is successfully loaded.
    if (LoadLibraryIfNeeded())
        return pSetDeviceGammaRamp(hDC, lpRamp);
    else
        return FALSE;
}


BOOL CGammaRamp::GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    //Call to GetDeviceGammaRamp only if this function is successfully loaded.
    if (LoadLibraryIfNeeded())
        return pGetDeviceGammaRamp(hDC, lpRamp);
    else
        return FALSE;

}


BOOL CGammaRamp::SetBrightness(HDC hDC, WORD wBrightness)
{
    /*
    Changes the brightness of the entire screen.
    This function may not work properly in some video cards.

    The wBrightness value should be a number between 0 and 255.
    128 = Regular brightness
    above 128 = brighter
    below 128 = darker

    If hDC is NULL, SetBrightness automatically load and release
    the display device context for you.

    */
    BOOL bReturn = FALSE;
    HDC hGammaDC = hDC;

    //Load the display device context of the entire screen if hDC is NULL.
    if (hDC == NULL)
        hGammaDC = GetDC(NULL);

    if (hGammaDC != NULL)
    {
        //Generate the 256-colors array for the specified wBrightness value.
        WORD GammaArray[3][256];

        for (int iIndex = 0; iIndex < 256; iIndex++)
        {
            int iArrayValue = iIndex * (wBrightness + 128);

            if (iArrayValue > 65535)
                iArrayValue = 65535;

            GammaArray[0][iIndex] =
            GammaArray[1][iIndex] =
            GammaArray[2][iIndex] = (WORD)iArrayValue;

        }

        //Set the GammaArray values into the display device context.
        bReturn = SetDeviceGammaRamp(hGammaDC, GammaArray);
    }

    if (hDC == NULL)
        ReleaseDC(NULL, hGammaDC);

    return bReturn;
}
#endif

PhysicalLCD::PhysicalLCD(QObject *parent) :
    PhysicalDevice(parent)
{
    mTouchEn = false;
    mOpened = true;
    mCurLevel = -1;
#ifdef WINDOWS
    mGammaRamp = new CGammaRamp;
#elif LINUX
    m_File = new QFile;
    m_File->setFileName(D_FB_DEV);
    m_File->open(QIODevice::ReadWrite|QIODevice::Unbuffered);
#elif PHONE

#endif

    mMonitorWait.SetTimeout(500);

    mMonitorCount = 0;

    mStandByTimeoutCount = 0;
    mStandbyEn = true;
    mStandbyMode = 0;
    mSrnSave.setnum(100, 100, 36000);//>@��С10s�����1Сʱ
    mSrnSaveHold.setnum(10, 50, 600);//>@��С1s�����1����

    mSrnSaveEn = true;   //>@һ����������������Ч��
    mSrnSaveMode = 1;
    mSrnSaveForm = 0;
    mSrnSaveModeFlag = false;

    mMode = false;

    connect(this, SIGNAL(sSrnSaveMode(bool)), this, SLOT(slot_SrnSaveMode(bool)), Qt::QueuedConnection);
}

PhysicalLCD::~PhysicalLCD()
{
    mMonitorCount = 0;
#ifdef WINDOWS
    delete mGammaRamp;
#elif LINUX
    if(m_File)
        m_File->close();
#endif
}

bool PhysicalLCD::SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce)
{
    if(pDevMode < mDevMode)
        return false;
    if(pFlag)
    {
        mDevMode = DM_NORMAL;
        if(mEnable == false)
        {
            mEnable = true;
#ifdef DISPLAY
            mMonitorWait.update(mMonitorCount, m_DevManager->m_DevMonitorFreq);
            slot_FeedLCD();
#endif
        }

        //>@��ѯ���޼�ͷ
#ifdef DISPLAY
        if(!m_Com.isNull())
        {
            if(!m_Com->mHasArrow)
            {
                if(!mSrnSaveEn)
                    SetSrnSaveEn(true);
            }
        }
#endif

        //>@������������ģʽ�˳�ʱ����Ҫ���³�ʼ�����ܱ��޸ĵĲ���
        if(pDevMode == DM_NET)
        {
            if(mLuminanceBk != mLuminance.mNum)
                SetPara(QString("Luminance"), QString::number(mLuminanceBk), true);
            if(mStandByBk != mStandBy.mNum)
                SetPara(QString("StandBy"), QString::number(mStandByBk), true);
            if(mDarkLevelBk != mDarkLevel.mNum)
                SetPara(QString("DarkLevel"), QString::number(mDarkLevelBk), true);
            if(mRotationBk != mRotation)
                SetPara(QString("Rotation"), QString::number(mRotationBk), true);
        }
    }
    else
    {
        mDevMode = pDevMode;
        if(pDevMode != DM_STANDBY)
            mEnable = false;
        //>@����ģʽ��������ģʽ
        if(pDevMode == DM_MDISK || pDevMode == DM_NET)  //>@����U�������Լ�APPģʽʱ���˳�����
            SetSrnSaveEn(false);

        //>@��������������ģʽʱ����Ҫ������ܱ��޸ĵĲ���
        if(pDevMode == DM_NET)
        {
            mLuminanceBk = mLuminance.mNum;
            mStandByBk = mStandBy.mNum;
            mDarkLevelBk = mDarkLevel.mNum;
            mRotationBk = mRotation;
        }
    }
    return true;
}

void PhysicalLCD::slot_Initialize()
{
    if(!mSrnSaveForm)  //>@ʹ�ô˿���ȥ��Scene����ɫ�߿�
    {
        mSrnSaveForm = new SrnSaveForm;
#ifndef WINDOWS
        mSrnSaveForm->setGeometry(qApp->desktop()->rect());
#else
        mSrnSaveForm->setGeometry(0,0,100,100);
#endif
        QLabel *tmpLabel = mSrnSaveForm->GetLogoLabel();
        tmpLabel->setScaledContents(true);
        QString tmpSrnPix = "/usr/bst/BOOT/ScrSave.BMP";
        QFile tmpFile(tmpSrnPix);
        if(tmpFile.exists())
            tmpLabel->setPixmap(QPixmap("/usr/bst/BOOT/ScrSave.BMP"));
        else
            tmpLabel->setPixmap(QPixmap("/usr/bst/BOOT/logo.png"));

        mSrnSaveForm->show();
        mSrnSaveForm->lower();
    }
    PhysicalDevice::slot_Initialize();
#ifdef DISPLAY
    m_Com = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
#endif
}

//>@��ʾʹ��Standby
bool PhysicalLCD::SetStandbyEn(bool pFlag)
{
    if(mStandbyEn != pFlag)
    {
        if(pFlag)
            mStandByTimeoutCount = mMonitorCount + mStandBy.mNum/100;
        mStandbyEn = pFlag;
        return true;
    }
    return false;
}

//>@��ʾʹ������
bool PhysicalLCD::SetSrnSaveEn(bool pFlag)
{
    if(mSrnSaveEn != pFlag)
    {
        if(pFlag)
        {
            //IDE_TRACE();
            if(mSrnSave.mNum)
                mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
            mSrnSaveMode = 1;
        }
        else
        {
            //IDE_TRACE();
            emit sSrnSaveMode(false);
            mSrnSaveMode = 1;
        }
        mSrnSaveEn = pFlag;
        return true;
    }
    return false;
}

void PhysicalLCD::ReCalibrate(bool pFroce)
{
    //>@�鿴����У׼�ļ������û�������У׼�����pFroceΪtrue��������ζ�����У׼
    if(!pFroce)
    {
        if(QFile::exists("/etc/pointeal"))
            return;
    }
    //>@��ʼУ׼
#ifdef LINUX
#ifdef IMX6X
    QDir tmpDir;
    tmpDir.cd("/usr/local/tslib/bin/");
    QProcess tmpProcess;
    tmpProcess.start("/usr/local/tslib/bin/ts_calibrate");
    tmpProcess.waitForFinished(60000);
    if(tmpProcess.state() != QProcess::NotRunning)
        tmpProcess.kill();
    tmpDir.cd("/");
#else
    QProcess tmpProcess;
    tmpProcess.start("ts_calibrate");
    tmpProcess.waitForFinished(60000);
    if(tmpProcess.state() != QProcess::NotRunning)
        tmpProcess.kill();
#endif
#endif
}

void PhysicalLCD::slot_FeedLCD()  //>@����lcd standbyģʽ
{
    if(mStandbyMode == 0)  //>@����Ѿ��������������˳�
    {
        emit sStandby(false);
        mStandbyMode = 1;
    }
    if(mStandBy.mNum)
        mStandByTimeoutCount = mMonitorCount + mStandBy.mNum/100;
    SetLuminance(mLuminance.mNum);

    if(mSrnSaveMode == 0)
    {
        emit sSrnSaveMode(false);
        mSrnSaveMode = 1;
    }
    if(mSrnSave.mNum)
        mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
}

quint32 PhysicalLCD::Monitor(quint32 pCount)  //>@100ms����һ��
{
    mMonitorCount = pCount;
    if(!mEnable)
    {
        SetLuminance(mLuminance.mNum);
        return 0;
    }
    if(!mInitialized)
        return 0;
#ifdef DISPLAY
    if(mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
    {
        if(mStandbyEn && mStandBy.mNum)
        {
            if(mStandbyMode == 1)
            {
                if(mMonitorCount >= mStandByTimeoutCount) //>@����standyģʽ
                {
                    //>@����standbyģʽ
                    emit sStandby(true);
                    mStandbyMode = 0;
                    SetLuminance(mDarkLevel.mNum);
#ifdef DISPLAY
                    emit sSetDevices(false, DM_STANDBY);
#endif
                }
            }
        }
        else
        {
            SetLuminance(mLuminance.mNum);
        }

        //>@����
        if(mSrnSaveEn && mSrnSave.mNum)
        {
            if(mSrnSaveMode == 0)  //>@�Ѿ�������ģʽ
            {
                //IDE_TRACE();
                if(mMonitorCount >= mSrnSaveHoldCount) //>@�˳�����ģʽ
                {
                    emit sSrnSaveMode(false);
                    mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
                    mSrnSaveMode = 1;
                }
            }
            else if(mSrnSaveMode == 1)
            {
                //IDE_TRACE();
                if(mMonitorCount >= mSrnSaveTimeoutCount) //>@��������ģʽ
                {
                    emit sSrnSaveMode(true);
                    mSrnSaveHoldCount = (mMonitorCount + mSrnSaveHold.mNum) % D_MAXTIMEOUT;
                    mSrnSaveMode = 0;
                }
            }
            //IDE_TRACE();
        }
        else
        {
            emit sSrnSaveMode(false);
            //IDE_TRACE();
        }
    }
#endif
}

void PhysicalLCD::slot_SrnSaveMode(bool pEnable)
{
    if(!mSrnSaveForm)
    {
        IDE_TRACE();
        return;
    }
    if(mSrnSaveModeFlag == pEnable)
    {
        IDE_TRACE();
        return;
    }
    mSrnSaveModeFlag = pEnable;
    if(pEnable)
    {
        //mSrnSaveForm->show();
        mSrnSaveForm->raise();
        mSrnSaveForm->activateWindow();
    }
    else
    {
        mSrnSaveForm->lower();
        //mSrnSaveForm->hide();  //>@���ϻ��������
    }
}


bool PhysicalLCD::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QString tmpCmd = tmpList.at(i);
        if(tmpCmd.isEmpty())
            continue;
        IDE_TRACE_STR(tmpCmd);
        if(!tmpCmd.compare("close", Qt::CaseInsensitive))
        {
            Close();
        }
        else if(!tmpCmd.compare("open", Qt::CaseInsensitive))
        {
            Open();
        }
    }
    return true;
}

quint32 PhysicalLCD::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("OverLayer"))
        {
            mOverlayers = tmpElement.text().split(';');
            tmpHBoxLayout = CreateCheck(tmpName, tmpElement.attribute("List").split(';'), mOverlayers);
        }
        else if(!tmpName.compare("Rotation"))
        {
            QString tmpValue = tmpElement.text();
            mRotation = tmpValue.toInt();
            mRotationBk = mRotation;
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), tmpValue);
#ifdef X86

#endif
        }
        else if(!tmpName.compare("Touch"))
        {
            QString tmpValue = tmpElement.text();
            mTouchEn = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else if(!tmpName.compare("Luminance"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mLuminance.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            SetLuminance(mLuminance.mNum);
            mLuminanceBk = mLuminance.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mLuminance.mNum, mLuminance.mMinNum, mLuminance.mMaxNum);
        }
        else if(!tmpName.compare("StandBy"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mStandBy.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mStandByBk = mStandBy.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mStandBy.mNum, mStandBy.mMinNum, mStandBy.mMaxNum);
        }
        else if(!tmpName.compare("DarkLevel"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mDarkLevel.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mDarkLevelBk = mDarkLevel.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mDarkLevel.mNum, mDarkLevel.mMinNum, mDarkLevel.mMaxNum);
        }
        else if(!tmpName.compare("SrnSave"))
        {
            int tmpNum = 0, tmpMinNum = 0, tmpMaxNum = 0;
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                tmpNum = tmpElement.text().toInt();
                tmpMinNum = tmpValueList.at(0).toInt();
                tmpMaxNum = tmpValueList.at(1).toInt();
                if(m_DevManager->m_DevMonitorFreq)
                {
                    mSrnSave.setnum(tmpNum/m_DevManager->m_DevMonitorFreq, tmpMinNum/m_DevManager->m_DevMonitorFreq, tmpMaxNum/m_DevManager->m_DevMonitorFreq);
                    if(mSrnSave.mNum)
                        mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
                }
            }
            tmpHBoxLayout = CreateNum(tmpName, tmpNum, tmpMinNum, tmpMaxNum);
        }
        else if(!tmpName.compare("SrnSaveHold"))
        {
            int tmpNum = 0, tmpMinNum = 0, tmpMaxNum = 0;
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                tmpNum = tmpElement.text().toInt();
                tmpMinNum = tmpValueList.at(0).toInt();
                tmpMaxNum = tmpValueList.at(1).toInt();
                if(m_DevManager->m_DevMonitorFreq)
                    mSrnSaveHold.setnum(tmpNum/m_DevManager->m_DevMonitorFreq, tmpMinNum/m_DevManager->m_DevMonitorFreq, tmpMaxNum/m_DevManager->m_DevMonitorFreq);
            }
            tmpHBoxLayout = CreateNum(tmpName, tmpNum, tmpMinNum, tmpMaxNum);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    slot_FeedLCD();
    return validcount;
}

void PhysicalLCD::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("OverLayer"))
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyCheck(tmpComponent, tmpElement, mOverlayers, tmpComponent->GetSelectList()))
            {
            }
        }
    }
    else if(!tmpName.compare("Rotation"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpRotation;
            if(!ModifyEnum(tmpComponent, tmpElement, tmpRotation, tmpComponent->currentText()))
            {
            }
            else
            {
                mRotation = tmpRotation.toInt();
                //>@PhysicalDial�л�ִ��һ���޸���ת�Ƕȣ�Ҳ�������д˴�������ʱLogicThemeManager��û�м��أ���˲�û��ʲôBUG��
                LogicThemeManager *tmpTheme = 0;
#ifdef IDE
                tmpTheme = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#elif DISPLAY
                tmpTheme = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#endif
                if(tmpTheme)
                {
                    tmpTheme->RotateTheme(mRotation);
                }
            }
        }
    }
    else if(!tmpName.compare("Touch"))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mTouchEn, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("Luminance"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mLuminance.mNum, tmpComponent->value(), mLuminance.mMinNum, mLuminance.mMaxNum))
            {
            }
            else
            {
                SetLuminance(mLuminance.mNum);
            }
        }
    }
    else if(!tmpName.compare("StandBy"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mStandBy.mNum, tmpComponent->value(), mStandBy.mMinNum, mStandBy.mMaxNum))
            {
            }
            else
            {
                if(mStandBy.mNum == 0)
                    SetStandbyEn(false);
                else
                    SetStandbyEn(true);
            }
        }
    }
    else if(!tmpName.compare("DarkLevel"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mDarkLevel.mNum, tmpComponent->value(), mDarkLevel.mMinNum, mDarkLevel.mMaxNum))
            {
            }
        }
    }
    else if(!tmpName.compare("SrnSave"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            int tmpNum;
            if(!ModifyNum(tmpComponent, tmpElement, tmpNum, tmpComponent->value(), mSrnSave.mMinNum*m_DevManager->m_DevMonitorFreq, mSrnSave.mMaxNum*m_DevManager->m_DevMonitorFreq))
            {
            }
            else
            {
                mSrnSave.mNum = tmpNum;
                if(mSrnSave.mNum == 0)
                    SetSrnSaveEn(false);
                else
                    SetSrnSaveEn(true);
            }
        }
    }
    else if(!tmpName.compare("SrnSaveHold"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            int tmpNum;
            if(!ModifyNum(tmpComponent, tmpElement, tmpNum, tmpComponent->value(), mSrnSaveHold.mMinNum*m_DevManager->m_DevMonitorFreq, mSrnSaveHold.mMaxNum*m_DevManager->m_DevMonitorFreq))
            {
            }
            else
            {
                mSrnSaveHold.mNum = tmpNum;
            }
        }
    }
}

//>@0~100����Բ�ͬ���豸����ת��
bool PhysicalLCD::SetLuminance(int pValue)
{
    if(!mOpened)  //>@�������Ļ�رգ��������ñ���
    {
        pValue = 0;
    }
    if(pValue < 0)
        pValue = 0;
    else if(pValue > 100)
        pValue = 100;
    if(pValue == mCurLevel)
        return true;
#ifdef DISPLAY
    int tmpValue = 0;
#ifdef WINDOWS
    //>@0~255
    tmpValue = pValue*255/100;
    mGammaRamp->SetBrightness(NULL, 127);
#elif LINUX
#ifdef N329
    //>@10Ϊ������190Ϊ���Ҳ��0~100��Ӧ�Ĳ��Ϊ1.8
    tmpValue = 190-(pValue*18/10);
    IDE_DEBUG(QString("Bright light is %1").arg(tmpValue));
    ioctl(m_File->handle(), IOCTL_LCD_BRIGHTNESS, &tmpValue);
#elif IMX6X
    //>@0��С��248���
    tmpValue = pValue*248/100;
    QString tmpCmd = QString("echo %1 > /sys/class/backlight/pwm-backlight.0/brightness").arg(tmpValue);
    system(tmpCmd.toLatin1().data());
    IDE_TRACE_STR(tmpCmd);
#endif
#endif
#endif
    mCurLevel = pValue;
    return true;
}

//>@��������ģʽ�������ģʽ�󣬱���һֱ��������������Ӧopen��close
void PhysicalLCD::GotoMode(bool pEnable)
{
    Open();
    mMode = pEnable;
}

bool PhysicalLCD::Close()
{
    if(!mMode && mOpened)
    {
        SetLuminance(0);
        mOpened = false;  //>@�����Ҫ�ر���Ļ�����������ñ���֮��
        emit sSetDevices(false, DM_SLEEP);
    }
    return true;
}

bool PhysicalLCD::Open()
{
    if(!mMode && !mOpened)  //>@Sodimas��CANOPEN���������ڽ���operation���һ��open������������������ʱ�ڴ���ģʽ�£���ô���ڷ���open������ٷ�һ����������������ʱ����ظ�ִ��open�����Ϊ�豸Ĭ���ǿ����ģ�������ִ����ı���¥�����ͷ������ʾ��bug��
    {
        mOpened = true;  //>@�����Ҫ������Ļ�����������ñ���֮ǰ
        SetLuminance(mLuminance.mNum);
        emit sSetDevices(true, DM_SLEEP);
    }
    return true;
}



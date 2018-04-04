#include "physicalbase.h"
#include "deviceinc.h"
#include "logicbase.h"

#ifdef SURRPORT_USBWATCHE
QDeviceWatcher* PhysicalMovable::mDeviceWatcher = 0;
int PhysicalMovable::mOccupyCount = 0;
QThread* PhysicalMovable::mWatcherThread = 0;
#endif

PhysicalDevice::PhysicalDevice(QObject *parent) :
    IODeviceBase(parent)
{
    mDevState = S_OFF;
    mDevType = P_INVALID;
    mLGDevices.clear();
}

PhysicalDevice::~PhysicalDevice()
{
    mDevState = S_OFF;

    //>@ɾ�����߼��ڵ��ж��Լ�������
    QList<LG_Type> tmpList = mLGDevices.keys();
    for(int i=tmpList.count()-1;i>0;i--)
    {
        LogicDevice *tmpLGDev = mLGDevices.take(tmpList.at(i));
        if(!tmpLGDev)
            continue;
        tmpLGDev->PHDeviceDeleted(this, false);
    }
    mLGDevices.clear();
}

void PhysicalDevice::SetDevState(DEVICESTATE pState, QString pDevPath)
{
    if(mDevState != pState)
    {
        if(pState == S_ON)
        {
            if(!pDevPath.endsWith("/"))
                pDevPath.append("/");
            mDevPath = pDevPath;
        }
        mDevState = pState;
        emit sDeviceStateChanged();
    }
}

void PhysicalDevice::AppendLGDev(LG_Type pType, LogicDevice *pDevice)
{
    if(!mLGDevices.contains(pType))
        mLGDevices.insert(pType, pDevice);
}

bool PhysicalDevice::InitDevice(PH_Type pPHType, QDomElement pElement)
{
    if(pPHType == P_INVALID || pElement.isNull())
        return false;
    mDevType = pPHType;
    mDevName = pElement.tagName();
    if(!pElement.hasChildNodes())
        return false;
    m_ParaElement = pElement;
    //>@��������ҳ��
    if(m_SettingGroup)
        m_SettingGroup->deleteLater();
    m_SettingGroup = new Groupbox(mDevName);
    if(!m_SettingGroup)
        return false;
    //>@��ʼ��������
    quint32 count = InitPara();
    if(!count)
    {
        m_SettingGroup->deleteLater();
        m_SettingGroup = 0;
    }
    else
    {
        m_SettingGroup->setFixedHeight(80+count*35);
    }
#ifdef DISPLAY
    SetEnable(true, DM_NORMAL);
#endif
    return (bool)(m_SettingGroup != 0);
}

PhysicalMovable::PhysicalMovable(QObject *parent):
    PhysicalUpgrade(parent)
{
    mIsMovable = false;
    OpenDeviceWatcher();
}

PhysicalMovable::~PhysicalMovable()
{
    CloseDeviceWatcher();
}

bool PhysicalMovable::OpenDeviceWatcher()
{
#ifdef SURRPORT_USBWATCHE
    mOccupyCount++;
    if(!mDeviceWatcher)
    {
        mDeviceWatcher = new QDeviceWatcher;
        mWatcherThread = new QThread;
        mDeviceWatcher->moveToThread(mWatcherThread);
        mDeviceWatcher->appendEventReceiver(this);
        mWatcherThread->start();
        mDeviceWatcher->start();
    }
    connect(mDeviceWatcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)), Qt::AutoConnection);
    connect(mDeviceWatcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)), Qt::AutoConnection);
#endif
    return true;
}

void PhysicalMovable::CloseDeviceWatcher()
{
#ifdef SURRPORT_USBWATCHE
    disconnect(mDeviceWatcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)));
    disconnect(mDeviceWatcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)));
    mOccupyCount--;
    if(mOccupyCount == 0)
    {
        if(mWatcherThread)
        {
            mWatcherThread->quit();
            mWatcherThread->deleteLater();
            mWatcherThread = 0;
        }
        if(mDeviceWatcher)
        {
            mDeviceWatcher->deleteLater();
            mDeviceWatcher = 0;
        }
    }
#endif
}

quint32 PhysicalMovable::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    PreInitProcess();
    quint32 validcount = 0;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QHBoxLayout* tmpHBoxLayout = 0;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("Movable"))
        {
            QString tmpValue = tmpElement.text();
            mIsMovable = (tmpValue.compare("true",Qt::CaseInsensitive)==0)?true:false;
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else if(!InitSubPHPara(tmpName, tmpElement, tmpHBoxLayout))
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    if(!mIsMovable)
    {
        SetPlugged();
        mDevState = S_ON;
    }
    //>@����
    PostInitProcess();
    return validcount;
}

void PhysicalMovable::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    PreUpdateProcess();
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Movable"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mIsMovable, tmpComponent->isChecked()))
            {
                if(!mIsMovable)
                {
                    SetPlugged();
                    mDevState = S_ON;
                }
            }
        }
    }
    else
    {
        UpdateSubPHPara(tmpName, pObject, tmpElement);
    }
    //>@����
    PostUpdateProcess();
}

//>@�����κο��ƶ��豸��ֻ������һ����豸������ٲ�����ͬ���豸��ϵͳ�����д���
//>@���γ��豸ʱ�������жϴ��豸��·����mDevPath�Ƿ���ͬ���������ͬ������Ϊ���ǵ�ǰ��Ч�豸���γ�
void PhysicalMovable::slot_DevStateChanged(DEVICESTATE pState, QString pDevPath)
{
    if(!mIsMovable)  //>@�����ǰ�豸���ǿɲ���豸����ִ�д˺�����Ч
        return;
    if(mDevState != pState)  //>@�鿴�Ƿ��Ѿ����ڿɲ���豸������ǣ����²�����豸������
    {
        DEVICESTATE tmpBkState = mDevState;
        mDevState = pState;
        bool ok = false;
        if(pState == S_OFF)
        {
            if(!mDevPath.compare(pDevPath))
                ok = SetUnplugged();
        }
        else if(pState == S_ON)
        {
            QString tmpBkPath = mDevPath;
            mDevPath = pDevPath;
            ok = SetPlugged();
            if(!ok)
                mDevPath = tmpBkPath;
        }
        IDE_TRACE_INT(mDevState);
        if(!ok)
            mDevState = tmpBkState;
        IDE_TRACE_INT(mDevState);
    }
}

bool PhysicalUpgrade::CheckCmdFile(QString pFile)
{
    QFile tmpFile(pFile);
    if(tmpFile.exists())
    {
        mContentList.insert(CONTENT_CMD, pFile);
        return true;
    }
    return false;
}

bool PhysicalUpgrade::ExecCmdList()
{
#ifdef DISPLAY
    QList<QString> tmpCmdFileList = mContentList.values(CONTENT_CMD);
    QString tmpCmdFile;
    foreach(tmpCmdFile, tmpCmdFileList)
    {
        cmdManager *tmpCmdManager = new cmdManager(tmpCmdFile);
        if(!tmpCmdManager->OpenCMD())
        {
            tmpCmdManager->m_IsChanged = false;
            delete tmpCmdManager;
            DelFile(tmpCmdFile);
            continue;
        }
        bool flag = tmpCmdManager->GetItemElement("/CMD");
        if(flag == false)
        {
            tmpCmdManager->m_IsChanged = false;
            delete tmpCmdManager;
            DelFile(tmpCmdFile);
            continue;       //>@���DEV�ļ����ݲ��Ϸ������˳�
        }
        DelFile(tmpCmdFile);  //>@ʹ�����˾�ɾ��
        QDomElement tmpElement;
        QDomNodeList tmpCmdList = tmpCmdManager->itemElement.childNodes();
        for(int i=0;i<tmpCmdList.count();i++)
        {
            tmpElement = tmpCmdList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpOperate = tmpElement.attribute("Operate");
            QString tmpString = tmpElement.text();
            if(tmpString.isEmpty())
                continue;
            IDE_DEBUG(QString("%1#%2").arg(tmpOperate).arg(tmpString));
            //>@��������ִ��ָ��
            if(!tmpOperate.compare("set", Qt::CaseInsensitive))
            {
                xmlParse *tmpSetParse = new xmlParse;
                if(tmpSetParse->setContent(tmpString))
                {
                    QDomElement tmpElement = tmpSetParse->documentElement();
                    if(!tmpElement.isNull())
                    {
                        QString tmpCommonType = tmpElement.tagName();
                        if(!tmpCommonType.compare("Physical"))
                        {
                            QDomNodeList tmpPHDevList = tmpElement.childNodes();
                            for(int i=0;i<tmpPHDevList.count();i++)
                            {
                                QDomElement tmpPHElement = tmpPHDevList.at(i).toElement();
                                if(tmpPHElement.isNull())
                                    continue;
                                PhysicalDevice* tmpPHDev = m_DevManager->m_PHDevList.value(GetPHType(tmpPHElement.tagName()));
                                if(!tmpPHDev)
                                    continue;
                                tmpPHDev->SetPara(tmpPHElement, tmpSetParse);
                            }
                        }
                        else if(!tmpCommonType.compare("Logic"))
                        {
                            QDomNodeList tmpLGDevList = tmpElement.childNodes();
                            for(int i=0;i<tmpLGDevList.count();i++)
                            {
                                QDomElement tmpLGElement = tmpLGDevList.at(i).toElement();
                                if(tmpLGElement.isNull())
                                    continue;
                                LogicDevice* tmpLGDev = m_DevManager->m_LGDevList.value(GetLGType(tmpLGElement.tagName()));
                                if(!tmpLGDev)
                                    continue;
                                tmpLGDev->SetPara(tmpLGElement, tmpSetParse);
                            }
                        }
                        else if(!tmpCommonType.compare("Theme"))
                        {
                            LogicThemeManager* tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
                            if(!tmpThemeManager)
                                return false;
                            QDomNodeList tmpComList = tmpElement.childNodes();
                            for(int i=0;i<tmpComList.count();i++)
                            {
                                QDomElement tmpComElement = tmpComList.at(i).toElement();
                                if(tmpComElement.isNull())
                                    continue;
                                COM_TYPE tmpType = getComponetType(tmpComElement.tagName());
#ifdef SUPPORT_SCENE
                                GraphicsOperate* tmpCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpType);
#elif SUPPORT_WIDGET
                                WidgetBase* tmpCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpType);
#endif
                                if(!tmpCom)
                                    continue;
                                tmpCom->SetPara(tmpComElement, tmpSetParse);
                                //>@���������ӿؼ�
                                QDomNodeList tmpSubComList = tmpComElement.firstChildElement("Control").childNodes();
                                for(int i=0;i<tmpComList.count();i++)
                                {
                                    QDomElement tmpSubComElement = tmpSubComList.at(i).toElement();
                                    if(tmpSubComElement.isNull())
                                        continue;
                                    COM_TYPE tmpSubType = getComponetType(tmpSubComElement.tagName());
#ifdef SUPPORT_SCENE
                                    GraphicsOperate* tmpSubCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpSubType);
#elif SUPPORT_WIDGET
                                    WidgetBase* tmpSubCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpSubType);
#endif
                                    if(!tmpSubCom)
                                        continue;
                                    tmpSubCom->SetPara(tmpSubComElement, tmpSetParse);
                                }
                            }
                        }
                    }
                }
                tmpSetParse->deleteLater();
            }
            else if(!tmpOperate.compare("cmd", Qt::CaseInsensitive))
            {
                tmpString.replace(";","&");
                system(tmpString.toLatin1().data());
                IDE_TRACE_STR(tmpString);
            }
            emit sInfo(QString("Executing %1 ...").arg(tmpString), 1000);
            QtSleep(10);
        }
        tmpCmdManager->m_IsChanged = false;
        delete tmpCmdManager;
    }
#endif
    return true;
}

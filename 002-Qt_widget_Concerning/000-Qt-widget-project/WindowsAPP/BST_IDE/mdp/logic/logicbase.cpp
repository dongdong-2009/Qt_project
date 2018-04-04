#include "logicbase.h"
#include "deviceinc.h"

LogicDevice::LogicDevice(QObject *parent) :
    IODeviceBase(parent)
{
    mPHDevices.clear();
    mDevType = L_INVALID;
}

LogicDevice::~LogicDevice()
{
    mPHDevices.clear();
}

bool LogicDevice::InitDevice(LG_Type pLGType, QDomElement pElement)
{
    if(pLGType == L_INVALID || pElement.isNull())
        return false;
    mDevType = pLGType;
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

bool LogicDevice::AppendPHDevice(PhysicalDevice *pPHDev)
{
    if(!pPHDev)  //>@�Ƿ���ڴ������豸
        return false;
    //>@�鿴�������豸���Ƿ��д��߼��豸�����û�����Ȳ���
    pPHDev->AppendLGDev(mDevType, this);
    PhysicalDevice *tmpPHDev = mPHDevices.value(pPHDev->mDevType);
    if(!tmpPHDev || tmpPHDev != pPHDev)
    {
        if(tmpPHDev)
        {
            disconnect(tmpPHDev, SIGNAL(readyRead()), 0, 0);
            disconnect(tmpPHDev, SIGNAL(sDeviceStateChanged()), 0, 0);
        }
        connect(pPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
        connect(pPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
        mPHDevices.insert(pPHDev->mDevType, pPHDev);
    }
    return true;
}

#if 1
QStringList LogicDevice::AppendPHDevices(QStringList pPHDevs)
{
    QStringList tmpList;
    DeletePHDevices();
    for(int i=0;i<pPHDevs.count();i++)
    {
        PH_Type tmpType = GetPHType(pPHDevs.at(i));
        if(tmpType == P_INVALID)
            continue;
        PhysicalDevice *tmpPHDev = 0;
#ifdef IDE
        tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#elif DISPLAY
        tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#elif SETTING
        tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#endif
        if(!AppendPHDevice(tmpPHDev))
            continue;
        tmpList.append(tmpPHDev->mDevName);
    }
    return tmpList;
}
#else
QStringList LogicFrameParser::AppendPHDevices(QStringList pPHDevs)
{
    QStringList tmpList;
    QList<PH_Type> tmpOldList = mPHDevices.keys();
    IDE_TRACE_INT(pPHDevs.count());
    for(int i=0;i<pPHDevs.count();i++)
    {
        PH_Type tmpType = GetPHType(pPHDevs.at(i));
        if(tmpType == P_INVALID)
            continue;
        //>@����Ѿ����ڵĽӿڣ�������
        if(tmpOldList.contains(tmpType))
        {
            IDE_TRACE_INT(tmpType);
            tmpOldList.removeOne(tmpType);
            continue;
        }
        PhysicalDevice *tmpPHDev = 0;
#ifdef IDE
        tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#elif DISPLAY
        tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#elif SETTING
        tmpPHDev = m_DevManager->m_PHDevList.value(tmpType);
#endif
        if(!AppendPHDevice(tmpPHDev))
            continue;
        IDE_TRACE_INT(tmpType);
        tmpList.append(tmpPHDev->mDevName);
    }
    for(int i=0;i<tmpOldList.count();i++)
    {
        PH_Type tmpType = tmpOldList.at(i);
        PhysicalDevice* tmpPHDev = mPHDevices.take(tmpType);
        if(!tmpPHDev)
            continue;
        IDE_TRACE_INT(tmpType);
        //>@ɾ������ڵ��ͬʱ������Ҫ�������ڵ������Լ�������
        tmpPHDev->mLGDevices.remove(mDevType);
        disconnect(tmpPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
        disconnect(tmpPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
    }
    return tmpList;
}
#endif

QStringList LogicDevice::PHDevicesAdded(QStringList pList, CheckList *pComponent)
{
    if(!pComponent)
        return QStringList();
    QStringList tmpList = AppendPHDevices(pList);
    for(int i=0;i<pList.count();i++)
    {
        QString tmpDevString = pList.at(i);
        Button *tmpCheckBox = pComponent->GetCheckbox(tmpDevString);
        if(tmpCheckBox)
        {
            disconnect(tmpCheckBox, SIGNAL(sPressed(bool)), 0, 0);
            if(!tmpList.contains(tmpDevString))
            {
                tmpCheckBox->setChecked(false);
            }
            else
            {
                tmpCheckBox->setChecked(true);
            }
            connect(tmpCheckBox, SIGNAL(sPressed(bool)), pComponent, SLOT(slot_ButtonClicked(bool)));
        }
    }
    //>@���������豸�ı���ź�
    emit sPHDeviceChanged();
    return tmpList;
}

//>@pSync��ʾɾ���ڵ�����Ƿ�ͬ����������
void LogicDevice::PHDeviceDeleted(PhysicalDevice *pPHDev, bool pSync)
{
    if(pPHDev == 0)
        return;
    if(mPHDevices.isEmpty())
        return;
    PhysicalDevice* tmpPHDev = mPHDevices.take(pPHDev->mDevType);
    if(!tmpPHDev)
        return;
    //>@ɾ������ڵ��ͬʱ������Ҫ�������ڵ������Լ�������
    tmpPHDev->mLGDevices.remove(mDevType);
    disconnect(tmpPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
    disconnect(tmpPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
    //>@ͬ��������
    if(pSync)
    {
        if(m_SettingGroup)
        {
            CheckList *tmpCheckList = m_SettingGroup->findChild<CheckList *>("Device");
            if(!tmpCheckList)
                return;
            for(int i=0;i<tmpCheckList->count();i++)
            {
                Button *tmpCheckBox = tmpCheckList->GetCheckbox(i);
                if(!tmpCheckBox)
                    continue;
                if(tmpCheckBox->text().compare(tmpPHDev->mDevName) == 0)
                {
                    disconnect(tmpCheckBox, SIGNAL(sPressed(bool)), 0, 0);
                    tmpCheckBox->setChecked(false);
                    connect(tmpCheckBox, SIGNAL(sPressed(bool)), tmpCheckList, SLOT(slot_ButtonClicked(bool)));
                    return;
                }
            }
        }
    }
}

void LogicDevice::DeletePHDevices()
{
    QList<PH_Type> tmpList = mPHDevices.keys();
    for(int i=0;i<tmpList.count();i++)
    {
        PhysicalDevice* tmpPHDev = mPHDevices.take(tmpList.at(i));
        if(!tmpPHDev)
            continue;
        //>@ɾ������ڵ��ͬʱ������Ҫ�������ڵ������Լ�������
        tmpPHDev->mLGDevices.remove(mDevType);
        disconnect(tmpPHDev, SIGNAL(readyRead()), this, SLOT(ParseDataStream()));
        disconnect(tmpPHDev, SIGNAL(sDeviceStateChanged()), this, SLOT(DeviceStateChanged()));
        tmpPHDev->m_SettingGroup->setParent(0);
    }
    mPHDevices.clear();
}

void LogicDevice::SetPHDeviceEditble(bool flag)
{
    if(m_SettingGroup)
    {
        CheckList *tmpCheckList = m_SettingGroup->findChild<CheckList *>("Device");
        if(!tmpCheckList)
            return;
        for(int i=0;i<tmpCheckList->count();i++)
        {
            Button *tmpCheckBox = tmpCheckList->GetCheckbox(i);
            if(!tmpCheckBox)
                continue;
            tmpCheckBox->setEnabled(flag);
        }
    }
}

void LogicDevice::ResetPHDevs()
{
    QList<PhysicalDevice*> tmpPHDevList = mPHDevices.values();
    PhysicalDevice *tmpPHDev = 0;
    foreach(tmpPHDev, tmpPHDevList)
    {
        if(tmpPHDev)
        {
            tmpPHDev->reset();
        }
    }
}

void LogicDevice::DeviceStateChanged()
{
    PhysicalDevice* tmpPHDev = VAR_CAST<PhysicalDevice*>(sender());
    if(!tmpPHDev)
        return;
    if(tmpPHDev->mDevState == S_OFF)
        PHDeviceDeleted(tmpPHDev, true);
}

QHBoxLayout* LogicDevice::InitDevicePara(QDomElement pElement)
{
    if(pElement.isNull())
        return (QHBoxLayout*)0;
    QHBoxLayout* tmpHBoxLayout;
    QStringList tmpDevList = pElement.text().split(';');
    AppendPHDevices(tmpDevList);
    tmpHBoxLayout = CreateCheck(pElement.tagName(), pElement.attribute("List").split(';'), tmpDevList);
    return tmpHBoxLayout;
}

void LogicDevice::UpdateDevicePara(QObject *pSender, QDomElement pElement)
{
    if(pElement.isNull())
        return;
    CheckList *tmpComponent = VAR_CAST<CheckList *>(pSender);
    if(tmpComponent)
    {
        QStringList tmpDevList = tmpComponent->GetSelectList();
        QStringList tmpList = PHDevicesAdded(tmpDevList, tmpComponent);
        if(!ModifyCheck(tmpComponent, pElement, tmpComponent->m_SelectList, tmpList))
        {
        }
    }
}

#include "physicaldisk.h"
#include "physicalelevator.h"
#include "modem.h"
#ifdef SUPPORT_SCENE
#include "uiscene.h"
#elif SUPPORT_WIDGET
#include "uiwidget.h"
#endif
#include "deviceinc.h"
#include "physicallcd.h"

bool Match(QString pData, QString pPattern)
{
    QString pattern(pPattern);
    QRegExp rx(pattern);
    bool match = rx.exactMatch(pData);
    return match;
}

QString GetMountAddr(QString pDriver, QString pStorageBufPath)
{
    if(pStorageBufPath.isEmpty() || pDriver.isEmpty())
        return QString();
    QString tmpSubDir;
    if(Match(pDriver, "/dev/sd[a-z][0-9]"))  //>@�ж��Ƿ�ΪU��
    {
        tmpSubDir = "UDisk/";
    }
    else if(Match(pDriver, "/dev/sd[a-z]"))  //>@�ж��Ƿ�ΪU��
    {
        tmpSubDir = "UDisk/";
    }
    else if(Match(pDriver, "/dev/mmcblk[0-9]p[0-9]"))  //>@�ж��Ƿ�ΪMMC
    {
        tmpSubDir = "MMC/";
    }
    else
        return QString();
    pStorageBufPath.replace("\\", "/");
    if(!pStorageBufPath.endsWith('/'))
        pStorageBufPath.append('/');
    QString tmpMountAddr = pStorageBufPath + tmpSubDir;
    return tmpMountAddr;
}

QString MountStorage(QString pDriver, QString pStorageBufPath)
{
    QString tmpMountAddr = GetMountAddr(pDriver, pStorageBufPath);
    if(tmpMountAddr.isEmpty())
    {
        IDE_TRACE_STR(pDriver);
        return QString();
    }
    if(!CreatPath(tmpMountAddr))
    {
        IDE_TRACE_STR(tmpMountAddr);
        return QString();
    }
    QString tmpRet;
    QProcess tmpProcess;
    //>@�˴���Ҫʹ��execute��ʹ�ô˷���readAllStandardError���ؿա�
    tmpProcess.start(QString("mount -t vfat %1 %2").arg(pDriver).arg(tmpMountAddr));
    tmpProcess.waitForFinished(1000);
    tmpRet.append(tmpProcess.readAllStandardOutput());
    tmpRet.append(tmpProcess.readAllStandardError());
    if(!tmpRet.isEmpty())  //>@ֻҪ����Ϣ���ͱ�ʾ���ز��ɹ���
    {
        IDE_TRACE_STR(tmpRet);
        return QString();
    }
    return tmpMountAddr;
}

bool UnMountStorage(QString pMountAddr)
{
    if(pMountAddr.isEmpty())
        return false;
    QString tmpRet;
    QProcess tmpProcess;
    tmpProcess.start(QString("unmount %1").arg(pMountAddr));
    tmpProcess.waitForFinished(1000);
    tmpRet.append(tmpProcess.readAllStandardOutput());
    tmpRet.append(tmpProcess.readAllStandardError());
    if(!tmpRet.isEmpty())
    {
        IDE_TRACE_STR(tmpRet);
        return false;
    }
    return true;
}

PhysicalStorage::PhysicalStorage(QString pAddr, QString pBufferPath, QObject *parent) :
    PhysicalHost(pAddr, pBufferPath, parent)
{
    mDevType = TYPE_STORAGE;
    mDevAddr = pAddr;
}

PhysicalStorage::~PhysicalStorage()
{

}

bool PhysicalStorage::Scan()
{
    if(!PhysicalHost::Scan())
        return false;
    if(isSync())
    {
        //>@��������
        DownloadPreference(D_MDPNAME);
        //>@��������
        QString tmpDevThemePath = mDevAddr+D_THEMEDIR;
        QDir dir(tmpDevThemePath);
        if(!dir.exists())
        {
            IDE_TRACE();
            return false;
        }
        QFileInfoList fileInfoList = dir.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList)
        {
            if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
                continue;
            if(fileInfo.isDir())
            {
                QString tmpDirName = fileInfo.fileName();
                QString tmpProjectPath = tmpDevThemePath + tmpDirName + QString("/") + tmpDirName + QString(".bstpro");
                //mDevItem.mDevTreeItem->AddProject(tmpProjectPath);
            }
        }
        QString tmpThemeName = GetRunningTheme();
        if(!tmpThemeName.isEmpty())
        {
            QString tmpThemePath = tmpDevThemePath + tmpThemeName + QString("/") + tmpThemeName + QString(".bstpro");
            //mDevItem.mDevTreeItem->SetRunningProject(tmpThemePath);
        }
        return true;
    }
    return true;
}

bool PhysicalStorage::isSync()
{
    return (mDevState == S_SYNC);
}

bool PhysicalStorage::Sync()
{
    if(isFileExist(mDevAddr))
    {
        //mDevItem.mDevTreeItem->SetState(S_SYNC);
    }
}

void PhysicalStorage::Async()
{
    if(isSync())
    {
        //mDevItem.mDevTreeItem->SetState(S_ON);
    }
}

void PhysicalStorage::OpenLocation(QString pLocation)
{
    if(isSync() && !pLocation.isEmpty())
    {
        QString tmpPath = mDevAddr + D_THEMEDIR + pLocation;
        tmpPath.replace("//", "/");
        QDesktopServices::openUrl(QUrl(tmpPath, QUrl::TolerantMode));
    }
}

bool PhysicalStorage::DeleteTheme(QString pThemeName)
{
    if(pThemeName.isEmpty())
        return false;
    QString tmpPath = mDevAddr + D_THEMEDIR + pThemeName;
    return DelFolder(tmpPath, false);
}

bool PhysicalStorage::SetRunningTheme(QString pThemeName)
{
    if(pThemeName.isEmpty())
        return false;
    //>@�޸�MDP�ļ�
    QString tmpPath = mDevAddr + D_BOOTDIR + D_MDPNAME;
    mdpManager *tmpManager = new mdpManager(tmpPath);
    if(tmpManager->OpenMDP())
    {
        QDomElement tmpElement = tmpManager->m_LGElementGroup.value(L_ThemeManager);
        if(!tmpElement.isNull())
        {
            QDomElement tmpThemeElement = tmpElement.firstChildElement("ActiveTheme");
            if(!tmpThemeElement.isNull())
            {
                bool flag = tmpManager->SetText(tmpThemeElement, pThemeName);
                tmpManager->SaveMDP();
                tmpManager->deleteLater();
                return flag;
            }
        }
    }
    tmpManager->deleteLater();
    return false;
}

QString PhysicalStorage::GetRunningTheme()
{
    QString tmpPath = mDevAddr + D_BOOTDIR + D_MDPNAME;
    mdpManager *tmpManager = new mdpManager(tmpPath);
    if(tmpManager->OpenMDP())
    {
        QDomElement tmpElement = tmpManager->m_LGElementGroup.value(L_ThemeManager);
        if(!tmpElement.isNull())
        {
            QDomElement tmpThemeElement = tmpElement.firstChildElement("ActiveTheme");
            if(!tmpThemeElement.isNull())
            {
                QString tmpThemeName = tmpThemeElement.text();
                tmpManager->deleteLater();
                return tmpThemeName;
            }
        }
    }
    tmpManager->deleteLater();
    return QString();
}

bool PhysicalStorage::Download(QString pThemeName, QString pThemePath)
{
    if(!isSync())
        return false;
    mIsBusy = true;
    //>@���ص���ǰ����Ŀ¼��
    QString tmpSrcPath = mDevAddr + D_THEMEDIR + pThemeName;
    QString tmpDstPath = QString("%1%2/").arg(mBufferPath).arg(pThemeName);
    if(CopyFolder(tmpSrcPath, tmpDstPath, true))
    {
        mIsBusy = false;
        return true;
    }
    mIsBusy = false;
    return false;
}

//>@���ϵͳ�Ѿ�����ͬ�����⣬�򸲸ǣ�����ֱ���ϴ�
bool PhysicalStorage::Upload(QString pThemeName)
{
    if(!isSync())
        return false;
    mIsBusy = true;
    //>@���Ȳ鿴Output���Ƿ��д�������ļ���
    QString tmpSrcPath = QString("%1%2/%3/%4").arg(mBufferPath).arg(pThemeName).arg(OUTPUTDIR).arg(pThemeName);
    QDir tmpSrcDir(tmpSrcPath);
    if(!tmpSrcDir.exists())
    {
        mIsBusy = false;
        return false;
    }
    QString tmpDstPath = mDevAddr + D_THEMEDIR + pThemeName;
    if(CopyFolder(tmpSrcPath, tmpDstPath, true))
    {
        mIsBusy = false;
        return true;
    }
    mIsBusy = false;
    return false;
}

bool PhysicalStorage::DownloadPreference(QString pMdpName)
{
    if(!isSync())
        return false;
    mIsBusy = true;
    //>@���ص���ǰ����Ŀ¼��
    QString tmpSrcPath = mDevAddr + D_BOOTDIR + pMdpName;
    QString tmpDstPath = mBufferPath + pMdpName;
    if(CopyFile(tmpSrcPath, tmpDstPath))
    {
        mIsBusy = false;
        return true;
    }
    mIsBusy = false;
    return false;
}

bool PhysicalStorage::UploadPreference(QString pMdpName)
{
    if(!isSync())
        return false;
    mIsBusy = true;
    QString tmpSrcPath = mBufferPath + pMdpName;
    QString tmpDstPath = mDevAddr + D_BOOTDIR + pMdpName;
    if(CopyFile(tmpSrcPath, tmpDstPath))
    {
        mIsBusy = false;
        return true;
    }
    mIsBusy = false;
    return false;
}

PhysicalDisk::PhysicalDisk(QObject *parent):
    PhysicalDevice(parent)
{
    mMonitorWait.SetTimeout(60000);
}

PhysicalDisk::~PhysicalDisk()
{

}

bool ReMountFat(QString pPath, int pType)
{
    QString tmpType;
    if(pType == 1)
    {
        tmpType = "vfat ro";
    }
    else if(pType == 2)
    {
        tmpType = "vfat rw";
    }
    else
        return false;
    QProcess tmpProcess;
    QString tmpQuery = QString("cat /proc/mounts | grep %1 | grep %2").arg(pPath).arg(tmpType);
    tmpProcess.start(tmpQuery);
    IDE_TRACE_STR(tmpQuery);
    tmpProcess.waitForFinished(1000);
    QString tmpRet = tmpProcess.readAllStandardOutput();
    if(tmpRet.isEmpty())  //>@�����Ϊ�գ����ʾ��ǰ·���Ѿ�������ΪĿ�����͡�
    {
        QString tmpMount = QString("mount -o %1,remount %2").arg(tmpType).arg(pPath);
        tmpProcess.start(tmpMount);
        IDE_TRACE_STR(tmpMount);
        tmpProcess.waitForFinished(1000);

        //>@���²�ѯ
        tmpProcess.start(tmpQuery);
        IDE_TRACE_STR(tmpQuery);
        tmpProcess.waitForFinished(1000);
        tmpRet = tmpProcess.readAllStandardOutput();
        if(tmpRet.isEmpty())
            return false;
    }
    return true;
}

quint32 PhysicalDisk::Monitor(quint32 pCount)
{
    if(!mEnable)
        return 0;
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    if(!mThreshold.mNum)
    {
        SYSTEM_CMD::GetDiskSpace(mDevPath, mDiskTotal, mDiskUsed, mDiskFree, mUsageRate);
    }
#ifdef IMX6X  //>@���IMX6���ӣ��鿴�Ƿ�Ϊֻ���ļ�ϵͳ
    ReMountFat(QString("/usr/bst"), 2);
#elif N329
    //ReMountFat(QString("/mnt/nand1-2"), 2);
#endif
    return 1;
}

quint32 PhysicalDisk::InitPara()
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
        if(!tmpName.compare("Model"))
        {
            mModel = tmpElement.text();
            tmpHBoxLayout = CreateTextEdit(tmpName, mModel);
        }
        else if(!tmpName.compare("BootCatalog"))
        {
#ifdef LINUX
            mDevPath = tmpElement.text();
            tmpHBoxLayout = CreateTextEdit(tmpName, mDevPath);
#else
            mDevPath = D_FSRCPATH;
#endif
        }
        else if(!tmpName.compare("Threshold"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mThreshold.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            tmpHBoxLayout = CreateNum(tmpName, mThreshold.mNum, mThreshold.mMinNum, mThreshold.mMaxNum);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    //>@����
    if(mDevPath.isEmpty())
        mDevPath = D_FSRCPATH;
    return validcount;
}

void PhysicalDisk::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Model"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mModel, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("BootCatalog"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mDevPath, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("Threshold"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mThreshold.mNum, tmpComponent->value(), mThreshold.mMinNum, mThreshold.mMaxNum))
            {
            }
        }
    }
}

PhysicalMobileDisk::PhysicalMobileDisk(QObject *parent):
    PhysicalMovable(parent)
{
}

PhysicalMobileDisk::~PhysicalMobileDisk()
{
}

QStringList GetDevs(QString pDir)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList();
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}

void PhysicalMobileDisk::slot_Initialize()
{
    PhysicalMovable::slot_Initialize();
    //IDE_DEBUG("MobileDisk");
    //>@�����Ѿ������U�̻���MMC
#ifndef QT_V453
#if (defined(UBUNTU) || defined(LINUX))
    QStringList tmpDevList;
    tmpDevList << "/dev/sda1" << "/dev/sda2" << "/dev/sdb1" << "/dev/sdb2"
               << "/dev/mmcblk1p1" << "/dev/mmcblk1p2" << "/dev/mmcblk2p1" << "/dev/mmcblk2p2";
    QString tmpDev;
    //IDE_TRACE_INT(tmpDevList.count());
    foreach(tmpDev, tmpDevList)
    {
        //IDE_TRACE_STR(tmpDev);
        QString tmpDevPath = MountStorage(tmpDev, mMountBufPath);
        //IDE_TRACE_STR(tmpDevPath);
        if(tmpDevPath.isEmpty())
            continue;
        slot_DevStateChanged(S_ON, tmpDevPath);
    }
#endif
#endif

#ifdef IDE
    mLCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
#elif DISPLAY
    mLCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
#endif
}

bool PhysicalMobileDisk::SetPlugged()
{
    //>@�������Ӻ󣬲�����������ʾͼ��
#ifdef DISPLAY
    //>@��ȷ���������ļ�ʱ�ŷ���standby���������Ҫ��UpgradeTo������д��
    //m_DevManager->PlayBeep("connect");
#ifdef SUPPORT_WIDGET
    emit sShowIcon("usb");
#elif SUPPORT_SCENE
    m_DevManager->ShowIcon("usb");
#endif
#endif

    emit sDeviceStateChanged();
    return true;
}

bool PhysicalMobileDisk::SetUnplugged()
{
    //>@�Ͽ����Ӻ󣬲�����������ʾͼ��
#ifdef DISPLAY
    //m_DevManager->PlayBeep("disconnect");
#ifdef SUPPORT_WIDGET
    emit sHideIcon("usb");
#elif SUPPORT_SCENE
    m_DevManager->ShowIcon("");
#endif
#endif
    //>@ֻҪ����U�̣���������������˴�ʱ���п�������Դ����ʹ�ã����ϵͳ������
    Restart();
    //emit sDeviceStateChanged();
    return true;
}

void PhysicalMobileDisk::slot_DeviceAdded(QString dev)
{
    QStringList tmpList = dev.split('>');
    if(tmpList.count()!=2)
        return;
    QString tmpType = tmpList.at(0);
    if(tmpType.compare("storage", Qt::CaseInsensitive) == 0)  //>@U��
    {
        QString tmpDev = tmpList.at(1);
#if (defined(UBUNTU) || defined(LINUX))
        tmpDev = MountStorage(tmpDev, mMountBufPath);
        IDE_TRACE_STR(tmpDev);
        if(tmpDev.isEmpty())
            return;
#endif
        slot_DevStateChanged(S_ON, tmpDev);
    }
    else if(tmpType.compare("usbdev", Qt::CaseInsensitive) == 0)  //>@�ӻ�U��
    {
#ifdef DISPLAY
#ifdef SUPPORT_WIDGET
        emit sShowIcon("usbdev");
#elif SUPPORT_SCENE
        m_DevManager->ShowIcon("usbdev");
#endif
#endif
    }
}

void PhysicalMobileDisk::slot_DeviceRemoved(QString dev)
{
    QStringList tmpList = dev.split('>');
    if(tmpList.count() != 2)
        return;
    QString tmpType = tmpList.at(0);
    if(tmpType.compare("storage", Qt::CaseInsensitive) == 0)  //>@U��
    {
        QString tmpDev = tmpList.at(1);
#if (defined(UBUNTU) || defined(LINUX))
        tmpDev = GetMountAddr(tmpDev, mMountBufPath);
        IDE_TRACE_STR(tmpDev);
        if(tmpDev.isEmpty())
            return;
#endif
        slot_DevStateChanged(S_OFF, tmpDev);
    }
    else if(tmpType.compare("usbdev", Qt::CaseInsensitive) == 0)  //>@�ӻ�U��
    {
        mContentList.clear();
        if(CheckCmdFile(QString("/usr/bst/"+D_DISK_CMD)))  //>@����п�ִ�е�CMD�ļ��������CMD�߼�����ģʽ
            ExecCmdList();
        mContentList.clear();

        //>@�γ��Ի�USB��ʱ��Ҫ����
        Reboot();
    }
}

void PhysicalMobileDisk::ScanDisks()
{
    for(int i=0;i<mMobileDisks.count();i++)
    {
        QString tmpDiskName = mMobileDisks.at(i);
        if(tmpDiskName.compare("UDisk") == 0)
        {
            //>@�����Ƿ��Ѿ�������U��
            //slot_DevStateChanged(S_ON, QString());
        }
        else if(tmpDiskName.compare("MMC") == 0)
        {
            //>@�����Ƿ��Ѿ�������MMC
            //slot_DevStateChanged(S_ON, QString());
        }
    }
}

bool PhysicalMobileDisk::GetDiskList(QString pDirPath, CONTENT_TYPE pContentType)
{
    QString tmpRePath;
    if(pContentType == CONTENT_BOOT)
        tmpRePath = pDirPath + D_BOOTDIR;
    else if(pContentType == CONTENT_THEME)
        tmpRePath = pDirPath + D_THEMEDIR;
    else if(pContentType == CONTENT_EXE)
        tmpRePath = pDirPath + D_EXEDIR;
    else if(pContentType == CONTENT_SYSTEM)
        tmpRePath = pDirPath + D_SYSTEMDIR;
    if(tmpRePath.isEmpty())
        return false;
    QStringList tmpFileList = findFilesInFloder(tmpRePath, QString(), true);
    if(!tmpFileList.isEmpty())
    {
        for(int i=0;i<tmpFileList.count();i++)
            mContentList.insert(pContentType, tmpFileList.at(i));
        return true;
    }
    return false;
}

bool PhysicalMobileDisk::GetComList(QString pDirPath, CONTENT_TYPE pContentType, QStringList pSuffix)
{
    QString tmpRePath;
    if(pContentType == CONTENT_LOGO)
        tmpRePath = pDirPath + QString("Logo/");
    else if(pContentType == CONTENT_VIDEO)
        tmpRePath = pDirPath + QString("Video/");
    else if(pContentType == CONTENT_IMAGE)
        tmpRePath = pDirPath + QString("Image/");
    else if(pContentType == CONTENT_TEXT)
        tmpRePath = pDirPath + QString("Text/");
    if(tmpRePath.isEmpty())
        return false;
    QString tmpString;
    QStringList tmpPathList = getAllInFloders(false, tmpRePath, false);
    if(tmpPathList.count()>0)
    {
        int count1 = tmpPathList.count();
        int count2 = pSuffix.count();
        for(int i=0;i<count1;i++)
        {
            tmpString = tmpPathList.at(i);
            for(int j=0;j<count2;j++)
            {
                if(tmpString.endsWith(pSuffix.at(j), Qt::CaseInsensitive))
                {
                    mContentList.insert(pContentType, tmpString);
                    break;
                }
            }
        }
        if(!mContentList.values(pContentType).isEmpty())
            return true;
    }
    return false;
}

bool PhysicalMobileDisk::CopyFolder(QString pSrcDir, QString pDstDir)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
        return false;
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@���·����ͬ���򲻽��п���
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    if(!CreatPath(pDstDir))
        return false;
    QDir tmpSrcDir(pSrcDir);
    QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
        QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
        CopyFolder(tmpSrcPath, tmpDstPath);
    }
    QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
    qreal diff = 100.0/files.size();
    for(int i = 0; i < files.size(); ++i)
    {
        QString tmpFileName = getFileName(files[i]);
        QString tmpDstFilePath = QString("%1%2").arg(pDstDir).arg(tmpFileName);
        emit sProgress(QString("Backup %1 ...").arg(tmpFileName), diff*i);
        QtSleep(10);
        CopyFile(files[i], tmpDstFilePath);
    }
    return true;
}

//>@pBackupΪFlaseʱ��ʾ�����ݵ������豸
//>@����mUpgradeDev�豸�е����ݵ����ش���
bool PhysicalMobileDisk::UpgradeDiskList(QString pDstPath, bool pBackup)
{
    if(pDstPath.isEmpty())
        return false;
    QList<CONTENT_TYPE> tmpList = mContentList.uniqueKeys();
    QString tmpSrcPath, tmpRelPath, tmpDstPath;
    for(int i=0;i<tmpList.count();i++)
    {
        CONTENT_TYPE tmpType = tmpList.at(i);
        if(tmpType > CONTENT_DISK && tmpType < CONTENT_COM)
        {
            QStringList tmpFileList = mContentList.values(tmpType);
            if(tmpFileList.isEmpty())
                continue;
            if(mDevState == S_OFF)
                return false;
            QString tmpSrcDisk = mDevPath+D_DISK_FS;
            int tmpSrcDiskLength = tmpSrcDisk.count();
            if(tmpSrcDiskLength && !tmpFileList.isEmpty())
            {
                qreal diff = 100.0 / tmpFileList.count();
                for(int j=0;j<tmpFileList.count();j++)
                {
                    tmpSrcPath = tmpFileList.at(j);
                    int index = tmpSrcPath.indexOf(tmpSrcDisk);
                    if(index < 0)
                        continue;
                    tmpRelPath = tmpSrcPath.mid(index+tmpSrcDiskLength);
                    if(tmpRelPath.isEmpty())
                        continue;
                    tmpDstPath = pDstPath + tmpRelPath;
                    emit sProgress(QString("Updating %1 ...").arg(getFileName(tmpSrcPath)), diff*j);
                    QtSleep(10);
                    //>@�����β��ж��Ƿ���Ҫ���ݵ������豸
                    if(pBackup && QFile::exists(tmpDstPath))
                    {
                        if(!cut(tmpDstPath, mDevPath+D_DISK_BACKUP+tmpRelPath))
                        {
                            IDE_TRACE_STR(tmpRelPath);
                            continue;
                        }
                    }
                    if(!copy(tmpSrcPath, tmpDstPath))
                    {
                        IDE_TRACE_STR(tmpRelPath);
                        continue;
                    }
                }
            }
        }
    }
    //>@�����ӿڰ����
    QString tmpIBFile = pDstPath + D_EXEDIR + D_INTERFACEBIN;
    if(QFile::exists(tmpIBFile))
    {
        //>@Ѱ�Ҵ���
        PhysicalCOM *tmpCom = 0;
#ifdef IDE
        tmpCom = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
#elif DISPLAY
        tmpCom = VAR_CAST<PhysicalCOM*>(m_DevManager->GetPHDev(P_COM));
#endif
#ifndef PHONE
        if(tmpCom && tmpCom->mSerialPort)
        {
            Modem tmpModem(tmpCom->mSerialPort);
            tmpModem.SendFile(tmpIBFile, 10000);
        }
#endif
    }
    return true;
}

//>@pBackupΪFlaseʱ��ʾ�����ݵ�UDisk
bool PhysicalMobileDisk::UpgradeComList(QString pDstPath, bool pBackup)
{
    if(pDstPath.isEmpty())
        return false;
#ifdef DISPLAY
    //>@�鿴��UI�ļ����Ƿ���ڴ���������������ȡ������Ϣ�����������������
#ifdef SUPPORT_SCENE
    GraphicsScene *tmpUiScene = GetThemeScene();
#elif SUPPORT_WIDGET
    UiWidget *tmpUiScene = GetThemeScene();
#endif
    if(!(tmpUiScene && tmpUiScene->m_UiManager))
        return false;
#ifdef SUPPORT_SCENE
    GraphicsOperate *tmpComOperate = 0;
#elif SUPPORT_WIDGET
    WidgetBase *tmpComOperate = 0;
#endif
    QString tmpComRelPath;
    QList<CONTENT_TYPE> tmpList = mContentList.uniqueKeys();
    for(int i=0;i<tmpList.count();i++)
    {
        CONTENT_TYPE tmpType = tmpList.at(i);
        if(tmpType > CONTENT_COM)
        {
            QString tmpComName;
            switch(tmpType)
            {
                case CONTENT_LOGO:
                    tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_LOGO);
                    tmpComRelPath = QString("/Resource/Image/");
                    tmpComName = QString("Logo");
                    break;
                case CONTENT_VIDEO:
                    tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_VIDEO);
                    tmpComRelPath = QString("/Resource/Film/");
                    tmpComName = QString("Video");
                    break;
                case CONTENT_IMAGE:
                    tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_IMAGE);
                    tmpComRelPath = QString("/Resource/Image/");
                    tmpComName = QString("Image");
                    break;
                case CONTENT_TEXT:
                    tmpComOperate = tmpUiScene->m_ComDiagramGroup.value(COM_TEXT);
                    tmpComRelPath = QString("/Resource/Text/");
                    tmpComName = QString("Text");
                    break;
                default:
                    continue;
            }
            if(!tmpComOperate)
                continue;
            QStringList tmpFileList = mContentList.values(tmpType);
            if(tmpFileList.isEmpty())
                continue;
            //>@��ִ�й����������ɹ���ͨ���������ui�ļ��Լ���ǰ����
            QString tmpComRcPath = tmpUiScene->m_UiManager->m_FileDir + tmpComOperate->m_ComPath + tmpComRelPath;
            tmpComRcPath.replace("//", "/");
            QSize tmpSize = tmpComOperate->GetCurRect().size().toSize();
            QString tmpBkPath = mDevPath+D_DISK_BACKUP+D_DISK_COM+tmpComName+"/";
            qreal diff = 100.0 / tmpFileList.count();
            for(int j=0;j<tmpFileList.count();j++)
            {
                QString tmpSrcFilePath = tmpFileList.at(j);
                if(tmpSrcFilePath.isEmpty())
                    continue;
                if(mDevState == S_OFF)
                    return false;
                QString tmpFileName = getFileName(tmpSrcFilePath);
                emit sProgress(QString("Updating COM %1 ...").arg(tmpFileName), diff*j);
                QtSleep(10);
                //>@����
                QString tmpDstFile = regularFile(QString(), tmpSrcFilePath, tmpComRcPath, tmpSize);
                if(tmpDstFile.isEmpty())
                {
                    IDE_TRACE_STR(tmpFileName);
                    continue;
                }
            }
        }
    }
#endif
    return true;
}

CONTENT_TYPE PhysicalMobileDisk::CanUpgrade()
{
    if(mDevState == S_OFF || mDevPath.isEmpty())
        return CONTENT_NONE;
    QDir tmpDir = QDir(mDevPath);
    if(!tmpDir.exists())
        return CONTENT_NONE;
    if(!mDevPath.endsWith("/"))
        mDevPath.append("/");
    //>@2.�����Դ�б�
    mContentList.clear();
    //>@3.�鿴�Ƿ���CMD�ļ�
    if(CheckCmdFile(mDevPath+D_DISK_CMD))  //>@����п�ִ�е�CMD�ļ��������CMD�߼�����ģʽ
        return CONTENT_CMD;
    //>@4.�鿴�Ƿ���̿���
    GetDiskList(mDevPath+D_DISK_FS, CONTENT_BOOT);
    GetDiskList(mDevPath+D_DISK_FS, CONTENT_EXE);
    GetDiskList(mDevPath+D_DISK_FS, CONTENT_THEME);
    GetDiskList(mDevPath+D_DISK_FS, CONTENT_SYSTEM);
    if(!mContentList.isEmpty())
        return CONTENT_DISK;
    //>@5.�鿴COM�ļ����Ƿ�����������
    GetComList(mDevPath+D_DISK_COM, CONTENT_LOGO, QStringList()<<".bmp"<<".jpg"<<".png"<<".svg"<<".gif");
    GetComList(mDevPath+D_DISK_COM, CONTENT_VIDEO, QStringList()<<".avi"<<".mp4"<<".rmvb"<<".flv");
    GetComList(mDevPath+D_DISK_COM, CONTENT_IMAGE, QStringList()<<".bmp"<<".jpg"<<".png"<<".svg"<<".gif");
    GetComList(mDevPath+D_DISK_COM, CONTENT_TEXT, QStringList()<<".bmp"<<".jpg"<<".png"<<".svg"<<".font");
    if(!mContentList.isEmpty())
        return CONTENT_COM;
    return CONTENT_NONE;
}

bool PhysicalMobileDisk::UpgradeTo(QString pDstPasth)
{
    if(pDstPasth.isEmpty())
        return false;
    if(!mLCD.isNull())
        mLCD->GotoMode(true);
    mContentType = CanUpgrade();
    IDE_TRACE_INT(mContentType);
    bool success = false;
    switch(mContentType)
    {
        case CONTENT_CMD:
        {
            emit sSetDevices(false, DM_MDISK);
            QtSleep(100);
            success = ExecCmdList();
            break;
        }
        case CONTENT_DISK:
        {
            emit sSetDevices(false, DM_MDISK);
            QtSleep(100);
            success = UpgradeDiskList(pDstPasth, true);
            break;
        }
        case CONTENT_COM:
        {
            emit sSetDevices(false, DM_MDISK);
            QtSleep(100);
            success = UpgradeComList(pDstPasth, true);
            break;
        }
        default:
        {
#ifdef N329
            emit sSetDevices(false, DM_MDISK);
            QtSleep(100);
            CreatPath(mDevPath+D_DISK_BACKUP);
            ClearFolder(mDevPath+D_DISK_BACKUP);
            success = CopyFolder(pDstPasth, mDevPath+D_DISK_BACKUP);
#endif
            break;
        }
    }
#ifdef N329
    //>@������ϣ�ж��U��
    UnMountStorage(mDevPath);
#endif
    if(success)
        emit sProgress(QString("Updating DISK success!"), 100);
    else
        emit sProgress(QString("Updating DISK error!"), 100);
    emit sProgress(QString("Please remove the USB key ..."), 100);
    emit sProgress(QString("Device will reboot after remove ..."), 1000000);
    //if(!mLCD.isNull())
    //    mLCD->GotoMode(false);
    return success;
}

bool PhysicalMobileDisk::InitSubPHPara(QString pName, QDomElement pElement, QHBoxLayout*& pHBoxLayout)
{
    if(!pName.compare("Model"))
    {
        mMobileDisks = pElement.text().split(';');
        pHBoxLayout = CreateCheck(pName, pElement.attribute("List").split(';'), mMobileDisks);
        ScanDisks();
    }
    else if(!pName.compare("Executable"))
    {
        QString tmpValue = pElement.text();
        mExecutable = (tmpValue.compare("false")==0)?(false):(true);
        pHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("MountBufPath"))
    {
        mMountBufPath = pElement.text();
        pHBoxLayout = CreateTextEdit(pName, mMountBufPath);
    }
    else
    {
        return false;
    }
    return true;
}

void PhysicalMobileDisk::UpdateSubPHPara(QString pName, QObject *pObject, QDomElement pElement)
{
    if(!pName.compare("Model"))
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyCheck(tmpComponent, pElement, mMobileDisks, tmpComponent->GetSelectList()))
            {
            }
            else
            {
                ScanDisks();
            }
        }
    }
    else if(!pName.compare("Executable"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, mExecutable, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!pName.compare("MountBufPath"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, pElement, mMountBufPath, tmpComponent->text()))
            {
            }
        }
    }
}

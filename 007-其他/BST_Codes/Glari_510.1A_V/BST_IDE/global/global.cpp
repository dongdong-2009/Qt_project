#include "global.h"
#ifdef LINUX
#include <unistd.h>
#include <sys/reboot.h>
#endif

const QString cmdManager::CmdSuffix = ".cmd";
const QString cmdManager::CmdFileType = "BST CMD File";

void Logger(MSG_LEVEL pLevel, QString pMsg)
{
#ifdef SURRPORT_LOG
    if(GlobalPara.m_Logger == 0)
        return;
    if(pLevel == LEVEL_WARN)
        GlobalPara.m_Logger->warn(pMsg);
    else if(pLevel == LEVEL_ERROR)
        GlobalPara.m_Logger->error(pMsg);
    else
        GlobalPara.m_Logger->info(pMsg);
#endif
}

void Logger(QString Key, QStringList tmplist)
{
    QString tmpcontent;
    MSG_LEVEL tmpLevel;
    LOGTIP_INFO *tmpLogTip = GlobalPara.m_LogTipGroup.value(Key);
    if(tmpLogTip == 0)
    {
        tmpLevel = LEVEL_INFO;
        tmpcontent = Key;
    }
    else
    {
        tmpLevel = tmpLogTip->level;
        tmpcontent = tmpLogTip->content;
    }
    int count = tmplist.count();
    QString tmpString;
    if(count == 1)
        tmpString = QString(tmpcontent).arg(tmplist.at(0));
    else if(count == 2)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1));
    else if(count == 3)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1)).arg(tmplist.at(2));
    else if(count == 4)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1)).arg(tmplist.at(2)).arg(tmplist.at(3));
    else if(count == 5)
        tmpString = QString(tmpcontent).arg(tmplist.at(0)).arg(tmplist.at(1)).arg(tmplist.at(2)).arg(tmplist.at(3)).arg(tmplist.at(4));
    else
        tmpString = tmpcontent;
    Logger(tmpLevel, tmpString);
}

void GetApplicationDirectory()
{
    GlobalPara.m_SoftwareDir = QApplication::applicationDirPath();
    if(!GlobalPara.m_SoftwareDir.endsWith("/"))
        GlobalPara.m_SoftwareDir.append("/");
}

//>@��ӡ��ǰ������Ϣ
QString GetHostPCInfo()
{
    QString tmpString;
#ifdef WINDOWS
    QSysInfo sysInfo;
    QSysInfo::WinVersion tmpVer = sysInfo.windowsVersion();
    switch(tmpVer)
    {
    case QSysInfo::WV_4_0:
        tmpString = QString("OperateSystem  :Win_NT\n");
        break;
    case QSysInfo::WV_5_0:
        tmpString = QString("OperateSystem  :Win_2000");
        break;
    case QSysInfo::WV_5_1:
        tmpString = QString("OperateSystem  :Win_XP\n");
        break;
    case QSysInfo::WV_5_2:
        tmpString = QString("OperateSystem  :Win_2003\n");
        break;
    case QSysInfo::WV_6_0:
        tmpString = QString("OperateSystem  :Win_VISTA\n");
        break;
    case QSysInfo::WV_6_1:
        tmpString = QString("OperateSystem  :Win_WINDOWS7\n");
        break;
    default:break;
    }

    SYSTEM_INFO info;
    GetSystemInfo(&info);
    tmpString.append(QString("CPUs    :%1\nCPU type:%2")
                        .arg(info.dwNumberOfProcessors)
                        .arg(info.dwProcessorType));
#endif
    return tmpString;
}

bool GetScreenInfo()
{
#ifdef IDE
    QDesktopWidget* desktopWidget = QApplication::desktop();
    if(desktopWidget)
    {
        //��ȡ���������С
        GlobalPara.m_DeskRect = desktopWidget->availableGeometry();
        //��ȡ�豸��Ļ��С
        GlobalPara.m_ScreenRect = desktopWidget->screenGeometry();
        //��ȡϵͳ���õ���Ļ��������Ļ������ʽ��ֵΪ1��
        GlobalPara.m_ScreenNum = desktopWidget->screenCount();
        return true;
    }
#endif
    return false;
}

//>@������־�����ļ���������Ҳ������������ļ����Ϸ������˳�����
bool InitLoggerInfo()
{
    return true;
}

//>@��ȡ�����ʼ����Ϣ����ʼ�����
bool GetBootConfigure()
{
    return true;
}

//>@���Թ��ʻ�
bool InitializeLinguist()
{
    QTranslator translator;
    QString qmFile = GlobalPara.m_SoftwareDir + D_RUNTIME_PATH + QString("IDE.qm");
    IDE_TRACE_STR(qmFile);
    translator.load(qmFile);   //���ղŷ�����.qm�ļ����ص�������
    qApp->installTranslator(&translator);
    return true;

//    QStringList environment = QProcess::systemEnvironment();
//    QString str;
//    bool bFinded = false;
//    foreach(str, environment)
//    {
//        if(str.startsWith("QTDIR="))
//        {
//            bFinded = true;
//            break;
//        }
//    }
//    if(bFinded)
//    {
//        str = str.mid(6);
//        bFinded = translator.load("language_" + QLocale::system().name(),str.append("/translations/"));
//        if(!bFinded)
//        {
//            //û��֧�����ĵ�Qt���ʻ������ļ���
//            translator.load("./IDE.qm");   //���ղŷ�����.qm�ļ����ص�������
//        }
//        qApp->installTranslator(&translator);
//        return true;
//    }
//    //�������� QTDIR ����������
//    return false;
}

#ifdef N329
Q_DECLARE_METATYPE(QVariant)
#endif

Global::Global(QObject *parent) :
    QObject(parent)
{
    //>@�Զ����ź��β�
    qRegisterMetaType<AREA_OPERATE>();
    qRegisterMetaType<TCPSERVICE_TYPE>();
    qRegisterMetaType<PH_Type>();
    qRegisterMetaType<LG_Type>();
    qRegisterMetaType<QProcess::ExitStatus>();
    qRegisterMetaType<QVariant>();
    qRegisterMetaType<QHostAddress>();

#ifdef DISPLAY
    m_DeviceManager = 0;
    m_BootParse = 0;
#endif
#ifdef IDE
    statusBar=0;
    docEditStatus=0;
    keyboardStatus=0;
    searchStatus=0;
    progressStatus=0;
    m_OutputMsgList = 0;

    m_ProjectBar = 0;
    m_DockViewToolBar = 0;
    m_SqlToolBar = 0;
    m_TabPageManager = 0;
#endif
#ifdef SURRPORT_QML
    m_QMLEngine = 0;
#endif
    m_SystemAudio = 0;
}

Global::~Global()
{
}

void Global::showMessage(QString pShowMsg)
{
#ifdef IDE
    if(docEditStatus)
        docEditStatus->setText(pShowMsg);
#endif
}

void Global::outputMessage(QString pShowMsg)
{
#ifdef IDE
    if(pShowMsg.isNull() == false)
    {
        if(m_OutputMsgList)
        {
            m_OutputMsgList->append(pShowMsg);
        }
    }
#endif
}

void Global::updateProgress(int pProgress)
{
#ifdef IDE
    if(progressStatus)
    {
        if(pProgress <= 0)
        {
            progressStatus->hide();
            return;
        }
        if(pProgress >= 100)
        {
            progressStatus->hide();
            return;
        }
        progressStatus->setValue(pProgress);
        progressStatus->show();
    }
#endif
}
void Global::updateProgress(QString pShowMsg, int pProgress)
{
#ifdef IDE
    outputMessage(pShowMsg);
    updateProgress(pProgress);
#endif
}

cmdManager::cmdManager() : xmlParse()
{

}

cmdManager::cmdManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = CmdSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != CmdSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(CmdSuffix);
}

bool cmdManager::NewCMD()
{
    bool flag = New();
    if(flag == false)
    {
        IDE_TRACE();
        m_Valid = false;
        return false;
    }
    m_Valid = true;

    appendChild(createProcessingInstruction("xml", XmlHead));

    QDomElement rootElement;
    rootElement = createElement("root");
    appendChild(rootElement);

    QDomElement tmpElement = createElement("FileType");
        tmpElement.appendChild(createTextNode("BST DEV File"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool cmdManager::OpenCMD()
{
    bool flag = Open();
    if(flag == false)
    {
       m_Valid = false;
       return false;
    }
    m_Valid = true;
    //>@Check FileType
    flag = GetItemElement("/FileType");
    if(flag == false)
    {
        m_Valid = false;
        return false;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare(CmdFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool cmdManager::CloseCMD()
{
    SaveCMD();
    return true;
}

bool cmdManager::DelCMD()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool cmdManager::SaveCMD()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool cmdManager::RenameCMD(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(CmdSuffix))
        tmpString.append(CmdSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

//>@   /Ui/Upgrade/Video
bool cmdManager::GetCMDGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Device");
    if(flag)
    {
        m_CfgItemGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            QString tmpCfgNameString = QString("/%1").arg(tmpString);
            m_CfgItemGroup.insert(tmpCfgNameString, tmpElement);//>@��������ҳ��Ԫ��
            //IDE_TRACE_STR(tmpCfgNameString);
        }
    }
    return true;
}

bool ExecCmd(QString pFile)
{
    cmdManager *tmpCmdManager = new cmdManager(pFile);
    if(!tmpCmdManager->OpenCMD())
    {
        tmpCmdManager->m_IsChanged = false;
        delete tmpCmdManager;
        DelFile(pFile);
        return false;
    }
    bool flag = tmpCmdManager->GetItemElement("/CMD");
    if(flag == false)
    {
        tmpCmdManager->m_IsChanged = false;
        delete tmpCmdManager;
        DelFile(pFile);
        return false;       //>@���DEV�ļ����ݲ��Ϸ������˳�
    }
    DelFile(pFile);  //>@ʹ�����˾�ɾ��
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
        if(!tmpOperate.compare("cmd", Qt::CaseInsensitive))
        {
            tmpString.replace(";","&");
            system(tmpString.toLatin1().data());
        }
        QtSleep(10);
    }
    tmpCmdManager->m_IsChanged = false;
    delete tmpCmdManager;
    return true;
}

bool isDevExist(QString pPath)
{
    QProcess tmpProcess;
    QString tmpQuery = QString("ls %1").arg(pPath);  //>@����ls���������ҵ����ļ��������readAllStandardOutput������������ļ��󸽴�һ����\n��,����ļ������ڣ������readAllStandardError�������
    tmpProcess.start(tmpQuery);
    tmpProcess.waitForFinished(1000);
    QString tmpRet = tmpProcess.readAllStandardOutput();
    if(!tmpRet.isEmpty())
        return true;
    return false;
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

//>@����
bool Reboot(bool pSync)
{
#ifdef WINDOWS
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    return true;
#elif LINUX
    if(pSync)
        sync(); // ͬ����������,���������ݻ�д��Ӳ��,�Է����ݶ�ʧ
    return reboot(RB_AUTOBOOT);
#endif
    return false;
}

Global GlobalPara;

#include "global.h"

const QString bstproManager::BstProSuffix = ".bstpro";
const QString bstproManager::BstProFileType = "BST Solution File";

const QString uiManager::UiSuffix = ".ui";
const QString uiManager::UiFileType = "BST UI File";

const QString mdpManager::MdpSuffix = ".mdp";
const QString mdpManager::MdpFileType = "BST MDP File";

const QString sevManager::SevSuffix = ".sev";
const QString sevManager::SevFileType = "BST SEV File";

const QString stcManager::StcSuffix = ".stc";
const QString stcManager::StcFileType = "BST STC File";

const QString rcManager::RcSuffix = ".rc";
const QString rcManager::RcFileType = "BST RC File";

const QString cfgManager::BootFileName = "boot.xml";
const QString cfgManager::CfgFileType = "BST IDE File";

const QString cmdManager::CmdSuffix = ".cmd";
const QString cmdManager::CmdFileType = "BST CMD File";

const QString devManager::DevSuffix = ".dev";
const QString devManager::DevFileType = "BST DEV File";

const QString mapManager::MapSuffix = ".map";
const QString mapManager::MapFileType = "BST MAP File";

const QString devInfoManager::DevInfoSuffix = ".devinfo";
const QString devInfoManager::DevInfoFileType = "BST DEVINFO File";

const QString fontManager::FontSuffix = ".font";
const QString fontManager::FontFileType = "BST FONT File";

const QString updateManager::UpdateSuffix = ".update";
const QString updateManager::UpdateFileType = "BST UPDATE File";

void GetApplicationDirectory()
{
    GlobalPara.m_SoftwareDir = QApplication::applicationDirPath();
    if(!GlobalPara.m_SoftwareDir.endsWith("/"))
        GlobalPara.m_SoftwareDir.append("/");
}

bool GetScreenInfo()
{
#if (defined(IDE) || defined(PRODUCT))
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


//>@��ȡ�����ʼ����Ϣ����ʼ������
bool GetBootConfigure()
{
    QString BootFile = GlobalPara.m_SoftwareDir + D_RUNTIME_PATH + cfgManager::BootFileName;
    cfgManager *tmpCfgManager = new cfgManager(BootFile);
    if(tmpCfgManager->m_Valid == false)
    {
        IDE_TRACE_STR(BootFile);
        return false;
    }
    tmpCfgManager->m_IsChanged = false;
    delete tmpCfgManager;
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
    qRegisterMetaType<E_DEVMODE>();
    qRegisterMetaType<QVariant>();
    qRegisterMetaType<BUTTONSTATE>();
#ifdef DISPLAY
    m_DeviceManager = 0;
    m_BootParse = 0;
#endif
#if (defined(IDE) || defined(PRODUCT))
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
}

Global::~Global()
{
}

void Global::showMessage(QString pShowMsg)
{
#if (defined(IDE) || defined(PRODUCT))
    if(docEditStatus)
        docEditStatus->setText(pShowMsg);
#endif
}

void Global::outputMessage(QString pShowMsg)
{
#if (defined(IDE) || defined(PRODUCT))
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
#if (defined(IDE) || defined(PRODUCT))
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
#if (defined(IDE) || defined(PRODUCT))
    outputMessage(pShowMsg);
    updateProgress(pProgress);
#endif
}

bstproManager::bstproManager(QString fileName) : xmlParse(fileName)
{
    IDE_TRACE_STR(m_FileDir);
    IDE_TRACE_STR(m_FileName);
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = BstProSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != BstProSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(BstProSuffix);
}

bool bstproManager::NewPro()
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
    tmpElement.appendChild(createTextNode("BST Solution File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("FileList");
        QDomElement tmpSubElement = createElement("UI");
            tmpSubElement.appendChild(createTextNode(D_UINAME));
        tmpElement.appendChild(tmpSubElement);
        tmpSubElement = createElement("MAP");
            tmpSubElement.appendChild(createTextNode(D_MAPNAME));
        tmpElement.appendChild(tmpSubElement);
        tmpSubElement = createElement("STC");
            tmpSubElement.appendChild(createTextNode(D_STCNAME));
        tmpElement.appendChild(tmpSubElement);
        tmpSubElement = createElement("STO");
            tmpSubElement.appendChild(createTextNode(D_STONAME));
        tmpElement.appendChild(tmpSubElement);
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool bstproManager::OpenPro()
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
    if(tmpString.compare(BstProFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool bstproManager::ClosePro()
{
    SavePro();
    return true;
}

bool bstproManager::DelPro()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool bstproManager::SavePro()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool bstproManager::RenamePro(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(BstProSuffix))
        tmpString.append(BstProSuffix);
    bool flag = Rename(tmpString);
    if(flag)
    {
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    }
    return flag;
}

//>@pFileName������һ����Ե�ַ
bool bstproManager::AddFile(QString pFileName)
{
    return AddFile(getFileSuffix(pFileName), pFileName);
}

//>@pFileName������һ����Ե�ַ
bool bstproManager::AddFile(QString pKey, QString pFileName)
{
    if(!m_Valid || pKey.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    if(GetItemElement("/FileList") == false)
    {
        IDE_TRACE();
        return false;
    }
    QString tmpFileName = getFileName(pFileName);
    QString tmpKey = pKey.toUpper();
    QDomElement tmpElement = itemElement.firstChildElement(tmpKey);
    if(tmpElement.isNull() == false)
    {
        return ModifyItemElement(tmpElement, tmpFileName);
    }
    else
    {
        tmpElement = createElement(tmpKey);
        tmpElement.appendChild(createTextNode(tmpFileName));
        itemElement.appendChild(tmpElement);
        m_IsChanged = true;
        return true;
    }
    return true;
}

bool bstproManager::DelFile(QString pKey)
{
    if(pKey.isEmpty())
        return false;
    if(GetItemElement("/FileList") == false)
        return false;
    QString tmpKey = pKey.toUpper();
    QDomElement tmpElement = itemElement.firstChildElement(tmpKey);
    if(tmpElement.isNull())
        return true;
    QDomElement tmpParent = tmpElement.parentNode().toElement();
    if(tmpParent.isNull())
        return false;
    tmpParent.removeChild(tmpElement);
    m_IsChanged = true;
    return true;
}

QString bstproManager::GetFile(QString pKey)
{
    if(pKey.isEmpty())
        return QString();
    if(GetItemElement(QString("/FileList/%1").arg(pKey.toUpper())) == false)
        return QString();
    if(itemElement.isNull())
        return QString();
    return itemElement.text();
}

cfgManager::cfgManager(QString fileName) : xmlParse(fileName)
{
    m_Valid = false;
    QFile file(fileName);
    bool flag = file.exists();
    if(flag == false)
    {
        IDE_TRACE();
        return ;
    }
    flag = Open();
    if(flag == false)
    {
       IDE_TRACE();
       return ;
    }
    //>@Check filetype
    flag = GetItemElement("/FileType");
    if(flag == false)
    {
        IDE_TRACE();
        return;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare(CfgFileType, Qt::CaseInsensitive) != 0)
    {
        IDE_TRACE_STR(tmpString);
        return;
    }
    m_Valid = true;

    getNewCategory();
    getSolutionCategory();
    getFileCategory();
    getComponetCategory();
    getCfgparaGroup();
    getTestcaseGroup();
}

bool cfgManager::getNewCategory()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/NewFile");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }
    GlobalPara.NewCategoryGroup.clear();
    QString tmpString;
    QDomElement tmpElement;
    QDomNode tmpNode;
    QDomNodeList newCategoryNode = GetChildNodes();
    for(int i=0;i < newCategoryNode.count();++i)
    {
        tmpNode = newCategoryNode.at(i);
        tmpElement = tmpNode.toElement();
        tmpString = tmpElement.tagName();
        GlobalPara.NewCategoryGroup.insert(tmpString, tmpElement);
    }
    return true;
}

bool cfgManager::getSolutionCategory()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/NewFile/Solution");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }

    GlobalPara.SolutionCategoryGroup.clear();

    QString tmpString;
    QDomElement tmpElement;
    QDomNode tmpNode;
    QDomNodeList solutionCategoryNode = itemElement.childNodes();
    for(int i=0;i < solutionCategoryNode.count();++i)
    {
        SolutionCategory tmpSolutionCategory;
        tmpNode = solutionCategoryNode.at(i);
        tmpElement = tmpNode.toElement();
        tmpSolutionCategory.Icon = tmpElement.attribute("img");
        tmpSolutionCategory.Resolution = tmpElement.firstChildElement("Resolution").text();
        tmpString = tmpElement.firstChildElement("Video").text();
        flag = (tmpString.compare(QString("yes"), Qt::CaseInsensitive) == 0)?true:false;
        tmpSolutionCategory.SupportVideo = flag;
        tmpString = tmpElement.firstChildElement("OpenGL").text();
        flag = (tmpString.compare(QString("yes"), Qt::CaseInsensitive) == 0)?true:false;
        tmpSolutionCategory.SupportOpenGL = flag;

        //>@protocol
        tmpString = tmpElement.firstChildElement("Protocol").text();
        tmpSolutionCategory.Protocol = SplitString(tmpString, ";");

        tmpString = tmpElement.firstChildElement("Description").text();
        tmpSolutionCategory.Description.append(tmpString);
        tmpString = tmpElement.tagName();
        GlobalPara.SolutionCategoryGroup.insert(tmpString, tmpSolutionCategory);
    }
    return true;
}

bool cfgManager::getFileCategory()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/NewFile/File");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }

    GlobalPara.FileCategoryGroup.clear();

    IDE_TRACE();

    QString tmpString;
    QDomElement tmpElement;
    QDomNode tmpNode;
    QDomNodeList fileCategoryNode = itemElement.childNodes();
    for(int i=0;i < fileCategoryNode.count();i++)
    {
        FileCategory tmpFileCategory;
        tmpNode = fileCategoryNode.at(i);
        tmpElement = tmpNode.toElement();
        tmpFileCategory.Icon = tmpElement.attribute("img");
        tmpFileCategory.Suffix = tmpElement.firstChildElement("Suffix").text();
        tmpFileCategory.Description = tmpElement.firstChildElement("Description").text();
        tmpString = tmpElement.tagName();
        GlobalPara.FileCategoryGroup.insert(tmpString, tmpFileCategory);
    }
    return true;
}

//>@��ȡboot.xml�ĵ��������״̬����Ӧ����Ч�Լ���Դ
void cfgManager::getEffectAndRcTypes(QDomElement pElement, ComCategory *pComCategory)
{
    pComCategory->StateMachines.clear();
    pComCategory->Icon = pElement.attribute("img");
    QDomNodeList StateMachineNodes = pElement.childNodes();
    for(int i=0;i < StateMachineNodes.count();++i)
    {
        QDomNode tmpNode = StateMachineNodes.at(i);
        QDomElement tmpElement = tmpNode.toElement();
        if(tmpElement.isNull())
        {
            IDE_TRACE();
            continue;
        }
        if(tmpElement.tagName() == QString("Control"))
            continue;
        STATEMACHINE *tmpState = new STATEMACHINE;
        tmpState->StateName = tmpElement.tagName();
        //IDE_TRACE_STR(tmpState->StateName);
        QDomNodeList EffectNodes = tmpElement.childNodes();
        for(int j=0;j < EffectNodes.count();++j)
        {
            QDomNode tmpEffectNode = EffectNodes.at(j);
            QDomElement tmpEffectElement = tmpEffectNode.toElement();
            if(tmpEffectElement.isNull())
            {
                //IDE_TRACE();
                continue;
            }
            QString tmpEffectName = tmpEffectElement.tagName();
            if(tmpEffectName.isEmpty() || tmpEffectName.compare(D_RCINVALID, Qt::CaseInsensitive)==0)
            {
                //IDE_TRACE();
                continue;
            }
            EFFECT *tmpEffect = new EFFECT;
            tmpEffect->EffectName = tmpEffectName;
            //>@�鿴�Ƿ���Parameter����
            QString tmpEffectPara = tmpEffectElement.attribute("Parameter");
            if(tmpEffectPara.isEmpty() == false)
            {
                QStringList tmpParaList = tmpEffectPara.split(';');
                for(int m=0;m<tmpParaList.count();m++)
                {
                    QStringList tmpSubParaList = tmpParaList.at(m).split(':');
                    if(tmpSubParaList.count()!=2)
                        continue;
                    EFFECTPARA *tmpEffectPara = new EFFECTPARA;
                    tmpEffectPara->ParaName = tmpSubParaList.at(0);
                    tmpEffectPara->ParaValue = tmpSubParaList.at(1);
                    tmpEffect->EffectParas.append(tmpEffectPara);
                }
            }
            QDomNodeList RcNodes = tmpEffectElement.childNodes();
            for(int k=0;k < RcNodes.count();++k)
            {
                QDomNode tmpRcNode = RcNodes.at(k);
                QDomElement tmpRcElement = tmpRcNode.toElement();
                if(tmpRcElement.isNull())
                {
                    //IDE_TRACE();
                    continue;
                }
                RESOURCE *tmpResource = new RESOURCE;
                //IDE_TRACE_STR(tmpResource.RcName);
                tmpResource->NickNames.append(tmpRcElement.tagName());
                QStringList tmpNickNameList = tmpRcElement.attribute("nickname").split('/', QString::SkipEmptyParts);
                if(!tmpNickNameList.isEmpty())
                    tmpResource->NickNames.append(tmpNickNameList);
                tmpResource->RcTypes = tmpRcElement.attribute("types").split(';');
                tmpEffect->Resources.append(tmpResource);
            }
            tmpState->Effects.append(tmpEffect);
        }
        pComCategory->StateMachines.append(tmpState);
        IDE_TRACE_STR(tmpState->StateName);
    }
}

//>@��ȡBoot.xml�ļ��е�Component����ʹ��/FuncInfo/Floor�ĸ�ʽ���д洢��
bool cfgManager::getComponetCategory()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Componet");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }
    GlobalPara.m_ComCategoryGroup.clear();

    COM_TYPE tmpComType;
    QDomElement tmpElement = itemElement.firstChildElement("Desktop");
    if(tmpElement.isNull())
    {
        IDE_TRACE();
        return false;
    }
    ComCategory *uComCategory = new ComCategory;
    uComCategory->ParentType = COM_INVALID;
    uComCategory->CurType = COM_DESKTOP;
    getEffectAndRcTypes(tmpElement, uComCategory);
    GlobalPara.m_ComCategoryGroup.insert(QString("/"), uComCategory);

    tmpElement = itemElement.firstChildElement("SubUI");
    if(tmpElement.isNull())
    {
        IDE_TRACE();
        return false;
    }
    QDomNodeList comCategoryNode = tmpElement.childNodes();
    for(int i=0;i < comCategoryNode.count();++i)
    {
        QDomNode tmpNode = comCategoryNode.at(i);
        QDomElement tmpSubElement = tmpNode.toElement();
        if(tmpSubElement.isNull())
        {
            IDE_TRACE();
            continue;
        }
        ComCategory *tmpComCategory = new ComCategory;
        tmpComCategory->ParentType = COM_DESKTOP;
        QString tmpString = tmpSubElement.tagName();
        tmpComType = getComponetType(tmpString);
        tmpComCategory->CurType = tmpComType;
        QString tmpNameString = QString("/%1").arg(tmpString);
        IDE_TRACE_STR(tmpNameString);
        getEffectAndRcTypes(tmpSubElement, tmpComCategory);
        GlobalPara.m_ComCategoryGroup.insert(tmpNameString, tmpComCategory);
        QDomElement tmptmpElement = tmpSubElement.firstChildElement(QString("Control"));
        if(tmptmpElement.isNull())
        {
            IDE_TRACE();
            continue;
        }
        QDomNodeList subComCategoryNode = tmptmpElement.childNodes();
        for(int j=0;j<subComCategoryNode.count();j++)
        {
            QDomNode tmpChildNode = subComCategoryNode.at(j);
            QDomElement tmpChildElement = tmpChildNode.toElement();
            if(tmpChildElement.isNull())
            {
                IDE_TRACE();
                continue;
            }
            ComCategory *tmpSubComCategory = new ComCategory;
            tmpSubComCategory->ParentType = tmpComType;
            QString tmpChildString = tmpChildElement.tagName();
            tmpSubComCategory->CurType = getComponetType(tmpChildString);
            QString tmpChildNameString = QString("%1/%2").arg(tmpNameString).arg(tmpChildString);
            IDE_TRACE_STR(tmpChildNameString);
            getEffectAndRcTypes(tmpChildElement, tmpSubComCategory);
            GlobalPara.m_ComCategoryGroup.insert(tmpChildNameString, tmpSubComCategory);
        }
    }
    return true;
}

bool cfgManager::getTestcaseGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/TestCase");
    if(flag)
    {
        GlobalPara.m_TestCaseGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpString = tmpElement.tagName();
            if(tmpString.isEmpty())
                continue;
            QString tmpSubCfgString = QString("/TestCase/%1").arg(tmpString);
            flag = GetItemElement(tmpSubCfgString);
            if(!flag)
                continue;
            QDomNodeList SubCfgItemNodeList = itemElement.childNodes();
            for(int j=0;j<SubCfgItemNodeList.count();j++)
            {
                tmpElement = SubCfgItemNodeList.at(j).toElement();
                if(tmpElement.isNull())
                    continue;
                QString tmpSubString = tmpElement.tagName();
                if(tmpSubString.isEmpty())
                    continue;
                GlobalPara.m_TestCaseGroup.insert(QString("/%1/%2").arg(tmpString).arg(tmpSubString), tmpElement);
            }
        }
    }
    return true;
}

bool cfgManager::getCfgparaGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/CfgPara");
    if(flag)
    {
        GlobalPara.m_ConfigInfoGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();  //>@����Physical��Logic
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpString = tmpElement.tagName();
            if(tmpString.isEmpty())
                continue;
            QString tmpPath = QString("/%1").arg(tmpString);
            GlobalPara.m_ConfigInfoGroup.insert(tmpPath, tmpElement);
            IDE_TRACE_STR(tmpPath);
            QString tmpSubCfgString = QString("/CfgPara/%1").arg(tmpString);
            flag = GetItemElement(tmpSubCfgString);
            if(!flag)
                continue;
            QDomNodeList SubCfgItemNodeList = itemElement.childNodes();   //>@����CPU����NET��
            for(int j=0;j<SubCfgItemNodeList.count();j++)              //>@��������ҳ������Ԫ��
            {
                tmpElement = SubCfgItemNodeList.at(j).toElement();
                if(tmpElement.isNull())
                    continue;
                QString tmpSubString = tmpElement.tagName();
                if(tmpSubString.isEmpty())
                    continue;
                QString tmpSubPath = QString("/%1/%2").arg(tmpString).arg(tmpSubString);
                GlobalPara.m_ConfigInfoGroup.insert(tmpSubPath, tmpElement);
                IDE_TRACE_STR(tmpSubPath);
                if(tmpString.compare("Middleware")==0)
                {
                    GlobalPara.m_MWonfigInfoGroup.insert(GetMWType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("Physical")==0)
                {
                    GlobalPara.m_PHonfigInfoGroup.insert(GetPHType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("Logic")==0)
                {
                    GlobalPara.m_LGonfigInfoGroup.insert(GetLGType(tmpSubString), tmpElement);
                }
            }
        }
    }
    return true;
}


QUiManager::QUiManager(QString pFileName) :
    xmlParse(pFileName)
{
    m_Valid = Open();
    if(m_Valid)
        m_Valid = GetItemElement("/widget/widget");
    if(!m_Valid)
    {
        IDE_TRACE();
        return;
    }

    GetComponentGroup(itemElement);
}

void QUiManager::GetComponentGroup(QDomElement pElement)
{
    QDomElement tmpElement;
    QDomNodeList SubuiNodeList = pElement.childNodes();
    for(int i=0;i<SubuiNodeList.count();i++)
    {
        tmpElement = SubuiNodeList.at(i).toElement();
        QString tmpString = tmpElement.tagName();
        if(tmpString.compare("widget") ==0)
        {
            QString tmpClass = tmpElement.attribute("class");
            if(tmpClass.startsWith("Widget"))
            {
                COM_TYPE tmpComType = getComponetType(tmpElement.attribute("name"));
                if(tmpComType < COM_FLOOR)
                    continue;
                m_ComElementGroup.insert(tmpComType, tmpElement);
            }
            else if(tmpClass.compare("QScrollArea") ==0)
            {
                GetComponentGroup(tmpElement);
            }
        }
    }
}

QRect QUiManager::GetRect(COM_TYPE pType)
{
    QDomElement tmpElement = m_ComElementGroup.value(pType);
    if(tmpElement.isNull())
        return QRect();

    QDomElement tmpProtElement;
    QDomNodeList SubuiNodeList = tmpElement.childNodes();
    for(int i=0;i<SubuiNodeList.count();i++)
    {
        tmpProtElement = SubuiNodeList.at(i).toElement();
        QString tmpString = tmpProtElement.attribute("name");
        if(tmpString.compare("geometry") ==0)
        {
            QDomElement tmpRectElement = tmpProtElement.firstChildElement("rect");
            if(tmpRectElement.isNull())
                return QRect();
            return QRect(tmpRectElement.firstChildElement("x").text().toInt(),
                         tmpRectElement.firstChildElement("y").text().toInt(),
                         tmpRectElement.firstChildElement("width").text().toInt(),
                         tmpRectElement.firstChildElement("height").text().toInt());
        }
    }
    return QRect();
}

void QUiManager::ModifyRect(COM_TYPE pType, QRect pRect)
{
    QDomElement tmpElement = m_ComElementGroup.value(pType);
    if(tmpElement.isNull())
        return;
    QDomElement tmpRectElement = tmpElement.firstChildElement("rect");
    if(tmpRectElement.isNull())
        return;
    ModifyItemElement(tmpRectElement.firstChildElement("x"), QString::number(pRect.x()));
    ModifyItemElement(tmpRectElement.firstChildElement("y"), QString::number(pRect.y()));
    ModifyItemElement(tmpRectElement.firstChildElement("width"), QString::number(pRect.width()));
    ModifyItemElement(tmpRectElement.firstChildElement("height"), QString::number(pRect.height()));
}

uiManager::uiManager(QString fileName) :
    xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = UiSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != UiSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(UiSuffix);
}

bool uiManager::NewUI()
{
    bool flag = New();
    if(flag == false)
    {
        IDE_TRACE();
        m_Valid = false;
        return false;
    }
    m_Valid = true;
    m_ComElementGroup.clear();

    appendChild(createProcessingInstruction("xml", XmlHead));

    QDomElement rootElement;
    rootElement = createElement("root");
    appendChild(rootElement);

    QDomElement tmpElement = createElement("FileType");
        tmpElement.appendChild(createTextNode("BST UI File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Solution");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Direction");
        tmpElement.appendChild(createTextNode("HV"));
        m_DirectionPermisson = HVBoth;
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Ui");
        QDomElement tmpChildElement = createElement("SubUI");
        tmpElement.appendChild(tmpChildElement);
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();

    GetComponentGroup();
    return true;
}

bool uiManager::OpenUI()
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
    if(tmpString.compare(UiFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    m_DirectionPermisson = GetDirectionPermisson();
    GetComponentGroup();
    return true;
}

bool uiManager::CloseUI()
{
    SaveUI();
    return true;
}

bool uiManager::DelUI()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool uiManager::SaveUI()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    return Save();
}

bool uiManager::RenameUI(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(UiSuffix))
        tmpString.append(UiSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool uiManager::isValid()
{
    if(!m_Valid)
        return false;
    if(m_FileDir.endsWith('/') == false)
        m_FileDir.append('/');
    QDir tmpDir(m_FileDir);
    if(tmpDir.exists() == false)
        return tmpDir.mkpath(m_FileDir);
    return true;
}

QString uiManager::GetSolution()
{
    return GetItemElementValue("/Solution");
}

bool uiManager::SetSolution(QString pSolution)
{
    return ModifyItemElement("/Solution", pSolution);
}

DIRECTION uiManager::GetDirectionPermisson()
{
    return GetDirection(GetItemElementValue("/Direction"));
}

bool uiManager::SetDirectionPermisson(DIRECTION pDirection)
{
    if(!m_Valid)
        return false;
    QString tmpDirString = GetDirectionString(pDirection);
    if(tmpDirString.isEmpty())
        return false;
    m_DirectionPermisson = pDirection;
    return ModifyItemElement("/Direction", tmpDirString);
}

//>@���ڸ���Global���е�m_ComElementGroup
void uiManager::GetComponentGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return;
    }
    bool flag = GetItemElement("/Ui/Desktop");
    if(flag)
    {
        m_ComElementGroup.clear();
        m_ComElementGroup.insert(COM_DESKTOP, itemElement);//>@Desktop
    }
    else
    {
        IDE_TRACE();
        return;
    }

    flag = GetItemElement("/Ui/SubUI");
    if(flag)
    {
        QDomElement tmpElement;
        QDomNodeList SubuiNodeList = itemElement.childNodes();
        for(int i=0;i<SubuiNodeList.count();i++)
        {
            tmpElement = SubuiNodeList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            COM_TYPE tmpComType = getComponetType(tmpString);
            if(tmpComType < COM_DESKTOP)
                continue;
            m_ComElementGroup.insert(tmpComType, tmpElement);//>@����subuiԪ��
            //IDE_TRACE_STR(tmpSubuiString);
            QString tmpControlString = QString("/Ui/SubUI/%1/Control").arg(tmpString);
            flag = GetItemElement(tmpControlString);
            if(flag)
            {
                QDomNodeList ControlNodeList = itemElement.childNodes();
                for(int i=0;i<ControlNodeList.count();i++)              //>@����subui����controlԪ��
                {
                    tmpElement = ControlNodeList.at(i).toElement();
                    QString tmpSubString = tmpElement.tagName();
                    COM_TYPE tmpSubComType = getComponetType(tmpSubString);
                    if(tmpSubComType < COM_DESKTOP)
                        continue;
                    m_ComElementGroup.insert(tmpSubComType, tmpElement);
                    //IDE_TRACE_STR(tmpSubControlString);
                }
            }
        }
    }
}

bool uiManager::GetComRect(int pLayout, QDomElement pComElement, QRect &pHRect, QRect &pVRect)
{
    if(pComElement.isNull())
        return false;
    QString tmpRectName;
    if(pLayout <= 0)
        tmpRectName = QString("Rect");
    else
        tmpRectName = QString("Rect%1").arg(pLayout);
    QDomElement tmpRectElement = pComElement.firstChildElement(tmpRectName);
    if(tmpRectElement.isNull())
    {
        tmpRectElement = pComElement.firstChildElement(QString("Rect"));
        if(tmpRectElement.isNull())
            return false;
    }
    QDomElement tmpElement = tmpRectElement.firstChildElement("Horizontal");
    if(tmpElement.isNull())
        return false;
    pHRect = GetRectFromString(tmpElement.text());
    tmpElement = tmpRectElement.firstChildElement("Vertical");
    if(tmpElement.isNull())
        return false;
    pVRect = GetRectFromString(tmpElement.text());
    return true;
}

QRect uiManager::GetComRect(int pLayout, QDomElement pComElement, DIRECTION pDirection)
{
    if(pComElement.isNull())
        return QRect();
    QString tmpRectName;
    if(pLayout <= 0)
        tmpRectName = QString("Rect");
    else
        tmpRectName = QString("Rect%1").arg(pLayout);
    QDomElement tmpRectElement = pComElement.firstChildElement(tmpRectName);
    if(tmpRectElement.isNull())
    {
        tmpRectElement = pComElement.firstChildElement(QString("Rect"));
        if(tmpRectElement.isNull())
            return QRect();
    }
    QDomElement tmpElement;
    if(pDirection == Horizontal)
        tmpElement = tmpRectElement.firstChildElement("Horizontal");
    else
        tmpElement = tmpRectElement.firstChildElement("Vertical");
    if(tmpElement.isNull())
        return QRect();
    return GetRectFromString(tmpElement.text());
}

bool uiManager::ModifyComRect(int pLayout, QDomElement pComElement, QRect pRect, DIRECTION pDirection, bool pForceNew)
{
    if(pComElement.isNull() || pRect.isNull())
        return false;
    COM_TYPE tmpComType = getComponetType(pComElement.tagName());
    if(tmpComType < COM_DESKTOP)
        return false;
    QString tmpRectName;
    if(pLayout <= 0)
        tmpRectName = QString("Rect");
    else
        tmpRectName = QString("Rect%1").arg(pLayout);
    QDomElement tmpRectElement = pComElement.firstChildElement(tmpRectName);
    if(tmpRectElement.isNull())
    {
        if(pForceNew)  //>@���ǿ�ƴ����µ�
        {
            //>@�����ڣ��򴴽�
            tmpRectElement = createElement("Rect");
            if(tmpRectElement.isNull())
                return false;
            pComElement.appendChild(tmpRectElement);
        }
        else
        {
            tmpRectElement = pComElement.firstChildElement(QString("Rect"));
            if(tmpRectElement.isNull())
                return false;
        }
    }
    //>@
    if(pRect.left()<0 || pRect.top()<0 || pRect.width()<=0 || pRect.height()<=0)
    {
        IDE_TRACE();
        return false;
    }
    //>@�鿴���������Ƿ���ڣ��������򴴽�
    //>@��ͬ���޸���һ����������ʱ��ֻ�޸Ŀ��߶ȣ����޸����Ͻ����ꣻ
    //>@����������²������꣬������Ͻ�������Ϊ0��������޸����꣬�򱣳����Ͻ����겻��
    QDomElement tmpHElement = tmpRectElement.firstChildElement("Horizontal");
    QDomElement tmpVElement = tmpRectElement.firstChildElement("Vertical");
    if(tmpHElement.isNull())
    {
        tmpHElement = createElement("Horizontal");
        tmpRectElement.appendChild(tmpHElement);
    }
    if(tmpVElement.isNull())
    {
        tmpVElement = createElement("Vertical");
        tmpRectElement.appendChild(tmpVElement);
    }
    QString tmpHOldString = tmpHElement.text();
    QString tmpVOldString = tmpVElement.text();
    QString tmpHString, tmpVString;
    if(pDirection == Horizontal)
    {
        tmpHString = QString("%1,%2,%3,%4").arg(pRect.left()).arg(pRect.top()).arg(pRect.width()).arg(pRect.height());
        if(!ModifyItemElement(tmpHElement, tmpHString))
        {
            IDE_TRACE_STR(tmpHString);
            return false;
        }
        if(canSwitch())
        {
            QRect tmpRect = GetRectFromString(tmpVOldString);
            if(tmpComType == COM_DESKTOP)
            {
                if(tmpRect.isNull())
                    tmpVString = QString("0,0,%1,%2").arg(pRect.height()).arg(pRect.width());
                else
                    tmpVString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top()).arg(pRect.height()).arg(pRect.width());
            }
            else
            {
                if(tmpRect.isNull())
                    tmpVString = QString("0,0,%1,%2").arg(pRect.width()).arg(pRect.height());
                else
                    tmpVString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top()).arg(pRect.width()).arg(pRect.height());
            }
            if(!ModifyItemElement(tmpVElement, tmpVString))
            {
                IDE_TRACE_STR(tmpVString);
                ModifyItemElement(tmpHElement, tmpHOldString);
                return false;
            }
        }
    }
    else if(pDirection == Vertical)
    {
        tmpVString = QString("%1,%2,%3,%4").arg(pRect.left()).arg(pRect.top()).arg(pRect.width()).arg(pRect.height());
        if(!ModifyItemElement(tmpVElement, tmpVString))
        {
            IDE_TRACE_STR(tmpVString);
            return false;
        }
        if(canSwitch())
        {
            QRect tmpRect = GetRectFromString(tmpHOldString);
            if(tmpComType == COM_DESKTOP)
            {
                if(tmpRect.isNull())
                    tmpHString = QString("0,0,%1,%2").arg(pRect.height()).arg(pRect.width());
                else
                    tmpHString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top()).arg(pRect.height()).arg(pRect.width());
            }
            else
            {
                if(tmpRect.isNull())
                    tmpHString = QString("0,0,%1,%2").arg(pRect.width()).arg(pRect.height());
                else
                    tmpHString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top()).arg(pRect.width()).arg(pRect.height());
            }
            if(!ModifyItemElement(tmpHElement, tmpHString))
            {
                IDE_TRACE_STR(tmpHString);
                ModifyItemElement(tmpVElement, tmpVOldString);
                return false;
            }
        }
    }
    return true;
}

QDomNodeList uiManager::GetComStateList(QDomElement pComElement)
{
    if(pComElement.isNull())
        return QDomNodeList();
    QDomElement tmpSMElement = pComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return QDomNodeList();
    return tmpSMElement.childNodes();
}

QDomElement uiManager::GetComState(QDomElement pComElement, QString pStateName, bool pCreateFlag)
{
    if(pComElement.isNull() || pStateName.isEmpty())
        return QDomElement();
    QDomElement tmpSMElement = pComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return QDomElement();
    QDomElement tmpStateElement = tmpSMElement.firstChildElement(pStateName);
    if(pCreateFlag && tmpStateElement.isNull())
    {
        tmpStateElement = createElement(pStateName);
        tmpStateElement.setAttribute(D_EFFECTTYPE, "SignalMap");
        tmpSMElement.appendChild(tmpStateElement);
    }
    return tmpStateElement;
}

bool uiManager::ModifyEffectType(QDomElement pStateElement, QString pEffectType)
{
    if(pStateElement.isNull())
        return false;
    //>@������յ���Ч����
    if(!pEffectType.isEmpty())
    {
        QString tmpEffectType = pStateElement.attribute(D_EFFECTTYPE);
        //>@��������õ���Ч���Ͳ�һ��������Ҫɾ��UI�е������ӽڵ�
        if(pEffectType.compare(tmpEffectType))
        {
            ClearItemElement(pStateElement);
            pStateElement.setAttribute(D_EFFECTTYPE, pEffectType);
            pStateElement.removeAttribute(D_EFFECTPARA);
            return true;
        }
    }
    return true;  //>@�������Ҫ�޸ģ�����ȷ
}

QDomElement uiManager::ModifyComState(QDomElement pComElement, QString pStateName, QString pEffectType, QString pEffectPara)
{
    if(pComElement.isNull() || pStateName.isEmpty())
        return QDomElement();
    QDomElement tmpSMElement = pComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
    {
        //>@�����ڣ��򴴽�
        tmpSMElement = createElement("StateMachine");
        if(tmpSMElement.isNull())
            return QDomElement();
        pComElement.appendChild(tmpSMElement);
    }
    QDomElement tmpStateElement = tmpSMElement.firstChildElement(pStateName);
    if(tmpStateElement.isNull())
    {
        //>@�����ڣ��򴴽�
        tmpStateElement = createElement(pStateName);
        if(tmpStateElement.isNull())
            return QDomElement();
        tmpSMElement.appendChild(tmpStateElement);
    }
    //>@�޸���Ч��������޸ĳɹ�������²�����
    //>@ע�⣬������޸ĵ���Ч����ͬ������Ϊ�޸Ĳ��ɹ�
    if(ModifyEffectType(tmpStateElement, pEffectType))
    {
        ModifyEffectPara(tmpStateElement, pEffectPara);
    }
    return tmpStateElement;
}

bool uiManager::ModifyEffectPara(QDomElement pStateElement, QString pEffectPara)
{
    if(pStateElement.isNull())
        return false;
    if(!pEffectPara.isEmpty())
    {
        QString tmpOldEffectPara = pStateElement.attribute(D_EFFECTPARA);
        if(tmpOldEffectPara.compare(pEffectPara))
        {
            QStringList tmpParaList = pEffectPara.split(';');
            QStringList tmpOldParaList = tmpOldEffectPara.split(';');
            for(int m=0;m<tmpParaList.count();m++)
            {
                QString tmpPara = tmpParaList.at(m);
                QStringList tmpValueList = tmpPara.split(':');
                if(tmpValueList.count()!=2)
                    continue;
                QString tmpName = tmpValueList.at(0);
                if(tmpName.isEmpty())
                    continue;
                for(int n=0;n<tmpOldParaList.count();n++)
                {
                    QString tmpOldPara = tmpOldParaList.at(n);
                    if(tmpOldPara.startsWith(tmpName))
                    {
                        if(tmpPara.contains('@'))
                        {
                            tmpOldParaList.replace(n, tmpPara);
                        }
                        else if(tmpOldPara.contains('@'))
                        {
                            tmpOldParaList.replace(n, tmpPara + QString('@') + tmpOldPara.split('@').last());
                        }
                        else
                        {
                            tmpOldParaList.replace(n, tmpPara);
                        }
                        break;
                    }
                }
            }
            QString tmpNewString = tmpOldParaList.join(QString(";"));
            pStateElement.setAttribute(D_EFFECTPARA, tmpNewString);
            return true;
        }
    }
    return true;  //>@�������Ҫ�޸ģ�����ȷ
}

QDomElement uiManager::GetParComDom(QString pComPath)
{
    COM_TYPE tmpType = getParentComType(getCurComType(pComPath));
    return m_ComElementGroup.value(tmpType);
}

QDomElement uiManager::GetParComDom(QDomElement pComElement)
{
    QDomElement tmpElement = pComElement.parentNode().toElement();
    if(tmpElement.isNull())
        return QDomElement();
    QString tmpName = tmpElement.tagName();
    if(tmpName.compare("Ui") == 0)  //>@Ϊ�������
    {
        return QDomElement();
    }
    else if(tmpName.compare("Control") == 0)  //>@Ϊ�������
    {
        return tmpElement.parentNode().toElement();
    }
    else if(tmpName.compare("SubUI") == 0)  //>@Ϊ�������
    {
        return tmpElement.parentNode().firstChildElement("Desktop");
    }
    return QDomElement();
}

bool uiManager::AddMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix)
{
    if(pMapStateElement.isNull())
        return false;
    QString tmpSrc;
    if(!pPrefix.isEmpty())
        tmpSrc = pPrefix+pMap.mSrc;
    QDomElement pMapElement = pMapStateElement.firstChildElement(tmpSrc);
    if(pMapElement.isNull())
    {
        pMapElement = createElement(tmpSrc);
        pMapStateElement.appendChild(pMapElement);
    }
    return ModifyItemElement(pMapElement, pMap.mMap);
}

bool uiManager::DelMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix)
{
    if(pMapStateElement.isNull())
        return false;
    QString tmpSrc;
    if(!pPrefix.isEmpty())
        tmpSrc = pPrefix+pMap.mSrc;
    QDomElement pMapElement = pMapStateElement.firstChildElement(tmpSrc);
    if(!pMapElement.isNull())
        pMapStateElement.removeChild(pMapElement);
    return true;
}

QList<RC_MAP> uiManager::GetMapList(QDomElement pComElement, QString &pMapType, QString pPrefix)
{
    QList<RC_MAP> tmpList;
    QDomElement tmpStateElement = GetComState(pComElement, QString("Map"));
    if(!tmpStateElement.isNull())
    {
        pMapType = tmpStateElement.attribute(D_EFFECTTYPE);
        QDomNodeList nodeList = tmpStateElement.childNodes();  //>@ĳ�������¼��е�����
        for(int j=0;j<nodeList.count();j++)
        {
            QDomElement element = nodeList.at(j).toElement();
            if(element.isNull())
                continue;
            QString tmpMapName;
            QString tmpName = element.tagName();
            if(tmpName.count() < 2)
                continue;
            if(!pPrefix.isEmpty())
            {
                if(!tmpName.startsWith(pPrefix))
                    continue;
                tmpMapName = tmpName.mid(pPrefix.count());
            }
            else
            {
                tmpMapName = tmpName;
            }
            RC_MAP tmpMap;
            tmpMap.mSrc = tmpMapName;
            tmpMap.mMap = element.text();
            tmpList.append(tmpMap);
        }
    }
    return tmpList;
}

QHash<QString, QString> uiManager::GetMapHash(QDomElement pComElement, QString &pMapType, QString pPrefix)
{
    QHash<QString, QString> tmpList;
    QDomElement tmpStateElement = GetComState(pComElement, QString("Map"));
    if(!tmpStateElement.isNull())
    {
        pMapType = tmpStateElement.attribute(D_EFFECTTYPE);
        QDomNodeList nodeList = tmpStateElement.childNodes();  //>@ĳ�������¼��е�����
        for(int j=0;j<nodeList.count();j++)
        {
            QDomElement element = nodeList.at(j).toElement();
            if(element.isNull())
                continue;
            QString tmpMapName;
            QString tmpName = element.tagName();
            if(tmpName.count() < 2)
                continue;
            if(!pPrefix.isEmpty())
            {
                if(!tmpName.startsWith(pPrefix))
                    continue;
                tmpMapName = tmpName.mid(pPrefix.count());
            }
            else
            {
                tmpMapName = tmpName;
            }
            tmpList.insert(tmpMapName, element.text());
        }
    }
    return tmpList;
}

FILEPATH_TYPE uiManager::GetRcFileType(QString pFileName)
{
    FILEPATH_TYPE tmpType = getRcFileType(pFileName);
    if(PATH_TYPENONE != tmpType)
        return tmpType;
    if(pFileName.isEmpty())
        return PATH_TYPENONE;
    //>@4���鿴�Ƿ�Ϊ��ǰUI�µ��ļ�
    QString tmpString = m_FileDir + pFileName;
    if(isFileExist(tmpString))
    {
        return PATH_TYPERC;
    }
    return PATH_TYPENONE;
}

FILEPATH_TYPE uiManager::RegulatRcFile(QString& pFileName)
{
    FILEPATH_TYPE tmpType = GetRcFileType(pFileName);
    if(tmpType == PATH_TYPERC)
    {
        pFileName.insert(0, m_FileDir);
    }
    return tmpType;
}

bool uiManager::IsRelativePath(QString pFileName)
{
    if(pFileName.isEmpty())
        return false;
    QString tmpString = m_FileDir + pFileName;
    if(isFileExist(tmpString))
    {
        return true;
    }
    return false;
}

bool uiManager::GetParentElement(QDomElement &pParentElement, QString pComPath)
{
    //>@�������ڵ�·��
    COM_TYPE tmpType = getCurComType(pComPath);
    if(tmpType == COM_DESKTOP)
    {
        if(!GetItemElement(QString("/Ui")))
        {
            if(!CreateItemElement(QString("/Ui")))
                return false;
        }
    }
    else if(tmpType > COM_AREA && tmpType < COM_COMPONENT)
    {
        //>@���û�����棬����Ϊû�и�
        QDomElement tmpElement = m_ComElementGroup.value(COM_DESKTOP);
        if(tmpElement.isNull())
            return false;
        if(!GetItemElement(QString("/Ui/SubUI")))
        {
            if(!CreateItemElement(QString("/Ui/SubUI")))
                return false;
        }
    }
    else if(tmpType > COM_COMPONENT)
    {
        COM_TYPE tmpParType = getParentComType(tmpType);
        if(tmpParType < COM_AREA)
            return false;
        //>@���û�и��ؼ����򷵻ؼ�
        QDomElement parentElement = m_ComElementGroup.value(tmpParType);
        if(parentElement.isNull())
            return false;
        QString tmpComString = QString("/Ui/SubUI/%1/Control").arg(parentElement.tagName());
        if(GetItemElement(tmpComString) == false)
        {
            if(!CreateItemElement(tmpComString))
                return false;
        }
    }
    else
    {
        return false;
    }
    pParentElement = itemElement;
    return true;
}

QDomElement uiManager::AddComDom(COM_TYPE pComType, QRect pRect, DIRECTION pDirection)
{
    if(pComType < COM_DESKTOP)
        return QDomElement();
    //>@����Ѿ��д˿ؼ��������棬�����ظ�����
    QDomElement tmpComElement = m_ComElementGroup.value(pComType);
    if(!tmpComElement.isNull())
        return tmpComElement;
    //>@
    if(pRect.isNull())
        return QDomElement();
    //>@���û�и��ؼ������˳������Ƕ���Desktop��Ҫ��������
    if(pComType > COM_DESKTOP)
    {
        QDomElement tmpParElement = m_ComElementGroup.value(getParentComType(pComType));
        if(tmpParElement.isNull())
            return QDomElement();
    }
    //>@
    QDomElement tmpParent;
    if(!GetParentElement(tmpParent, getComponetPath(pComType)))
        return QDomElement();
    QString tmpComName = getComponetName(pComType);
    tmpComElement = createElement(tmpComName);
    if(tmpComElement.isNull())
        return QDomElement();
    //>@����Rect
    if(!ModifyComRect(0, tmpComElement, pRect, pDirection, true))
        return QDomElement();
    tmpParent.appendChild(tmpComElement);
    m_IsChanged = true;
    m_ComElementGroup.insert(pComType, tmpComElement);
    return tmpComElement;
}

bool uiManager::DeleteComDom(COM_TYPE pComType)
{
    QDomElement tmpElement = m_ComElementGroup.take(pComType);
    if(tmpElement.isNull())
        return true;
    return DeleteItemElement(tmpElement);
}

bool uiManager::SaveRelativeComDom(QDomElement pComElement)
{
    if(!isValid())
        return false;
    QString tmpRelativePath = m_FileDir;
    COM_TYPE tmpType = getComponetType(pComElement.tagName());
    QString tmpComPath = getComponetPath(tmpType);
    QDomNodeList tmpStateList = GetComStateList(pComElement);
    for(int i=0;i<tmpStateList.count();i++)
    {
        QDomElement tmpStateElement = tmpStateList.at(i).toElement();
        if(tmpStateElement.isNull())
            continue;
        QString tmpStateName = tmpStateElement.tagName();
        QString tmpEffectName = tmpStateElement.attribute(D_EFFECTTYPE);
        QDomNodeList tmpRcList = tmpStateElement.childNodes();
        for(int j=0;j<tmpRcList.count();j++)
        {
            QDomElement tmpRcElement = tmpRcList.at(j).toElement();
            if(tmpRcElement.isNull())
                continue;
            QString tmpRcName = tmpRcElement.tagName();
            QString tmpRcFile = tmpRcElement.text();
            if(tmpRcName.compare(QString(D_RCINVALID)) && !IsRelativePath(tmpRcFile)) //����ڲ���Ҫ��Դ����Ч�Լ��Ѿ��任Ϊ���·������Դ
            {
                QString tmpString;
                bool needCopy = true;
                if(tmpType == COM_DESKTOP)
                {
                    //>@�����Desktop
                    tmpString = QString("Desktop") + QString("/") +
                                       tmpStateName + QString("/") +
                                       tmpEffectName + QString("/") +
                                       getFileName(tmpRcFile);
                }
                else if(tmpType == COM_QML || tmpType == COM_OPENGL) //>@
                {
                    FILEPATH_TYPE tmpType = GetRcFileType(tmpRcFile);
                    if(tmpType != PATH_TYPEABS)
                    {
                        IDE_TRACE_STR(tmpRcFile);
                        continue;
                    }
                    tmpString = tmpComPath.mid(1) + QString("/") +
                                       tmpStateName + QString("/") +
                                       tmpEffectName + QString("/") +
                                       getFileName(tmpRcFile);
                    QString tmpDst = tmpRelativePath + tmpString;
                    QString tmpSrc = tmpRcFile;  //>@
                    int index1 = tmpSrc.lastIndexOf('.');
                    if(index1 < 0)
                    {
                        IDE_TRACE_STR(tmpSrc);
                        continue;
                    }
                    QString tmpSrcDir = tmpSrc.mid(0, index1);
                    int index2 = tmpDst.lastIndexOf('.');
                    if(index2 < 0)
                    {
                        IDE_TRACE_STR(tmpDst);
                        continue;
                    }
                    QString tmpDstDir = tmpDst.mid(0, index2);
                    if(CopyFolder(tmpSrcDir, tmpDstDir, true) == false)
                    {
                        IDE_TRACE_STR(tmpSrcDir);
                        continue;
                    }
                }
                else
                {
                    FILEPATH_TYPE tmpType = GetRcFileType(tmpRcFile);
                    if(tmpType == PATH_TYPENONE)
                    {
                        continue;
                    }
                    else if(tmpType == PATH_TYPENETADDR)
                    {
                        needCopy = false;
                        tmpString = tmpRcFile;
                    }
                    else
                    {
                        tmpString = tmpComPath.mid(1) + QString("/") +
                                       tmpStateName + QString("/") +
                                       tmpEffectName + QString("/") +
                                       getFileName(tmpRcFile);
                    }
                }
                if(needCopy)
                {
                    QString tmpDst = tmpRelativePath + tmpString;
                    if(!CopyFile(tmpRcFile, tmpDst))
                    {
                        IDE_TRACE_STR(tmpRcFile);
                        continue;
                    }
                }
                ModifyItemElement(tmpRcElement, tmpString);
            }
        }
    }
    return true;
}

bool uiManager::RegularFile(COM_TYPE pComType, QString pRcName, QString &pRcFile, QString pDstPath, QSize pSize)
{
    if(pRcName == QString(D_RCINVALID)) //����ڲ���Ҫ��Դ����Ч
    {
        return true;
    }
    else if(pComType >= COM_DESKTOP)
    {
        if(pDstPath.endsWith('/') == false)
            pDstPath.append('/');
        QString tmpSrc = m_FileDir + pRcFile;
        //>@�����ļ����͹�����Դ
        RC_TYPE tmpRcType = getRcType(pRcFile);
        switch(tmpRcType)
        {
            case RC_BMP:
            case RC_JPG:
            case RC_PNG:
            {
                //>@�޸�Ŀ���ļ���ΪRcName����׺Ϊpng
                int index = pRcFile.lastIndexOf(".");
                if(index < 0)
                {
                    pRcFile = pRcName + QString(".png");
                }
                else
                {
                    QString tmpString = pRcFile.mid(0, index+1);
                    pRcFile = tmpString + QString("png");
                }
                QString tmpDst = pDstPath + pRcFile;
                if(ConvertImage(tmpSrc, tmpDst, pSize) == false)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
                break;
            }
            case RC_GIF:
            {
                int index = pRcFile.lastIndexOf(".");
                if(index < 0)
                {
                    pRcFile = pRcName + QString(".gif");
                }
                else
                {
                    QString tmpString = pRcFile.mid(0, index+1);
                    pRcFile = tmpString + QString("gif");
                }
                QString tmpDst = pDstPath + pRcFile;
                if(CopyFile(tmpSrc, tmpDst) == false)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
                break;
            }
            case RC_SVG:
            {
                //>@�̶�ͼ����Դ��svg�ļ�������
                int index = pRcFile.lastIndexOf(".");
                if(index < 0)
                {
                    pRcFile = pRcName + QString(".svg");
                }
                else
                {
                    QString tmpString = pRcFile.mid(0, index+1);
                    pRcFile = tmpString + QString("svg");
                }
                QString tmpDst = pDstPath + pRcFile;
                if(CopyFile(tmpSrc, tmpDst) == false)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
                break;
            }
            case RC_QML:
            {
                //>@ת��QML�ļ��Լ���ʹ�õ�ͼ����Դ���̶�ͼ����Դ��QML�ļ�������
                QString tmpDst = pDstPath + pRcFile;
                if(CopyFile(tmpSrc, tmpDst) == false)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
                int index1 = tmpSrc.lastIndexOf('.');
                if(index1 < 0)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
                QString tmpSrcDir = tmpSrc.mid(0, index1);
                int index2 = tmpDst.lastIndexOf('.');
                if(index2 < 0)
                {
                    IDE_TRACE_STR(tmpDst);
                    return false;
                }
                QString tmpDstDir = tmpDst.mid(0, index2);
                if(CopyFolder(tmpSrcDir, tmpDstDir, true) == false)
                {
                    IDE_TRACE_STR(tmpSrcDir);
                    return false;
                }
                break;
            }
            case RC_FONT:
            {
                int index = pRcFile.lastIndexOf("/");
                if(index < 0)
                {
                    pRcFile = pRcName + QString(".font");
                }
                else
                {
                    QString tmpString = pRcFile.mid(0, index+1);
                    pRcFile = tmpString + pRcName + QString(".font");
                }
                QString tmpDst = pDstPath + pRcFile;
                if(CopyFile(tmpSrc, tmpDst) == false)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
                break;
            }
            case RC_MP3:
            case RC_WAV:
            {
                //>@ֻת����Ժ;���·���µ��ļ���QRC�Լ������ļ���������ת��
                FILEPATH_TYPE tmpType = GetRcFileType(pRcFile);
                if(tmpType == PATH_TYPERC || tmpType == PATH_TYPEABS)
                {
                    //>@�޸�Ŀ���ļ���ΪRcName����׺Ϊwav
                    int index = pRcFile.lastIndexOf("/");
                    if(index < 0)
                    {
                        pRcFile = pRcName + QString(".wav");
                    }
                    else
                    {
                        QString tmpString = pRcFile.mid(0, index+1);
                        pRcFile = tmpString + pRcName + QString(".wav");
                    }
                    QString tmpDst = pDstPath + pRcFile;
                    if(ConvertAudio(tmpSrc, tmpDst) == false)
                    {
                        IDE_TRACE_STR(tmpSrc);
                        return false;
                    }
                }
                break;
            }
            case RC_MP4:
            case RC_AVI:
            case RC_RMVB:
            case RC_FLV:
            {
                //>@ֻת����Ժ;���·���µ��ļ���QRC�Լ������ļ���������ת��
                FILEPATH_TYPE tmpType = GetRcFileType(pRcFile);
                if(tmpType == PATH_TYPERC || tmpType == PATH_TYPEABS)
                {
                    //>@�޸�Ŀ���ļ���ΪRcName����׺Ϊmp4
                    int index = pRcFile.lastIndexOf("/");
                    if(index < 0)
                    {
                        pRcFile = pRcName + QString(".mp4");
                    }
                    else
                    {
                        QString tmpString = pRcFile.mid(0, index+1);
                        pRcFile = tmpString + pRcName + QString(".mp4");
                    }
                    QString tmpDst = pDstPath + pRcFile;
                    if(ConvertVideo(tmpSrc, tmpDst, pSize) == false)
                    {
                        IDE_TRACE_STR(tmpSrc);
                        return false;
                    }
                }
                break;
            }
            default:break;  //>@����QRC�Լ���ַ���ļ�������ת��
        }
        return true;
    }
    return false;
}

mdpManager::mdpManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = MdpSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != MdpSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(MdpSuffix);
}

bool mdpManager::NewMDP()
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
        tmpElement.appendChild(createTextNode("BST MDP File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Monitor");
        tmpElement.appendChild(createTextNode("5000"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Passwd");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Pattern");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Showlog");
        tmpElement.appendChild(createTextNode("false"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CfgPara");
        tmpElement.appendChild(createElement("Middleware"));
        tmpElement.appendChild(createElement("Physical"));
        tmpElement.appendChild(createElement("Logic"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool mdpManager::OpenMDP()
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
    if(tmpString.compare(MdpFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return GetCfgparaGroup();
}

bool mdpManager::CloseMDP()
{
    SaveMDP();
    return true;
}

bool mdpManager::DelMDP()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool mdpManager::SaveMDP()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool mdpManager::RenameMDP(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(MdpSuffix))
        tmpString.append(MdpSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool mdpManager::GetCfgparaGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/CfgPara");
    if(flag)
    {
        m_CfgparaElementGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();  //>@����BGIMG��Physical��Logic
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpString = tmpElement.tagName();
            if(tmpString.isEmpty())
                continue;
            QString tmpPath = QString("/%1").arg(tmpString);
            m_CfgparaElementGroup.insert(tmpPath, tmpElement);
            IDE_TRACE_STR(tmpPath);
            QString tmpSubCfgString = QString("/CfgPara/%1").arg(tmpString);
            flag = GetItemElement(tmpSubCfgString);
            if(!flag)
                continue;
            QDomNodeList SubCfgItemNodeList = itemElement.childNodes();   //>@����CPU����NET��
            for(int j=0;j<SubCfgItemNodeList.count();j++)              //>@��������ҳ������Ԫ��
            {
                tmpElement = SubCfgItemNodeList.at(j).toElement();
                if(tmpElement.isNull())
                    continue;
                QString tmpSubString = tmpElement.tagName();
                if(tmpSubString.isEmpty())
                    continue;
                QString tmpSubPath = QString("/%1/%2").arg(tmpString).arg(tmpSubString);
                m_CfgparaElementGroup.insert(tmpSubPath, tmpElement);
                IDE_TRACE_STR(tmpSubPath);
                if(tmpString.compare("Physical")==0)
                {
                    m_PHElementGroup.insert(GetPHType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("Logic")==0)
                {
                    m_LGElementGroup.insert(GetLGType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("Middleware")==0)
                {
                    m_MWElementGroup.insert(GetMWType(tmpSubString), tmpElement);
                }
            }
        }
    }
    return true;
}

bool mdpManager::ClrMDP()
{
    if(GetItemElement("/CfgPara"))
    {
        ClrDomCldNodes(itemElement);
        return true;
    }
    return false;
}

bool mdpManager::SetMonitor(quint32 pFreq)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Monitor", QString::number(pFreq));
}

quint32 mdpManager::GetMonitor()
{
    if(!m_Valid)
        return 0;
    bool flag = GetItemElement("/Monitor");
    if(flag == false)
        return 0;
    return itemElement.text().toUInt();
}

bool mdpManager::SetLocation(QString pLocation)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Location", pLocation);
}

QString mdpManager::GetLocation()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Location");
    if(flag == false)
        return QString();
    return itemElement.text();
}

bool mdpManager::SetPassword(QString pPasswd)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Passwd", pPasswd);
}

QString mdpManager::GetPassword()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Passwd");
    if(flag == false)
        return QString();
    return itemElement.text();
}

bool mdpManager::SetPattern(QString pPattern)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Pattern", pPattern);
}

QString mdpManager::GetPattern()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Pattern");
    if(flag == false)
        return QString();
    return itemElement.text();
}

QString mdpManager::GetVersion()
{
    if(!m_Valid)
        return QString();
    return GetItemElementValue("/CfgPara/Logic/ContentManager/Application");
}

bool mdpManager::ShowLog(bool pEnable)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Pattern", pEnable?QString("true"):QString("false"));
}

bool mdpManager::CanShowLog()
{
    if(!m_Valid)
        return false;
    bool flag = GetItemElement("/Showlog");
    if(flag)
        return (itemElement.text().compare("false", Qt::CaseInsensitive)==0)?(false):(true);
    return false;
}

void mdpManager::DelDeviceDom(QString pPath)
{
    DEV_TYPE tmpDevType = GetDevType(pPath);
    if(tmpDevType == DEV_MIDDLEWARE)
    {
        MW_Type tmpType = GetMWType(getFileName(pPath));
        DelDeviceDom(tmpType, pPath);
    }
    else if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = GetPHType(getFileName(pPath));
        DelDeviceDom(tmpType, pPath);
    }
    else if(tmpDevType == DEV_LOGIC)
    {
        LG_Type tmpType = GetLGType(getFileName(pPath));
        DelDeviceDom(tmpType, pPath);
    }
}

void mdpManager::DelDeviceDom(MW_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != M_INVALID)
        m_MWElementGroup.take(pType);
    DelDom(tmpElement);
    m_IsChanged = true;
}

void mdpManager::DelDeviceDom(PH_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != P_INVALID)
        m_PHElementGroup.take(pType);
    DelDom(tmpElement);
    m_IsChanged = true;
}

void mdpManager::DelDeviceDom(LG_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != L_INVALID)
        m_LGElementGroup.take(pType);
    DelDom(tmpElement);
    m_IsChanged = true;
}

QDomElement mdpManager::AddDeviceDom(QString pPath, QDomElement pInitElement)
{
    if(pPath.isEmpty() || pInitElement.isNull())
        return QDomElement();
    //>@��ȡ���ڵ�
    QString tmpParPath = getParentDirectory(pPath);
    QString tmpParName = GetDirectoryName(tmpParPath);
    tmpParPath = QString("/%1").arg(tmpParName);
    QDomElement tmpParElement = m_CfgparaElementGroup.value(tmpParPath);
    if(tmpParElement.isNull())
    {
        tmpParElement = createElement(tmpParName);
        if(!GetItemElement("/CfgPara"))
            return QDomElement();
        itemElement.appendChild(tmpParElement);
        m_CfgparaElementGroup.insert(tmpParPath, tmpParElement);
    }

    QDomElement deviceElement = createElement(pInitElement.tagName());
    //>@��������
    QDomNodeList CfgItemList = pInitElement.childNodes();
    for(int i=0;i<CfgItemList.count();i++)
    {
        QDomElement tmpSubInitElement = CfgItemList.at(i).toElement();
        if(tmpSubInitElement.isNull())
            continue;
        QDomElement tmpSubElement = CreateParaDom(tmpSubInitElement);
        if(tmpSubElement.isNull())
            continue;
        deviceElement.appendChild(tmpSubElement);
    }
    tmpParElement.appendChild(deviceElement);
    m_IsChanged = true;
    m_CfgparaElementGroup.insert(pPath, deviceElement);
    DEV_TYPE tmpDevType = GetDevType(pPath);
    if(tmpDevType == DEV_MIDDLEWARE)
    {
        MW_Type tmpType = GetMWType(getFileName(pPath));
        if(tmpType != M_INVALID)
            m_MWElementGroup.insert(tmpType, deviceElement);
    }
    else if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = GetPHType(getFileName(pPath));
        if(tmpType != P_INVALID)
            m_PHElementGroup.insert(tmpType, deviceElement);
    }
    else if(tmpDevType == DEV_LOGIC)
    {
        LG_Type tmpType = GetLGType(getFileName(pPath));
        if(tmpType != L_INVALID)
            m_LGElementGroup.insert(tmpType, deviceElement);
    }
    return deviceElement;
}

QDomElement mdpManager::CreateParaDom(QDomElement pInitElement)
{
    if(pInitElement.isNull())
        return QDomElement();
    QDomElement tmpElement;
    tmpElement = createElement(pInitElement.tagName());
    QString tmpParaString = pInitElement.attribute("Para");
    if(!tmpParaString.isEmpty())
        tmpElement.setAttribute("Para", tmpParaString);
    QString tmpListString = pInitElement.attribute("List");
    if(!tmpListString.isEmpty())
        tmpElement.setAttribute("List", tmpListString);
    tmpElement.appendChild(createTextNode(pInitElement.text()));
    return tmpElement;
}

bool mdpManager::ClrDomCldNodes(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    QDomNodeList cldNodeList = pElement.childNodes();
    for(int i=cldNodeList.count()-1;i>=0;i--)
    {
        pElement.removeChild(cldNodeList.at(i));
    }
    return true;
}

bool mdpManager::DelDom(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    if(!ClrDomCldNodes(pElement))
        return false;
    QString key = m_CfgparaElementGroup.key(pElement);
    QDomNode parNode = pElement.parentNode();
    parNode.removeChild(pElement);
    m_CfgparaElementGroup.remove(key);
    return true;
}

devManager::devManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = DevSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != DevSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(DevSuffix);
}

bool devManager::NewDEV()
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

bool devManager::OpenDEV()
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
    if(tmpString.compare(DevFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return GetDevGroup();
}

bool devManager::CloseDEV()
{
    SaveDEV();
    return true;
}

bool devManager::DelDEV()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool devManager::SaveDEV()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool devManager::RenameDEV(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(DevSuffix))
        tmpString.append(DevSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool devManager::SetCode(QString pCode)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Code", pCode);
}

QString devManager::GetCode()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Code");
    if(flag == false)
        return QString();
    return itemElement.text();
}

rcManager::rcManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = RcSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != RcSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(RcSuffix);
}

bool rcManager::NewRC()
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
        tmpElement.appendChild(createTextNode("BST RC File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    Save();
    return true;
}

bool rcManager::OpenRC()
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
    if(tmpString.compare(RcFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool rcManager::CloseRC()
{
    SaveRC();
    return true;
}

bool rcManager::DelRC()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool rcManager::SaveRC()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool rcManager::RenameRC(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(RcSuffix))
        tmpString.append(RcSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool rcManager::GetRcGroup()
{
    return true;
}

stcManager::stcManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = StcSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != StcSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(StcSuffix);
}

bool stcManager::NewSTC()
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
        tmpElement.appendChild(createTextNode("BST STC File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("TestCase");
        tmpElement.appendChild(createElement("Module"));
        QDomElement tmpSubElement = createElement("Protocol");
            tmpSubElement.appendChild(createElement("Generator"));
            tmpSubElement.appendChild(createElement("Analyzer"));
        tmpElement.appendChild(tmpSubElement);
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool stcManager::OpenSTC()
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
    if(tmpString.compare(StcFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return GetStcGroup();
}

bool stcManager::CloseSTC()
{
    SaveSTC();
    return true;
}

bool stcManager::DelSTC()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool stcManager::SaveSTC()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool stcManager::RenameSTC(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(StcSuffix))
        tmpString.append(StcSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool stcManager::GetStcGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/TestCase/Module");
    if(flag)
    {
        m_ModuleCaseGroup.clear();
        QDomElement tmpElement;
        QDomNodeList tmpModuleList = itemElement.childNodes();
        for(int i=0;i<tmpModuleList.count();i++)
        {
            tmpElement = tmpModuleList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpString = tmpElement.tagName();
            m_ModuleCaseGroup.insert(tmpString, tmpElement);//>@����Ԫ��
            //IDE_TRACE_STR(tmpString);
        }
    }
    flag = GetItemElement("/TestCase/Protocol/Generator");
    if(flag)
    {
        if(itemElement.hasChildNodes())
        {
            QDomElement tmpElement = itemElement.firstChildElement();
            if(!tmpElement.isNull())
                m_GeneratorElement = tmpElement;
        }
    }
    flag = GetItemElement("/TestCase/Protocol/Analyzer");
    if(flag)
    {
        m_AnalyzerGroup.clear();
        QDomElement tmpElement;
        QDomNodeList tmpModuleList = itemElement.childNodes();
        for(int i=0;i<tmpModuleList.count();i++)
        {
            tmpElement = tmpModuleList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            m_AnalyzerGroup.append(tmpElement);//>@����Ԫ��
        }
    }
    return true;
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

mapManager::mapManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = MapSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != MapSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(MapSuffix);
}

bool mapManager::NewMAP()
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
        tmpElement.appendChild(createTextNode("BST MAP File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("UiPath");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Map");
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool mapManager::OpenMAP()
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
    if(tmpString.compare(MapFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return GetCfgparaGroup();
}

bool mapManager::CloseMAP()
{
    SaveMAP();
    return true;
}

bool mapManager::DelMAP()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool mapManager::SaveMAP()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool mapManager::RenameMAP(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(MapSuffix))
        tmpString.append(MapSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool mapManager::SetUIPath(QString UiPath)
{
    ModifyItemElement("/UiPath", UiPath);
    return false;
}

QString mapManager::GetUIPath()
{
    bool flag = GetItemElement("/UiPath");
    if(flag == false)
    {
        return QString();
    }
    return itemElement.text();
}

bool mapManager::GetCfgparaGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Map");
    if(flag)
    {
        m_CfgparaElementGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            if(tmpString.isEmpty())
                continue;
            QString tmpCfgNameString = QString("/%1").arg(tmpString);
            m_CfgparaElementGroup.insert(tmpCfgNameString, tmpElement);//>@��������ҳ��Ԫ��
            QString tmpSubCfgString = QString("/Map/%1").arg(tmpString);  //>@map/com
            flag = GetItemElement(tmpSubCfgString);
            if(flag)
            {
                QDomNodeList SubCfgItemNodeList = itemElement.childNodes();
                for(int j=0;j<SubCfgItemNodeList.count();j++)              //>@��������ҳ������Ԫ��
                {
                    tmpElement = SubCfgItemNodeList.at(j).toElement();
                    QString tmpSubString = tmpElement.tagName();
                    if(tmpSubString.isEmpty())
                        continue;
                    QString tmpSubCfgNameString = QString("/%1/%2").arg(tmpString).arg(tmpSubString);
                    m_CfgparaElementGroup.insert(tmpSubCfgNameString, tmpElement);   //>@map/com/floor
                }
            }
        }
    }
    return true;
}

bool mapManager::ClrMap()
{
    if(GetItemElement("/Map"))
        ClrDomCldNodes(itemElement);
    return false;
}

bool mapManager::ClrDomCldNodes(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    QDomNodeList cldNodeList = pElement.childNodes();
    for(int i=cldNodeList.count()-1;i>=0;i--)
    {
        pElement.removeChild(cldNodeList.at(i));
    }
    return true;
}

bool mapManager::DelDom(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    if(!ClrDomCldNodes(pElement))
        return false;
    QString key = m_CfgparaElementGroup.key(pElement);
    QDomNode parNode = pElement.parentNode();
    parNode.removeChild(pElement);
    m_CfgparaElementGroup.remove(key);
    return true;
}

bool mapManager::AddDomNewNode(DomInfo pDomInfo)
{
    if(!CreateItemElement(pDomInfo.mParLabel))
       return false;

    QString newLabel = pDomInfo.mParLabel;
    if(!newLabel.endsWith("/"))
        newLabel.append("/");
    newLabel.append(pDomInfo.mTagName);

    if(GetItemElement(newLabel))
        DelDom(itemElement);

    if(pDomInfo.mTagName.isEmpty())
        return false;
    QDomElement element = createElement(pDomInfo.mTagName);
    if(!pDomInfo.mAtPara.isEmpty())
        element.setAttribute("Para", pDomInfo.mAtPara);
    if(!pDomInfo.mAtList.isEmpty())
        element.setAttribute("List", pDomInfo.mAtList);
    element.appendChild(createTextNode(pDomInfo.mText));
    itemElement.appendChild(element);
    return true;
}

bool mapManager::MapItemToElement(MAPITEM_INFO *pMapItemInfo, QDomElement &pParentElement)
{
    if(pMapItemInfo == 0 || pParentElement.isNull())
        return false;
    QString tmpMapContent;
    QString tmpEventName = pParentElement.tagName();
    if(pMapItemInfo->mMediaInfo.mFuncMap.isEmpty() == false)
        tmpMapContent.append(QString("%1:%2").arg(tmpEventName).arg(pMapItemInfo->mMediaInfo.mFuncMap));
    if(pMapItemInfo->mMediaInfo.mImage.isEmpty() == false)
        tmpMapContent.append(QString(";Image:%1").arg(pMapItemInfo->mMediaInfo.mImage));
    if(pMapItemInfo->mMediaInfo.mAudio.isEmpty() == false)
        tmpMapContent.append(QString(";Audio:%1").arg(pMapItemInfo->mMediaInfo.mAudio));
    if(pMapItemInfo->mMediaInfo.mVideo.isEmpty() == false)
        tmpMapContent.append(QString(";Video:%1").arg(pMapItemInfo->mMediaInfo.mVideo));
    if(pMapItemInfo->mMediaInfo.mLogo.isEmpty() == false)
        tmpMapContent.append(QString(";Logo:%1").arg(pMapItemInfo->mMediaInfo.mLogo));
    if(pMapItemInfo->mMediaInfo.mText.isEmpty() == false)
        tmpMapContent.append(QString(";Text:%1").arg(pMapItemInfo->mMediaInfo.mText));
    QString tmpString = QString("Key_%1").arg(pMapItemInfo->mEventValue);
    QDomElement tmpElement = createElement(tmpString);
    tmpElement.appendChild(createTextNode(tmpMapContent));
    pParentElement.appendChild(tmpElement);
    return true;
}

MAPITEM_INFO *mapManager::ElementToMapItem(QDomElement pElement)
{
    if(pElement.isNull())
        return (MAPITEM_INFO *)0;
    QString tmpEventValue = pElement.tagName(); //Floor/key_1
    if(tmpEventValue.isEmpty())
        return (MAPITEM_INFO *)0;
    QStringList tmpKeyList = tmpEventValue.split("_");
    if(tmpKeyList.count() == 2 && tmpKeyList.at(0).compare("Key", Qt::CaseInsensitive) == 0)
    {
        MAPITEM_INFO *tmpMapItemInfo = new MAPITEM_INFO;
        tmpMapItemInfo->mEventValue = tmpKeyList.at(1);
        QString tmpMapContent = pElement.text();
        QStringList tmpActionList = tmpMapContent.split(";");
        for(int k=0;k<tmpActionList.count();k++)   //content
        {
            QStringList tmpChildActionList = tmpActionList.at(k).split(":");
            if(tmpChildActionList.count() != 2)
                continue;
            QString tmpChildKey = tmpChildActionList.at(0);
            if(tmpChildKey.isEmpty())
                continue;
            QString tmpChildContent = tmpChildActionList.at(1);
            if(tmpChildContent.isEmpty())
                continue;
            tmpMapItemInfo->mMediaInfo.mFuncMap = tmpChildContent;
        }
        return tmpMapItemInfo;
    }
    return (MAPITEM_INFO *)0;
}

QDomElement mapManager::GetMapElement(MAP_EVENT pTypeFilter)
{
    if(m_Valid)
    {
        QStringList labelList = m_CfgparaElementGroup.keys();
        for(int i=0;i<labelList.count();i++)
        {
            QString eventLabel = labelList.at(i);
            MAP_EVENT eventType = GetMapEventType(getFileName(eventLabel));
            if(eventType != pTypeFilter)
                continue;
            QDomElement parElement = m_CfgparaElementGroup.value(eventLabel);
            if(parElement.isNull())
                continue;
            return parElement;
        }
    }
    return QDomElement();
}

bool mapManager::SetMapElement(MAP_EVENT pType, QString pEvent, QString pActions)
{
    QDomElement tmpElement = GetMapElement(pType);
    QString tmpPath;
    QString tmpRelPath;
    //>@�������Ȼ�ȡ·��
    tmpRelPath = QString("/Com/%1").arg(GetMapEventTypeString(pType));
    tmpPath = QString("/Map/Com/%1").arg(GetMapEventTypeString(pType));
    if(tmpElement.isNull())
    {
        if(!CreateItemElement(tmpPath))
            return false;
        tmpElement = itemElement;
        m_CfgparaElementGroup.insert(tmpRelPath, tmpElement);
    }
    return ModifyItemElement(QString("%1/%2").arg(tmpPath).arg(pEvent), pActions);
}

bool mapManager::GetMapItemGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Map");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }
    m_MapItemInfoGroup.clear();
    QDomNodeList tmpList = itemElement.childNodes();
    for(int i=0;i<tmpList.count();i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpEventName = tmpElement.tagName();
        if(tmpEventName.isEmpty())
            continue;
        QDomNodeList ChildrenList = tmpElement.childNodes();
        for(int j=0;j<ChildrenList.count();j++)
        {
            QDomElement tmpChildElement = ChildrenList.at(j).toElement();
            MAPITEM_INFO *tmpMapItemInfo = ElementToMapItem(tmpChildElement);
            if(tmpMapItemInfo == 0)
                continue;
            m_MapItemInfoGroup.insertMulti(tmpEventName, tmpMapItemInfo);
            IDE_TRACE_STR(tmpEventName);
        }
    }
    IDE_TRACE_INT(m_MapItemInfoGroup.count("Floor"));
    return true;
}

bool mapManager::SaveMapItemGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Map");
    if(flag == false)
    {
        IDE_TRACE();
        return false;
    }
    //>@���Map�µ�Ԫ��
    QDomNodeList tmpNodeList = itemElement.childNodes();
    for(int j=tmpNodeList.count()-1;j>=0;j--)
    {
        QDomNode tmp = itemElement.removeChild(tmpNodeList.at(j));
        tmp.clear();
    }
    //>@��m_MapItemInfoGroup����Ԫ��
    QList<QString> tmpKeyList = m_MapItemInfoGroup.uniqueKeys();
    for(int i=0;i<tmpKeyList.count();i++)
    {
        QString tmpEventName = tmpKeyList.at(i);
        QList<MAPITEM_INFO *> tmpMapItemInfoList = m_MapItemInfoGroup.values(tmpEventName);
        if(tmpMapItemInfoList.count() > 0)
        {
            //>@û���򴴽�
            QDomElement tmpParentElement = itemElement.firstChildElement(tmpEventName);
            if(tmpParentElement.isNull())
            {
                tmpParentElement = createElement(tmpEventName);
                itemElement.appendChild(tmpParentElement);
            }
            for(int j=0;j<tmpMapItemInfoList.count();j++)
            {
                MAPITEM_INFO * tmpMapItemInfo = tmpMapItemInfoList.at(j);
                if(tmpMapItemInfo)
                {
                    MapItemToElement(tmpMapItemInfo, tmpParentElement);
                }
            }
        }
    }
    m_IsChanged = true;
    return true;
}

bool mapManager::AddMapItem(QString pEventName, QString pEventValue, MEDIA_INFO pMediaInfo)
{
    QList<MAPITEM_INFO *> tmpMapItemInfoList = m_MapItemInfoGroup.values(pEventName);
    for(int i=0;i<tmpMapItemInfoList.count();i++)
    {
        MAPITEM_INFO * tmpMapItemInfo = tmpMapItemInfoList.at(i);
        if(tmpMapItemInfo && tmpMapItemInfo->mEventValue.compare(pEventValue, Qt::CaseInsensitive) == 0)
        {
            return false;
        }
    }
    MAPITEM_INFO * tmpMapItemInfo = new MAPITEM_INFO;
    tmpMapItemInfo->mEventValue = pEventValue;
    tmpMapItemInfo->mMediaInfo = pMediaInfo;
    m_MapItemInfoGroup.insertMulti(pEventName, tmpMapItemInfo);
    return true;
}

bool mapManager::DelMapItem(QString pEventName, QString pEventValue)
{
    QList<MAPITEM_INFO *> tmpMapItemInfoList = m_MapItemInfoGroup.values(pEventName);
    for(int i=0;i<tmpMapItemInfoList.count();i++)
    {
        MAPITEM_INFO * tmpMapItemInfo = tmpMapItemInfoList.at(i);
        if(tmpMapItemInfo && tmpMapItemInfo->mEventValue.compare(pEventValue, Qt::CaseInsensitive) == 0)
        {
            m_MapItemInfoGroup.remove(pEventName, tmpMapItemInfo);
            delete tmpMapItemInfo;
        }
    }
    return true;
}

MAPITEM_INFO *mapManager::GetMapItem(QString pEventName, QString pEventValue)
{
    QList<MAPITEM_INFO *> tmpMapItemInfoList = m_MapItemInfoGroup.values(pEventName);
    for(int i=0;i<tmpMapItemInfoList.count();i++)
    {
        MAPITEM_INFO * tmpMapItemInfo = tmpMapItemInfoList.at(i);
        if(tmpMapItemInfo && tmpMapItemInfo->mEventValue.compare(pEventValue, Qt::CaseInsensitive) == 0)
        {
            return tmpMapItemInfo;
        }
    }
    return (MAPITEM_INFO *)0;
}

bool mapManager::ModifyMapItem(QString pEventName, QString pEventValue, MEDIA_INFO mMediaInfo)
{
    QList<MAPITEM_INFO *> tmpMapItemInfoList = m_MapItemInfoGroup.values(pEventName);
    for(int i=0;i<tmpMapItemInfoList.count();i++)
    {
        MAPITEM_INFO * tmpMapItemInfo = tmpMapItemInfoList.at(i);
        if(tmpMapItemInfo && tmpMapItemInfo->mEventValue.compare(pEventValue, Qt::CaseInsensitive) == 0)
        {
            tmpMapItemInfo->mMediaInfo = mMediaInfo;
            return true;
        }
    }
    return false;
}

sevManager::sevManager(QString fileName) : xmlParse(fileName)
{
    int tmpNum, tmpSuffixNum;
    tmpNum = m_FileName.length();
    tmpSuffixNum = SevSuffix.length();
    if(tmpNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != SevSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(SevSuffix);
}

bool sevManager::NewSev()
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
        tmpElement.appendChild(createTextNode(SevFileType));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool sevManager::OpenSev()
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
    if(tmpString.compare(SevFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    GetElevatorGroup();
    return true;
}

bool sevManager::CloseSev()
{
    SaveSev();
    return true;
}

bool sevManager::DelSev()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool sevManager::SaveSev()
{
    return Save();
}

bool sevManager::RenameSev(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(SevSuffix))
        tmpString.append(SevSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool sevManager::GetElevatorGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Server/EvelatorList");
    if(flag)
    {
        m_ElevatorElementGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            QString tmpNameString = QString("/%1").arg(tmpString);
            m_ElevatorElementGroup.insert(tmpNameString, tmpElement);//>@��������ҳ��Ԫ��
            IDE_TRACE_STR(tmpNameString);
        }
    }
    return true;
}

devInfoManager::devInfoManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = DevInfoSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != DevInfoSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(DevInfoSuffix);
}

bool devInfoManager::NewDevInfo()
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
        tmpElement.appendChild(createTextNode("BST DEVINFO File"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool devInfoManager::OpenDevInfo()
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
    if(tmpString.compare(DevInfoFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool devInfoManager::CloseDevInfo()
{
    SaveDevInfo();
    return true;
}

bool devInfoManager::DelDevInfo()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool devInfoManager::SaveDevInfo()
{
    Save();
    return true;
}

bool devInfoManager::RenameDevInfo(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(DevInfoSuffix))
        tmpString.append(DevInfoSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

fontManager::fontManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = FontSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != FontSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(FontSuffix);
}

bool fontManager::NewFont()
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
        tmpElement.appendChild(createTextNode("BST Font File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Content");
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool fontManager::OpenFont()
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
    if(tmpString.compare(FontFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return GetParagraphGroup();
}

bool fontManager::CloseFont()
{
    SaveFont();
    return true;
}

bool fontManager::DelFont()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool fontManager::SaveFont()
{
    Save();
    return true;
}

bool fontManager::RenameFont(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(FontSuffix))
        tmpString.append(FontSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool fontManager::GetParagraphGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/Content");
    if(flag)
    {
        m_ParagraphGroup.clear();
        QDomElement tmpElement;
        QDomNodeList PrargraphList = itemElement.childNodes();
        for(int i=0;i<PrargraphList.count();i++)
        {
            tmpElement = PrargraphList.at(i).toElement();
            QString tmpDesc = tmpElement.tagName();
            m_ParagraphGroup.insert(tmpDesc, tmpElement);//>@��������ҳ��Ԫ��
            //IDE_TRACE_STR(tmpDesc);
        }
    }
    return true;
}

bool fontManager::AddParagraph(QString pDesc, QString pString, FONT_INFO pInfo)
{
    if(GetItemElement("/Content") == false)
    {
        IDE_TRACE();
        return false;
    }
    QDomElement tmpElement = m_ParagraphGroup.value(pDesc);
    if(tmpElement.isNull() == false)
    {
        IDE_TRACE();//>@����Ѿ������򷵻�false
        return false;
    }
    tmpElement = createElement(pDesc);
    tmpElement.setAttribute("Para", pInfo.toString());
    tmpElement.appendChild(createTextNode(pString));
    itemElement.appendChild(tmpElement);
    m_ParagraphGroup.insert(pDesc, tmpElement);
    return true;
}

bool fontManager::ModifyParagraph(QString pDesc, QString pString, FONT_INFO pInfo)
{
    QDomElement tmpElement = m_ParagraphGroup.value(pDesc);
    if(tmpElement.isNull())
    {
        IDE_TRACE();//>@����������򷵻�false
        return false;
    }
    tmpElement.setAttribute("Para", pInfo.toString());
    return ModifyItemElement(tmpElement, pString);
}

bool fontManager::DelParagraph(QString pDesc)
{
    QDomElement tmpElement = m_ParagraphGroup.value(pDesc);
    if(tmpElement.isNull())
    {
        IDE_TRACE();//>@����������򷵻�true
        return true;
    }
    if(GetItemElement("/Content") == false)
    {
        IDE_TRACE();
        return false;
    }
    itemElement.removeChild(tmpElement);
    m_ParagraphGroup.remove(pDesc);
    return true;
}

bool fontManager::GetParagraph(QString pDesc, QString &pString, FONT_INFO &pInfo)
{
    QDomElement tmpElement = m_ParagraphGroup.value(pDesc);
    if(tmpElement.isNull())
    {
        IDE_TRACE();//>@����������򷵻�false
        return false;
    }
    pInfo = tmpElement.attribute("Para");
    pString = tmpElement.text();
    return true;
}

updateManager::updateManager(QString fileName) : xmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = UpdateSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != UpdateSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(UpdateSuffix);
}

bool updateManager::OpenUpdate()
{
    QFile file(m_FileDir + m_FileName);
    bool flag = file.exists();
    if(flag == true)
    {
        flag = file.open(QFile::ReadOnly | QFile::Text);
        if (flag == false)
        {
            IDE_TRACE();
            m_Valid = false;
            return false;
        }
        flag = read(&file);
        if(flag == false)
        {
            file.close();
            m_Valid = false;
            IDE_TRACE();
            return false;
        }
    }
    else
    {
        IDE_TRACE();
        m_Valid = false;
        return false;
    }
    file.close();

    m_Valid = true;
    //>@Check FileType
    flag = GetItemElement("/FileType");
    if(flag == false)
    {
        m_Valid = false;
        return false;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare(UpdateFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool updateManager::CloseUpdate()
{
    SaveUpdate();
    return true;
}

bool updateManager::SaveUpdate()
{
    Save();
    return true;
}

QString updateManager::GetVersion(QString pName)
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return QString();
    }
    bool flag = GetItemElement("/Software");
    if(flag)
    {
        QDomElement tmpElement = itemElement.firstChildElement(pName);
        if(!tmpElement.isNull())
            return tmpElement.text();
    }
    return QString();
}

const QString burnManager::BurnSuffix = ".burn";
const QString burnManager::BurnFileType = "BST BURN File";

burnManager::burnManager(QString fileName) : xmlParse(fileName)
{
    int tmpNum, tmpSuffixNum;
    tmpNum = m_FileName.length();
    tmpSuffixNum = BurnSuffix.length();
    if(tmpNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != BurnSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(BurnSuffix);
}

bool burnManager::NewBurn()
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
        tmpElement.appendChild(createTextNode(BurnFileType));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("HardwareCheck");
    {
        QDomElement tmpSubElement = createElement("LostMaterial");
        tmpElement.appendChild(tmpSubElement);

        tmpSubElement = createElement("ShortCircuit");
        tmpElement.appendChild(tmpSubElement);

        tmpSubElement = createElement("PowerSuface");
        tmpElement.appendChild(tmpSubElement);
    }
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("UsbBurn");
    {
        QDomElement tmpSubElement = createElement("Program1");
        tmpElement.appendChild(tmpSubElement);

        tmpSubElement = createElement("Program1");
        tmpElement.appendChild(tmpSubElement);
    }
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool burnManager::OpenBurn()
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
    if(tmpString.compare(BurnFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return true;
}

bool burnManager::CloseBurn()
{
    SaveBurn();
    return true;
}

bool burnManager::DelBurn()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool burnManager::SaveBurn()
{
    return Save();
}

bool burnManager::RenameBurn(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(BurnSuffix))
        tmpString.append(BurnSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}


Global GlobalPara;
#include "uicantainer.h"

bool UiCantainerBase::LoadUI(QString pFileName)
{
    return false;
}

bool UiCantainerBase::Initialize(uiManager *pManager)
{
    if(!(pManager && pManager->m_Valid))
    {
        IDE_TRACE();
        return false;
    }
    m_UiManager = pManager;
    return InitUI();
}

bool UiCantainerBase::Initialize(QString pFileName)
{
    Release();
    m_UiManager = new uiManager(pFileName);
    if(isFileExist(m_UiManager->m_FileDir+m_UiManager->m_FileName) == false)
    {
        IDE_TRACE_STR(m_UiManager->m_FileName);
        m_UiManager->NewUI();
    }
    else
    {
        IDE_TRACE_STR(m_UiManager->m_FileName);
        m_UiManager->OpenUI();
    }
    return Initialize(m_UiManager);
}

void UiCantainerBase::Release()
{
    ReleaseUI();
    //>@������ⲿ�����uimanager������ɾ������Ҫ���ⲿɾ����
    if(!m_ExtraClass && m_UiManager) //>@�˴������κα����ļ�������
    {
        delete m_UiManager;
        m_UiManager = 0;
    }
}

void UiCantainerBase::ModifyLayout(int pLayout)
{
    if(m_UiLayout != pLayout)
    {
        m_UiLayout = pLayout;
        UpdateUI();
    }
}

DIRECTION UiCantainerBase::GetDirection(int pRotate)
{
    DIRECTION tmpUiDirection = Horizontal;
    if((pRotate == 0) || (pRotate == 180))
        tmpUiDirection = Horizontal;
    else
        tmpUiDirection = Vertical;
    return tmpUiDirection;
}

bool UiCantainerBase::InitUI()
{
    //>@��ʼ������༭��
    if(!m_UiManager)
        return false;
    return true;
}

int UiCantainerBase::OriginRotate()
{
#ifdef UNKNOW //LINUX
    QStringList environment = QProcess::systemEnvironment();
    QString str;
    QString tmpReg = QString("QWS_DISPLAY=Transformed:Rot");
    int size = tmpReg.size();
    foreach(str,environment)
    {
        int index1 = str.indexOf(tmpReg);
        if(index1 >= 0)
        {
            index1 += size;
            QString tmpRotate;
            int index2 = str.indexOf(':', index1);
            if(index2)
                tmpRotate = str.mid(index1, index2-index1);
            else
                tmpRotate = str.mid(index1);
            return tmpRotate.toInt();
        }
    }
#else
    return m_OriginRotate;
#endif
    return 0;
}


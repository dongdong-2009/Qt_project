#include "physicalprotocol.h"
#ifdef DISPLAY
    #ifdef SUPPORT_SCENE
        #include "graphicsmedia.h"
    #elif SUPPORT_WIDGET
        #include "widgetmedia.h"
    #endif
#endif
#include "logicthememanager.h"
#include "deviceinc.h"
#include "toolhttp.h"

ReadFileOp::ReadFileOp(QObject *parent) : QObject(parent)
{
    mEnable = false;
    unbind();
}

ReadFileOp::~ReadFileOp()
{
    unbind();
}

void ReadFileOp::unbind()
{
    if(mEnable)
    {
        mEnable = false;
        mCurPack.clear();
        mSize = mPos = mSurplus = 0;
    }
}

bool ReadFileOp::bind(QString pSrc)
{
    unbind();
    if(!QFile::exists(pSrc))
    {
        IDE_TRACE_STR(pSrc);
        return false;
    }
    mFile.setFileName(pSrc);
//    if(!mFile.open(QIODevice::ReadOnly))
//    {
//        IDE_TRACE_STR(pSrc);
//        return false;
//    }
//    mFile.close();
    mSurplus = mSize = mFile.size();
    mEnable = true;
    return true;
}

void ReadFileOp::setpos(qint64 pPos)
{
    if(pPos > mSize)
        mPos = mSize;
    else
        mPos = pPos;
    mSurplus = mSize - mPos;
}

bool ReadFileOp::read(qint64 maxlen)
{
    if(!mEnable)
        return false;
    mCurPack.clear();
    if(maxlen <= 0)
        return false;
    if(mPos >= mSize)
    {
        IDE_TRACE();
        return false;
    }
    if(!mFile.open(QIODevice::ReadOnly))
    {
        IDE_TRACE();
        return false;
    }
    bool flag = false;
    if(mFile.seek(mPos))
    {
        flag = true;
        mCurPack = mFile.read(maxlen);
    }
    mPos = mFile.pos();
    mSurplus = mSize - mPos;
    mFile.close();
    return flag;
}

int ReadFileOp::progress()
{
    int tmpProg = 0;
    if(mEnable)
    {
        if(mSize > 0 && mPos > 0)
        {
            int div = mSize/100;
            if(div > 0)
                tmpProg = mPos / div;
        }
    }
    return tmpProg;
}

WriteFileOp::WriteFileOp(QObject *parent) : QObject(parent)
{
    mEnable = false;
    unbind();
}

WriteFileOp::~WriteFileOp()
{
    unbind();
}

void WriteFileOp::unbind()
{
    if(mEnable)
    {
        mEnable = false;
        QString tmpSrc = mFile.fileName();
        if(!tmpSrc.isEmpty())
            DelFile(tmpSrc+D_TMPSUFFIX);
        mSize = mPos = mSurplus = 0;
    }
}

bool WriteFileOp::bind(QString pSrc, qint64 pSize)
{
    unbind();
    if(pSrc.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QDir dir;
    dir.mkpath(getFileDirectory(pSrc));
    if(QFile::exists(pSrc) && !CutFile(pSrc, pSrc+D_TMPSUFFIX))
    {
        IDE_TRACE_STR(pSrc);
        return false;
    }
    mFile.setFileName(pSrc);
    mFile.setPermissions(QFile::WriteOther);
    if(!mFile.open(QFile::WriteOnly))
    {
        IDE_TRACE_STR(pSrc);
        return false;
    }
    mFile.close();
    mSurplus = mSize = pSize;
    mEnable = true;
    return true;
}

bool WriteFileOp::write(QByteArray pByteArray, qint64 pPos) //>@pPos���Ϊ��������ʾֱ��׷��
{
    if(!mEnable)
        return false;
    if(pByteArray.isEmpty())
        return true;
    if(!mFile.open(QFile::Append))
    {
        IDE_TRACE();
        return false;
    }
    bool flag = true;
    if(pPos >= 0)
        flag = mFile.seek(pPos);
    if(flag)
    {
        if(mFile.write(pByteArray) <= 0)
        {
            flag = false;
        }
        else
        {
            mPos = mFile.pos();
            mSurplus = mSize - mPos;
        }
    }
    mFile.close();
    return flag;
}

bool WriteFileOp::repair()  //>@��������ļ��ָ�
{
    if(!mEnable)
        return false;
    QString tmpSrc = mFile.fileName();
    if(tmpSrc.isEmpty())
        return false;
    return CutFile(tmpSrc+D_TMPSUFFIX, tmpSrc);
}

int WriteFileOp::progress()
{
    int tmpProg = 0;
    if(mEnable)
    {
        if(mSize > 0 && mPos > 0)
        {
            int div = mSize/100;
            if(div > 0)
                tmpProg = mPos / div;
        }
    }
    return tmpProg;
}

PhysicalProtocol::PhysicalProtocol(QObject *parent) :
    PhysicalDevice(parent)
{
#ifdef SUPPORT_MUTEX
    m_ParseDataMutex = new QMutex(QMutex::Recursive);
#else
    m_ParseDataMutex = false;
#endif

    m_Connected = false;

    m_Elevator = false;
    m_MonitorCount = 0;
    m_MaxBlockSize = 4096;

    m_ReadFileOp = new ReadFileOp(this);
    m_WriteFileOp = new WriteFileOp(this);

    m_CurDevType = P_INVALID;

    m_CurRevFrame = 0;
    m_RetransFile = false;

    m_DebugMode = false;
    m_LoopFlag = false;
    m_FrameState = State_Wait;
    connect(&m_ConfMonitorTimer, SIGNAL(timeout()), this, SLOT(slot_ConfMonitorTimeout()));

    mHostFlag = false;
    mAutoConnLog = false;
    mAutoConnFlag = false;
    mAutoConnState = false;

    for(int i=0;i<3;i++)
    {
        m_HBFreq[i].setnum(0, 0, 0x7fffffff);
        m_HBWait[i].SetTimeout(0);
        m_HBCount[i] = 0;
    }

    //>@��ʾ��m_HBOverflow+1��û�м�⵽��������ʾ������ʱ
    m_HBOverflow[0] = 0;
    m_HBOverflow[1] = 3;
    m_HBOverflow[2] = 5;

    connect(this, SIGNAL(sHB(quint32)), this, SLOT(slot_HB(quint32)), Qt::QueuedConnection);
    connect(this, SIGNAL(sHBTimeout(quint32)), this, SLOT(slot_HBTimeout(quint32)), Qt::QueuedConnection);

    connect(this, SIGNAL(sConnect()), this, SLOT(slot_Connect()), Qt::QueuedConnection);
    connect(this, SIGNAL(sDisconnect()), this, SLOT(slot_Disconnect()), Qt::QueuedConnection);

    connect(this, SIGNAL(sZip(FRAME_STRUCT *)), this, SLOT(slot_Zip(FRAME_STRUCT *)), Qt::QueuedConnection);
    connect(this, SIGNAL(sUnzip(FRAME_STRUCT *)), this, SLOT(slot_Unzip(FRAME_STRUCT *)), Qt::QueuedConnection);

    connect(this, SIGNAL(sInit(QVariant)), this, SLOT(slot_Init(QVariant)), Qt::QueuedConnection);
}

PhysicalProtocol::~PhysicalProtocol()
{
    Disconnected();
    WaitForLeisure(0);
    m_DebugMode = false;

#ifdef SUPPORT_MUTEX
    if(m_ParseDataMutex)
        delete m_ParseDataMutex;
#else
    m_ParseDataMutex = false;
#endif
}

void PhysicalProtocol::slot_Initialize()
{
    PhysicalDevice::slot_Initialize();
#ifdef DISPLAY
    if(!m_LCD.isNull())
    {
        //>@��Standbyģʽ�˳�
        disconnect(m_LCD.data(), SIGNAL(sStandby(bool)), 0, 0);
        connect(m_LCD.data(), SIGNAL(sStandby(bool)), this, SLOT(slot_StandbyCallback(bool)));
    }

    mPlayer = VAR_CAST<PhysicalPlayer *>(m_DevManager->m_PHDevList.value(P_Player));


    if(!mThemeManager.isNull() && mThemeManager.data()->m_UiContainer)
    {
#ifdef SUPPORT_SCENE
        UiScene *tmpContainer = mThemeManager.data()->m_UiContainer;
        mMedia = VAR_CAST<GraphicsMedia*>(tmpContainer->m_ComDiagramGroup.value(COM_MEDIA));
#elif SUPPORT_WIDGET
        UiWidget *tmpContainer = mThemeManager.data()->m_UiContainer;
        mMedia = VAR_CAST<WidgetMedia*>(tmpContainer->m_ComDiagramGroup.value(COM_MEDIA));
#endif
    }

    if(!mThemeManager.isNull())
    {
        disconnect(this, SIGNAL(sArrowEvent(quint32)), 0, 0);
        disconnect(this, SIGNAL(sFloorEvent(quint32)), 0, 0);
        disconnect(this, SIGNAL(sRadioFunEvent(quint32)), 0, 0);
        disconnect(this, SIGNAL(sMediaEvent(quint32)), 0, 0);

        if(m_Elevator)
        {
            connect(this, SIGNAL(sArrowEvent(quint32)), mThemeManager.data()->m_EventMap, SLOT(slot_ArrowEvent(quint32)), Qt::QueuedConnection);
            connect(this, SIGNAL(sFloorEvent(quint32)), mThemeManager.data()->m_EventMap, SLOT(slot_FloorEvent(quint32)), Qt::QueuedConnection);
            connect(this, SIGNAL(sRadioFunEvent(quint32)), mThemeManager.data()->m_EventMap, SLOT(slot_FunctionEvent(quint32)), Qt::QueuedConnection);
            connect(this, SIGNAL(sMediaEvent(quint32)), mThemeManager.data()->m_EventMap, SLOT(slot_MediaEvent(quint32)), Qt::QueuedConnection);
            connect(this, SIGNAL(sButtonEvent(QVariant,BUTTONSTATE)), mThemeManager.data()->m_EventMap, SLOT(slot_ButtonEvent(QVariant,BUTTONSTATE)));

            connect(mThemeManager.data()->m_EventMap, SIGNAL(sBtnCallEvent(QVariant)), this, SLOT(slot_BtnCallEvent(QVariant)), Qt::QueuedConnection);
        }
    }
#endif
}

void PhysicalProtocol::slot_BtnCallEvent(QVariant pPara)
{
    ReplyButtonFrame(pPara, false);
}

void PhysicalProtocol::slot_StandbyCallback(bool pFlag)
{
    if(pFlag)
    {
        ReplyFloorFrame(0, 0, 1<<2);  //>@֪ͨ��������ʾ������Standbyģʽ
    }
    else
    {
        ReplyFloorFrame(0, 0, 0);
    }
}

quint32 PhysicalProtocol::InitPara()
{
#ifndef PART_TEST
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
        if(!tmpName.compare("DebugMode"))
        {
            QString tmpValue = tmpElement.text();
            m_DebugMode = (!tmpValue.compare("false"))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else if(!tmpName.compare("AreaReuse"))
        {
            QString tmpValue = tmpElement.text();
            m_ReuseType = GetReuseType(tmpValue);
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), tmpValue);
        }
        else if(!tmpName.compare("HBFreq"))  //>@��ָ��ʾ����APP���͵�����
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                m_HBFreq[0].setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
                if(m_HBFreq[0].mNum)
                {
                    m_HBWait[0].SetTimeout(m_HBFreq[0].mNum);
                    m_HBWait[0].update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
                tmpHBoxLayout = CreateNum(tmpName, m_HBFreq[0].mNum, m_HBFreq[0].mMinNum, m_HBFreq[0].mMaxNum);
            }
        }
        else if(!tmpName.compare("BtnBase"))  //>@��ť����
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                m_BtnBaseNum.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
                tmpHBoxLayout = CreateNum(tmpName, m_BtnBaseNum.mNum, m_BtnBaseNum.mMinNum, m_BtnBaseNum.mMaxNum);
            }
        }
        else if(!tmpName.compare("BtnRange"))  //>@��Ч��ť����
        {
            m_BtnRange = tmpElement.text();
            tmpHBoxLayout = CreateTextEdit(tmpName, m_BtnRange);
            UpdateBtnRange(m_BtnRange);
        }
        else if(!tmpName.compare("FlrStation"))  //>@��ǰ��վ
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                m_FlrStationNum.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
                tmpHBoxLayout = CreateNum(tmpName, m_FlrStationNum.mNum, m_FlrStationNum.mMinNum, m_FlrStationNum.mMaxNum);
            }
        }
        else if(!tmpName.compare("Elevator"))  //>@�Ƿ������Э��
        {
            QString tmpValue = tmpElement.text();
            m_Elevator = (!tmpValue.compare("false"))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else
        {
            tmpHBoxLayout = InitSubPara(tmpName, tmpElement);
        }
        if(!tmpHBoxLayout)
            continue;
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    validcount++;
    return validcount;
#endif
}

void PhysicalProtocol::UpdateBtnRange(QString pValue)
{
    m_ValidBtns.clear();
    QStringList tmpValidBtnStrs = pValue.split(';');
    if(tmpValidBtnStrs.isEmpty())
        return;
    QString tmpNum;
    foreach(tmpNum, tmpValidBtnStrs)
    {
        if(tmpNum.contains('~'))
        {
            QStringList tmpNumRange = tmpNum.split('~');
            if(tmpNumRange.count() == 2)
            {
                bool ok = false;
                int tmpRange1 = tmpNumRange.at(0).toInt(&ok);
                if(!ok)
                    continue;
                int tmpRange2 = tmpNumRange.at(1).toInt(&ok);
                if(!ok)
                    continue;
                if(tmpRange1 > tmpRange2)
                {
                    qSwap(tmpRange1, tmpRange2);
                }
                for(int i=tmpRange1;i<=tmpRange2;i++)
                {
                    m_ValidBtns.append(i);
                }
            }
        }
        else
        {
            bool ok = false;
            int tmpNumValue = tmpNum.toInt(&ok);
            if(!ok)
                continue;
            m_ValidBtns.append(tmpNumValue);
        }
    }
}

void PhysicalProtocol::UpdatePara(QObject* pObject)
{
#ifndef PART_TEST
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("DebugMode"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, m_DebugMode, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("AreaReuse"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue;
            if(!ModifyEnum(tmpComponent, tmpElement, tmpValue, tmpComponent->currentText()))
            {
            }
            else
            {
                m_ReuseType = GetReuseType(tmpValue);
            }
        }
    }
    else if(!tmpName.compare("HBFreq"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, m_HBFreq[0].mNum, tmpComponent->value(), m_HBFreq[0].mMinNum, m_HBFreq[0].mMaxNum))
            {
            }
            else
            {
                if(m_HBFreq[0].mNum)
                {
                    m_HBWait[0].SetTimeout(m_HBFreq[0].mNum);
                    m_HBWait[0].update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
            }
        }
    }
    else if(!tmpName.compare("BtnBase"))  //>@��ť����
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, m_BtnBaseNum.mNum, tmpComponent->value(), m_BtnBaseNum.mMinNum, m_BtnBaseNum.mMaxNum))
            {
            }
        }
    }
    else if(!tmpName.compare("BtnRange"))  //>@��Ч��ť����
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, m_BtnRange, tmpComponent->text()))
            {
            }
            else
            {
                UpdateBtnRange(m_BtnRange);
            }
        }
    }
    else if(!tmpName.compare("FlrStation"))  //>@
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, m_FlrStationNum.mNum, tmpComponent->value(), m_FlrStationNum.mMinNum, m_FlrStationNum.mMaxNum))
            {
            }
        }
    }
    else if(!tmpName.compare("Elevator"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, m_Elevator, tmpComponent->isChecked()))
            {
            }
        }
    }
    else
    {
        UpdateSubPara(tmpName, pObject, tmpElement);
    }
#endif
}

bool PhysicalProtocol::CanAbort()
{
    return !m_LoopFlag;
}

void PhysicalProtocol::WaitForLeisure(quint32 pTimeout)
{
    QtWait(m_LoopFlag, false, pTimeout);
}

void PhysicalProtocol::Connected()
{
    //>@��λ����
    m_HBFreq[1].setnum(0);
    m_HBFreq[2].setnum(0);

    m_Connected = true;
}

void PhysicalProtocol::Disconnected()
{
    if(m_Connected)
    {
        m_Connected = false;
        //>@�������
        ClearConfQueue();
        SetCurDev(P_INVALID, (QIODevice*)0);
    }
}

bool PhysicalProtocol::isConnected()
{
    return m_Connected;
}

quint32 PhysicalProtocol::Monitor(quint32 pCount)
{
#ifndef PART_TEST
    m_MonitorCount = pCount;
    if(m_Connected)
    {
//        if(m_HBFreq[0].mNum)   //>@˫����������ʾ��������������
//        {
//            if(m_HBWait[0].isValid(pCount, m_DevManager->m_DevMonitorFreq))
//            {
//                emit sHB(m_HBFreq[0].mNum);
//            }
//        }
        if(m_HBFreq[1].mNum)
        {
            if(m_HBWait[1].isValid(pCount, m_DevManager->m_DevMonitorFreq))
            {
                //>@���m_HBOverflow��û�н��յ�����������Ϊ��ʱ
                if(m_HBCount[1] >= m_HBOverflow[1])
                {
                    m_HBCount[1] = 0;
                    emit sHBTimeout(1);
                }
                else
                    m_HBCount[1]++;
            }
        }
        if(m_HBFreq[2].mNum)   //>@Host������������
        {
            if(m_HBWait[2].isValid(pCount, m_DevManager->m_DevMonitorFreq))
            {
                //>@���m_HBOverflow��û�н��յ�����������Ϊ��ʱ
                if(m_HBCount[2] >= m_HBOverflow[2])
                {
                    m_HBCount[2] = 0;
                    emit sHB(m_HBFreq[2].mNum);

                    emit sHBTimeout(2);
                }
                else
                {
                    m_HBCount[2]++;
                    emit sHB(m_HBFreq[2].mNum);
                }
            }
        }
    }
#endif
    return 1;
}

/**************************************
  �����룺
    0��ʧ��
    1���ɹ�
    2������ʧ��
    >=D_FRAMEMINLEN:���͵�������(��С֡��СΪD_FRAMEMINLEN)
*/
quint32 PhysicalProtocol::ProtocolWait(int &pFlag, int pEqlFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while((pFlag!=pEqlFlag))   //>@������ߣ��������˳�
        {
            if((mDevState != S_ON))
                return 2;
            QtSleep(10);
        }
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && (pFlag!=pEqlFlag))
        {
            if((mDevState != S_ON))
                return 2;
            QtSleep(10);
        }
    }
    return (quint32)(bool)(pFlag==pEqlFlag);
}

void PhysicalProtocol::SetCurDev(PH_Type pType, QIODevice* pDev)
{
    m_CurDevType = pType;
    m_CurTransDev = pDev;
}

/**************************************
  �����룺
    0���βδ���
    2��֡���ݶδ���
    3����֧�ֵ�֡����
    >=D_FRAMEMINLEN:���͵�������(��С֡��СΪD_FRAMEMINLEN)
  ��ע:
    ������ɺ���Զ���֡�����ͷš�
*/
quint32 PhysicalProtocol::SendData(FRAME_STRUCT *pFrame)
{
    if(!m_Connected)
    {
        IDE_DEBUG(QString("Disconnected!"));
        return 0;
    }
    if(!pFrame)
    {
        IDE_DEBUG(QString("Send Protocol -- dev %1 [frame null]").arg(mDevType));
        return 0;
    }
    if(pFrame->mDev.isNull())
    {
        pFrame->deleteLater();
        IDE_DEBUG(QString("Send Protocol -- dev %1 [dev null]").arg(mDevType));
        return 0;
    }
    //>@��֡
    QByteArray tmpSendData;
    if(pFrame->mFlag == Flag_Normal)
        tmpSendData.append(D_FRAMEHEADER);
    else
        tmpSendData.append(D_FRAMEHEADER_ACK);
    quint8 tmpFrameType = pFrame->mType;
    tmpSendData.append(tmpFrameType);
    switch(tmpFrameType)
    {
        case Type_FloorStatus:
        case Type_Urgency:
        case Type_ButtonStatus:
        case Type_DisabilityButton:
        {
            quint16 tmpLenth = pFrame->mData.count();
            if(tmpLenth != 7)
            {
                pFrame->deleteLater();
                IDE_DEBUG(QString("Send Protocol -- dev %1 wrong frame[%2 bytes]").arg(mDevType).arg((quint32)tmpLenth));
                return 2;  //֡���ݶδ���
            }
            tmpSendData.append(pFrame->mData);
            break;
        }
        case Type_Heartbeat:
        case Type_SetCmd:
        case Type_ScanDev:
        case Type_StreamTrans:
        {
            tmpSendData.append(pFrame->mCmd);
            quint16 tmpLenth = pFrame->mData.count();
            tmpSendData.append((quint8)(tmpLenth>>8));
            tmpSendData.append((quint8)(tmpLenth));
            tmpSendData.append(pFrame->mData);
            break;
        }
        case Type_RegCmd:
        case Type_FileTrans:
        {
            tmpSendData.append(pFrame->mCmd);
            tmpSendData.append((quint8)(pFrame->mAddr>>8));
            tmpSendData.append((quint8)(pFrame->mAddr));
            quint16 tmpLenth = pFrame->mData.count();
            tmpSendData.append((quint8)(tmpLenth>>8));
            tmpSendData.append((quint8)(tmpLenth));
            tmpSendData.append(pFrame->mData);
            break;
        }
        default:
        {
            IDE_DEBUG(QString("Send Protocol -- dev %1 [wrong type %2]").arg(mDevType).arg(tmpFrameType));
            tmpSendData.clear();
            pFrame->deleteLater();
            return 3;
        }
    }
    quint8 tmpCRC = 0;
#ifdef CRC
    int tmpCount = tmpSendData.count();
    for(int i=1;i<tmpCount;i++)
    {
        tmpCRC ^= tmpSendData.at(i);
    }
#endif
    tmpSendData.append(tmpCRC&0x7f);
    if(pFrame->mFlag == Flag_Normal)
        tmpSendData.append(D_FRAMEFOOTER);
    else
        tmpSendData.append(D_FRAMEFOOTER_ACK);
    //>@����
    qint64 tmpLenth = -1;
    if(pFrame->mDevType == P_UDP)
    {
        ToolUdp *tmpdev = VAR_CAST<ToolUdp*>(pFrame->mDev.data());
        if(tmpdev)
            tmpLenth = tmpdev->write(tmpSendData);
    }
    else if(pFrame->mDevType == P_TCP)
    {
        QTcpSocket *tmpdev = VAR_CAST<QTcpSocket*>(pFrame->mDev.data());
        if(tmpdev)
        {
            QString tmpIP = tmpdev->peerAddress().toString();
            quint16 tmpPort = tmpdev->peerPort();
            tmpLenth = tmpdev->write(tmpSendData);
        }
    }
    else if(pFrame->mDevType == P_COM)
    {
#ifndef PHONE
        SerialPort *tmpdev = VAR_CAST<SerialPort*>(pFrame->mDev.data());
        if(tmpdev)
            tmpLenth = tmpdev->write(tmpSendData);
#endif
    }
    else if(pFrame->mDevType == P_TEST)
    {
    }
    else
    {
        PhysicalProtocol *tmpdev = VAR_CAST<PhysicalProtocol*>(pFrame->mDev.data());
        if(tmpdev)
            tmpLenth = tmpdev->write(tmpSendData);
    }
    //IDE_DEBUG(QString("Send Protocol -- dev %1 [[%2 bytes]->[%3 bytes]]").arg(pFrame->mDevType).arg(tmpSendData.count()).arg((qint32)tmpLenth));
    tmpSendData.clear();
    pFrame->deleteLater();
    return tmpLenth;
}

quint32 PhysicalProtocol::SendFrame(FRAME_STRUCT *pFrame)
{
    if(!pFrame)
    {
        IDE_DEBUG(QString("Send Protocol -- dev %1 [null frame]").arg(mDevType));
        return 0;
    }
    pFrame->mFlag = Flag_Normal;  //>@ǿ������Ϊѯ��֡
    return SendData(pFrame);
}

quint32 PhysicalProtocol::ReplyFrame(FRAME_STRUCT *pFrame)
{
    if(!pFrame)
    {
        IDE_DEBUG(QString("Reply Protocol -- dev %1 [null frame]").arg(mDevType));
        return 0;
    }
    pFrame->mFlag = Flag_Ack;  //>@ǿ������Ϊ��Ӧ֡
    return SendData(pFrame);
}

quint32 PhysicalProtocol::ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pFrameBuffer)
{
    if(pFrameBuffer.isEmpty())
    {
        pFrameBuffer.clear();
        return 0;
    }
    if((m_FrameState != State_Wait) && (m_CurRevFrame && !m_CurRevFrame->mDev.isNull() && (m_CurRevFrame->mDev.data() != pDev)))  //>@һ֡δ������ǰ���������յ��������豸����������
    {
        pFrameBuffer.clear();
        return 0;
    }
    quint32 tmpCount = pFrameBuffer.count();
    //IDE_DEBUG(QString("Get Protocol -- dev %1 [%2 bytes]").arg(mDevType).arg(tmpCount));

//    for(int m=0;m<pFrameBuffer.count();m++)
//    {
//        IDE_TRACE_INT((quint8)(pFrameBuffer.at(m)));
//    }

    quint8 tmpValue = 0;
    quint32 i = 0;
    while(i < tmpCount)
    {
        switch(m_FrameState)
        {
            case State_Wait:
            {
                for(;i<tmpCount;i++)
                {
                    tmpValue = (quint8)(pFrameBuffer.at(i));
                    //IDE_TRACE_INT(tmpValue);
                    FRAME_FLAG tmpFlag = Flag_Unknow;
                    if(D_FRAMEHEADER == tmpValue)
                        tmpFlag = Flag_Normal;
                    else if(D_FRAMEHEADER_ACK == tmpValue)
                        tmpFlag = Flag_Ack;
                    if(tmpFlag != Flag_Unknow)
                    {
                        //>@�½�һ��֡
                        if(m_CurRevFrame)
                            m_CurRevFrame->deleteLater();
                        m_CurRevFrame = new FRAME_STRUCT;
                        m_CurRevFrame->SetDev(pType, pDev);
                        m_CurRevFrame->mFlag = tmpFlag;
                        m_FrameState = State_Header;
                        i++;
                        break;
                    }
                }
                break;
            }
            case State_Header:
            {
                m_CurRevFrame->mType = (FRAME_TYPE)(quint8)pFrameBuffer.at(i);  //>@ͨ����ͬ��֡����ȷ��֡����
#ifdef CRC
                m_CurRevFrame->mCRC ^= m_CurRevFrame->mType;
#endif
                //IDE_TRACE_INT(m_CurRevFrame->mType);
                switch(m_CurRevFrame->mType)
                {
                    case Type_FloorStatus:
                    case Type_FloorStatus2:
                    case Type_Urgency:
                    case Type_ButtonStatus:
                    case Type_DisabilityButton:
                    {
                        m_CurRevFrame->mLength = 7;
                        m_FrameState = State_Data;
                        i++;
                        break;
                    }
                    case Type_Heartbeat:
                    case Type_SetCmd:
                    case Type_ScanDev:
                    case Type_RegCmd:
                    case Type_FileTrans:
                    case Type_StreamTrans:
                    {
                        m_FrameState = State_CmdPara;
                        i++;
                        break;
                    }
                    default:
                    {
                        m_FrameState = State_Wait;  //>@����Ҫi++
                        break;
                    }
                }
                break;
            }
            case State_CmdPara:
            {
                m_CurRevFrame->mCmd = (quint8)pFrameBuffer.at(i);
                //IDE_TRACE_INT(m_CurRevFrame->mCmd);
                switch(m_CurRevFrame->mType)
                {
                    case Type_Heartbeat:
                    case Type_SetCmd:
                    case Type_ScanDev:
                    case Type_StreamTrans:
                    {
                        m_FrameState = State_DatalenH;
                        i++;
                        break;
                    }
                    case Type_RegCmd:
                    case Type_FileTrans:
                    {
                        m_FrameState = State_AddrH;
                        i++;
                        break;
                    }
                    default:
                    {
                        m_FrameState = State_Wait;  //>@����Ҫi++
                        break;
                    }
                }
                break;
            }
            case State_AddrH:
            {
                tmpValue = (quint8)(pFrameBuffer.at(i++));
                m_CurRevFrame->mAddr = tmpValue*256;
                m_FrameState = State_AddrL;
#ifdef CRC
                m_CurRevFrame->mCRC ^= tmpValue;
#endif
                break;
            }
            case State_AddrL:  //>@�������ݳ���
            {
                tmpValue = (quint8)(pFrameBuffer.at(i++));
                m_CurRevFrame->mAddr += tmpValue;
                m_FrameState = State_DatalenH;
#ifdef CRC
                m_CurRevFrame->mCRC ^= tmpValue;
#endif
                break;
            }
            case State_DatalenH:
            {
                tmpValue = (quint8)(pFrameBuffer.at(i++));
                m_CurRevFrame->mLength = tmpValue*256;
                m_FrameState = State_DatalenL;
#ifdef CRC
                m_CurRevFrame->mCRC ^= tmpValue;
#endif
                break;
            }
            case State_DatalenL:  //>@�������ݳ���
            {
                tmpValue = (quint8)(pFrameBuffer.at(i++));
                m_CurRevFrame->mLength += tmpValue;
                m_FrameState = State_Data;
#ifdef CRC
                m_CurRevFrame->mCRC ^= tmpValue;
#endif
                break;
            }
            case State_Data:
            {
                quint16 tmpLen = tmpCount-i;
                //IDE_TRACE_INT(tmpLen);
                if(tmpLen  >= m_CurRevFrame->mLength)
                {
                    m_CurRevFrame->mData.append(pFrameBuffer.mid(i, m_CurRevFrame->mLength));
                    i += m_CurRevFrame->mLength;
                    m_CurRevFrame->mLength = 0;
                    m_FrameState = State_CRC;
                }
                else
                {
                    m_CurRevFrame->mData.append(pFrameBuffer.mid(i));
                    i += tmpLen;
                    m_CurRevFrame->mLength -= tmpLen;
                }
#ifdef CRC
                int tmpCount = m_CurRevFrame->mData.count();
                for(int i=0;i<tmpCount;i++)
                {
                    m_CurRevFrame->mCRC ^= m_CurRevFrame->mData.at(i);
                }
#endif
                break;
            }
            case State_CRC:
            {
#ifdef CRC
                quint8 tmpCRC = (quint8)pFrameBuffer.at(i);
                if(tmpCRC != m_CurRevFrame->mCRC)
                {
                    m_FrameState = State_Wait;  //>@����Ҫi++
                    break;
                }
#endif
                m_FrameState = State_Footer;
                i++;
                break;
            }
            case State_Footer:
            {
                tmpValue = (quint8)(pFrameBuffer.at(i));
                //IDE_TRACE_INT(m_CurRevFrame->mFlag);
                //IDE_TRACE_INT(i);
                if(m_CurRevFrame->mFlag == Flag_Normal)
                {
                    if(D_FRAMEFOOTER == tmpValue)  //>@���յ���������֡
                    {
                        //>@�����
                        ParseFrame(m_CurRevFrame);
                        m_CurRevFrame = 0;
                        m_FrameState = State_Wait;  //>@��Ҫi++
                        i++;
                        break;
                    }
                }
                else if(m_CurRevFrame->mFlag == Flag_Ack)
                {
                    if(D_FRAMEFOOTER_ACK == tmpValue)  //>@���յ���������֡
                    {
                        //>@�����
                        ParseFrame(m_CurRevFrame);
                        m_CurRevFrame = 0;
                        m_FrameState = State_Wait;  //>@��Ҫi++
                        i++;
                        break;
                    }
                }
                else
                {
                    IDE_DEBUG(QString("Frame Flag is unknow[%1]").arg(m_CurRevFrame->mFlag));
                }
                m_FrameState = State_Wait;  //>@����Ҫi++
                break;
            }
            default:
            {
                m_FrameState = State_Wait;
                break;
            }
        }
    }
    pFrameBuffer.clear();
    return 1;
}

/**************************************
  �����룺
    0���βδ���
    1���ɹ�
    2��ִ֡��ʧ��
    3��֡���ʹ���
*/
quint32 PhysicalProtocol::ParseFrame(FRAME_STRUCT *pFrame)
{
    if(!pFrame)
    {
        IDE_DEBUG(QString("Get Protocol -- dev %1 get null frame!").arg(mDevType));
        return 0;
    }
    //IDE_TRACE_INT(pFrame->mType);
    quint32 ret = 1;
    switch(pFrame->mType)
    {
        case Type_Heartbeat:  //>@6sһ��
        {
            ret = ParseHeartbeatFrame(pFrame);
            break;
        }
        case Type_ScanDev:
        {
            ret = ParseScandevFrame(pFrame);
            break;
        }
        case Type_Urgency:
        {
            ret = ParseUrgencyFrame(pFrame);
            break;
        }
        case Type_FloorStatus:
        case Type_FloorStatus2:
        {
            ret = ParseFloorFrame(pFrame);
            break;
        }
        case Type_ButtonStatus:
        {
            ret = ParseButtonFrame(pFrame);
            break;
        }
        case Type_DisabilityButton:
        {
            ret = ParseDisBtnFrame(pFrame);
            break;
        }
        case Type_SetCmd:
        {
            ret = ParseConfFrame(pFrame);
            break;
        }
        case Type_RegCmd:
        {
            ret = ParseRegFrame(pFrame);
            break;
        }
        case Type_FileTrans:
        {
            ret = ParseFileFrame(pFrame);
            break;
        }
        case Type_StreamTrans:
        {
            ret = ParseStreamFrame(pFrame);
            break;
        }
        default:
        {
            pFrame->deleteLater();
            return 3;
        }
    }
    return ret;
}

quint32 PhysicalProtocol::ParseFloorFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)   //>@������������������
    {
        if(pFrame->mType == Type_FloorStatus)
        {
            volatile quint8 extendFun = pFrame->mData.at(6) & 0x7f;
            volatile quint8 tmpArrowFresh = extendFun & D_ArrowFresh;
            volatile quint8 tmpFloorFresh = extendFun & D_FloorFresh;
            volatile quint8 tmpFuncFresh = extendFun & D_FunctionFresh;
            if(m_ReuseType == RU_FLRFUNC)
            {
                if(tmpFloorFresh)
                {
                    tmpFuncFresh = 0;
                    emit sRadioFunEvent(HIDERC);
                }
                else if(tmpFuncFresh)
                {
                    tmpFloorFresh = 0;
                    emit sFloorEvent(HIDERC);
                }
            }
            //>@��ͷ
            ARROWSTATE arrowState = ArrowUnknow;
            if(tmpArrowFresh)
            {
                arrowState = (ARROWSTATE)((pFrame->mData.at(3)) & D_ArrowArea) ;
                emit sArrowEvent((quint32)arrowState);
            }
            //>@¥��
            if(tmpFloorFresh)
            {
                FLOOR_INFO tmpFloor(pFrame->mData.at(0), pFrame->mData.at(1), pFrame->mData.at(2));
                emit sFloorEvent(tmpFloor.toInt());
            }
            //>@��ѡ����
            quint32 radioFun = INVALIDRC;
            if(tmpFuncFresh)
            {
                radioFun = (quint32)(pFrame->mData.at(4)) & 0x0000007f;
                emit sRadioFunEvent(radioFun);
            }
            //>@����ʾͼƬ���ٲ�������
            //>@����
            quint8 audioNum = (quint8)INVALIDRC;
            if(extendFun & D_AudioFresh)
            {
                audioNum = (pFrame->mData.at(5)) & 0x7f;
                emit sMediaEvent(audioNum);
            }
        }
        else if(pFrame->mType == Type_FloorStatus2)
        {
            FLOOR_INFO tmpFloor(pFrame->mData.at(0), pFrame->mData.at(1), pFrame->mData.at(2));
            ARROWSTATE tmpArrow = (ARROWSTATE)((pFrame->mData.at(3)) & D_ArrowArea);
            quint8 tmpArrive = CharToU32(pFrame->mData.at(3)) & D_SignalArrive;
            quint8 tmpVoice = CharToU32(pFrame->mData.at(3)) & D_SignalVoice;
            quint8 tmpLowpower = CharToU32(pFrame->mData.at(3)) & D_SignalLowPower;
            quint32 tmpResvFunc = (CharToU32(pFrame->mData.at(4))<<14) + (CharToU32(pFrame->mData.at(5))<<7) + CharToU32(pFrame->mData.at(6));
            emit sFloorEvent(tmpFloor.toInt());
            emit sArrowEvent((quint32)tmpArrow);
            if(tmpResvFunc)
            {
                for(int i=0;i<21;i++)
                {
                    if((tmpResvFunc>>i) & 0x01)
                    {
                        emit sRadioFunEvent(i+1);
                        break;
                    }
                }
            }
            if(tmpArrive)
            {
                if(tmpArrow & D_ArrowUpDown == D_ArrowUpDown)
                    emit sMediaEvent(100+D_ArrowUpDown);
                else if(tmpArrow & D_ArrowUp)
                    emit sMediaEvent(100+D_ArrowUp);
                else if(tmpArrow & D_ArrowDown)
                    emit sMediaEvent(100+D_ArrowDown);
            }
            if(tmpVoice)
            {
                emit sMediaEvent(tmpFloor.toIndex());
            }
        }
    }
    else   //>@�����ӻ�����Ӧ����
    {
        quint8 tmpTouch = pFrame->mData.at(0) & 0x7f;
        if(tmpTouch)
            emit sTouchOutputChange(tmpTouch);
        quint8 tmpSwitch = pFrame->mData.at(1) & 0x7f;
        if(tmpSwitch)
            emit sSwitchOutputChange(tmpTouch);
    }
    pFrame->deleteLater();
    return 1;
}

quint32 PhysicalProtocol::ParseButtonFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)   //>@������������������
    {
        QList<QVariant> tmpBtnList;
        for(int i=0;i<7;i++)
        {
            quint8 tmpValue = pFrame->mData.at(i) & 0x7f;
            if(!tmpValue)
                continue;
            for(int j=0;j<7;j++)
            {
                if(tmpValue & (1<<j))
                {
                    int tmpFlrNum = i*7+j+1+m_BtnBaseNum.mNum;
                    if(tmpFlrNum > 42 || m_ValidBtns.isEmpty() || m_ValidBtns.contains(tmpFlrNum))
                        tmpBtnList.append(tmpFlrNum);
                }
            }
        }
        emit sButtonEvent(tmpBtnList, STATE_LIGHT);
    }
    else
    {
        QList<QVariant> tmpBtnList;
        for(int i=0;i<7;i++)
        {
            quint8 tmpValue = pFrame->mData.at(i) & 0x7f;
            if(!tmpValue)
                continue;
            for(int j=0;j<7;j++)
            {
                if(tmpValue & (1<<j))
                {
                    int tmpFlrNum = i*7+j+1+m_BtnBaseNum.mNum;
                    if(tmpFlrNum > 42 || m_ValidBtns.isEmpty() || m_ValidBtns.contains(tmpFlrNum))
                        tmpBtnList.append(tmpFlrNum);
                }
            }
        }
        emit sButtonEvent(tmpBtnList, STATE_PRESS);
    }
    pFrame->deleteLater();
    return 1;
}

quint32 PhysicalProtocol::ParseDisBtnFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)   //>@������������������
    {
        QList<QVariant> tmpBtnList;
        for(int i=0;i<6;i++)
        {
            quint8 tmpValue = pFrame->mData.at(i) & 0x7f;
            for(int j=0;j<7;j++)
            {
                if(tmpValue & (1<<j))
                    tmpBtnList.append(i*7+j+1);
            }
        }
        emit sButtonEvent(tmpBtnList, STATE_LIGHT);
    }
    else
    {
//        quint64 tmpBtnList = 0;
//        for(int i=0;i<6;i++)
//            tmpBtnList += ((pFrame->mData.at(i) & 0x7f)<<(i*7));
        QList<QVariant> tmpBtnList;
        for(int i=0;i<6;i++)
        {
            quint8 tmpValue = pFrame->mData.at(i) & 0x7f;
            for(int j=0;j<7;j++)
            {
                if(tmpValue & (1<<j))
                    tmpBtnList.append(i*7+j+1);
            }
        }
        emit sButtonEvent(tmpBtnList, STATE_PRESS);
    }
    pFrame->deleteLater();
    return 1;
}

quint32 PhysicalProtocol::ParseUrgencyFrame(FRAME_STRUCT *pFrame)
{
#ifdef DISPLAY
    PhysicalPlayer *tmpPlayer = mPlayer.data();
    if(tmpPlayer)
    {
        //>@������Ƶ�¼�
        quint8 tmpAudio = (quint8)pFrame->mData.at(0);//>@����������Ƶ�����۵�ǰ�豸�Ƿ��ڽ���״̬�����в���, ����λΪ��Ƶ��ţ�����λΪ��Ƶ������Ƶ
        if(tmpAudio)
        {
            quint8 tmpAudioNum = tmpAudio & 0x0f;
            quint8 tmpVolumeDiv = (tmpAudio & 0xf0) >> 4;
            if(tmpVolumeDiv && tmpAudioNum == 1)
            {
                //>@ֻ�е�֡�б仯ʱ���Ŵ�standby�л���
                if(!m_LCD.isNull() && (m_LCD.data()->mStandbyMode == 0))
                {
#ifdef DISPLAY      //>@�˳�standbyģʽʱ��Ҫִ�н��豸�ָ�����
                    emit sSetDevices(true, DM_STANDBY);
                    emit sFeedLCD();  //>@����ι�������ֹ����standbyģʽ
#endif
                }

                if(!mPlayer.isNull())
                {
                    MEDIA_PLAYER *tmpMediaPlayer = 0;
                    if(tmpMediaPlayer = tmpPlayer->GetMediaPlayer(AO_BEEP))
                    {
                        tmpMediaPlayer->SetEnable(true);
                        QString tmpFile = D_FSRCPATH + D_BOOTDIR + QString("beep.wav");
                        //if(tmpMediaPlayer->CanPlay(tmpFile))
                        {
                            if(tmpVolumeDiv == 0)
                                tmpPlayer->SetScaler(AO_BEEP, 0, false);  //>@false��ʾ��ֱ��ͬ���������������ǰ��������Ƶ�ڲ��ţ�������Ĵ���Ƶ������
                            else
                                tmpPlayer->SetScaler(AO_BEEP, (qreal)(1.0/(qreal)tmpVolumeDiv), false);
                            IDE_TRACE_INT(tmpVolumeDiv);
                            tmpMediaPlayer->Add(AO_BEEP, tmpFile, (int)LOOP_NONE, false, tmpPlayer->mAoParas[AO_BEEP].GetVolume());
                        }
                    }
                }
            }
        }
        //>@������������
        quint8 tmpAudioType = (quint8)pFrame->mData.at(1);//>@��Ƶ��������������λΪ��Ƶ���ͣ�����λΪ������Ƶֵ��1��ʾ¥�㱨վ��2��ʾ��վ�ӡ�4��ʾ������������������Ƶ����8��ʾBEEP
        if(tmpAudioType)
        {
            quint8 tmpVolumeDiv = (tmpAudioType & 0xf0) >> 4;
            qreal tmpScaler ;
            if(tmpVolumeDiv == 0)
                tmpScaler = 0;
            else
                tmpScaler = (qreal)(1.0/(qreal)tmpVolumeDiv);
            if(tmpAudioType & 0x01)
            {
                tmpPlayer->SetScaler(AO_LIFTFLR, tmpScaler);
            }
            if(tmpAudioType & 0x02)
            {
                tmpPlayer->SetScaler(AO_LIFTARV, tmpScaler);
            }
            if(tmpAudioType & 0x04)
            {
                tmpPlayer->SetScaler(AO_ADMUSIC, tmpScaler);
                tmpPlayer->SetScaler(AO_VIDEO, tmpScaler);
            }
            if(tmpAudioType & 0x08)
            {
                tmpPlayer->SetScaler(AO_BEEP, tmpScaler);
            }
        }
    }
#endif
    pFrame->deleteLater();
    return 0;
}

quint32 PhysicalProtocol::ParseScandevFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)  //>@����������ɨ��֡
    {
        ToolUdp *tmpUdp = VAR_CAST<ToolUdp*>(pFrame->mDev.data());
        if(tmpUdp)
        {
            QString tmpMsg = QString("IP[%1] Scanning...").arg(tmpUdp->m_SenderHost.toString());
            emit sInfo(tmpMsg);
        }
        //>@����ɨ��֡�е�ָ�����Ӧ����
        if(!pFrame->mData.isEmpty())
        {
            QString tmpRecv = QString::fromLatin1(pFrame->mData.data());
            m_RcvScanInfo = SetCmdInfo(tmpRecv);
            IDE_DEBUG(QString("Parse Scan -- dev %1 [%2]").arg(pFrame->mDevType).arg(tmpRecv));
            switch(m_RcvScanInfo.mType)
            {
                case SC_Connect:  //>@��������һ������
                {
                    slot_Connect();
                }
                case SC_Disconnect:
                {
                    slot_Disconnect();
                }
                default:
                    break;
            }
        }
        pFrame->mData.clear();
        pFrame->mData.append(m_DevManager->mDevCode.toLatin1());
        return ReplyFrame(pFrame);
    }
    else  //>@�����ǰ�豸ʱ�������򽫷�����Ӧ���豸�Ѽ�����
    {
        ToolUdp *tmpUdp = VAR_CAST<ToolUdp*>(pFrame->mDev.data());
        if(tmpUdp)
        {
            if(!tmpUdp->m_SenderHost.isNull())
            {
                quint16 tmpPort = tmpUdp->m_SenderPort;
                QString tmpDevStr;
                if(pFrame->mData.isEmpty())
                    tmpDevStr = QString("%1:%2").arg(tmpUdp->m_SenderHost.toString()).arg(tmpPort);
                else
                    tmpDevStr = QString("%1:%2:%3").arg(tmpUdp->m_SenderHost.toString()).arg(tmpPort).arg(QString(pFrame->mData));
                m_DevAddrList.append(tmpDevStr);
            }
        }
        pFrame->deleteLater();
    }
    return 1;
}

QByteArray PhysicalProtocol::SetRegister(quint16 pAddr, QByteArray pContent)
{
#ifdef DISPLAY
    if(pAddr < REG_ADDR_LG)  //>@PH_DEV
    {
        pAddr -= REG_ADDR_PH;
        PH_Type tmpType = (PH_Type)(pAddr / 100);
        quint16 tmpSubAddr = pAddr % 100;
        PhysicalDevice *tmpDev = m_DevManager->m_PHDevList.value(tmpType);
        if(tmpDev)
            tmpDev->WriteReg(tmpSubAddr, pContent);
    }
    else if(pAddr < REG_ADDR_COM)  //>@LG_DEV
    {
        pAddr -= REG_ADDR_LG;
        LG_Type tmpType = (LG_Type)(pAddr / 100);
        quint16 tmpSubAddr = pAddr % 100;
        LogicDevice *tmpDev = m_DevManager->m_LGDevList.value(tmpType);
        if(tmpDev)
            tmpDev->WriteReg(tmpSubAddr, pContent);
    }
    else  //>@COM_DEV
    {
        pAddr -= REG_ADDR_COM;
        COM_TYPE tmpType = (COM_TYPE)(pAddr / 100);
        quint16 tmpSubAddr = pAddr % 100;
        LogicThemeManager* tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
        if(tmpThemeManager)
        {
#ifdef SUPPORT_SCENE
            GraphicsOperate* tmpCom;
#elif SUPPORT_WIDGET
            WidgetBase* tmpCom;
#endif
            tmpCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpType);  //>@��ȡ�������������
            if(tmpCom)
            {
                tmpCom->WriteReg(tmpSubAddr, pContent);
            }
        }
    }
#endif
    return QByteArray();
}

QByteArray PhysicalProtocol::GetRegister(quint16 pAddr)
{
#ifdef DISPLAY
    if(pAddr < REG_ADDR_LG)  //>@PH_DEV
    {
        pAddr -= REG_ADDR_PH;
        PH_Type tmpType = (PH_Type)(pAddr / 100);
        quint16 tmpSubAddr = pAddr % 100;
        PhysicalDevice *tmpDev = m_DevManager->m_PHDevList.value(tmpType);
        if(tmpDev)
            return tmpDev->ReadReg(tmpSubAddr, 1000);
    }
    else if(pAddr < REG_ADDR_COM)  //>@LG_DEV
    {
        pAddr -= REG_ADDR_LG;
        LG_Type tmpType = (LG_Type)(pAddr / 100);
        quint16 tmpSubAddr = pAddr % 100;
        LogicDevice *tmpDev = m_DevManager->m_LGDevList.value(tmpType);
        if(tmpDev)
            return tmpDev->ReadReg(tmpSubAddr, 1000);
    }
    else  //>@COM_DEV
    {
        pAddr -= REG_ADDR_COM;
        COM_TYPE tmpType = (COM_TYPE)(pAddr / 100);
        quint16 tmpSubAddr = pAddr % 100;
        LogicThemeManager* tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
        if(tmpThemeManager)
        {
#ifdef SUPPORT_SCENE
            GraphicsOperate* tmpCom;
#elif SUPPORT_WIDGET
            WidgetBase* tmpCom;
#endif
            tmpCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpType);  //>@��ȡ�������������
            if(tmpCom)
            {
                return tmpCom->ReadReg(tmpSubAddr, 1000);
            }
        }
    }
#endif
    return QByteArray();
}

quint32 PhysicalProtocol::ParseRegFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)  //>@����������֡
    {
        if(pFrame->mCmd & 0x01)  //>@д
        {
            QByteArray tmpRet = SetRegister(pFrame->mAddr, pFrame->mData);
            if(pFrame->mCmd & 0x02)
            {
                pFrame->mData = tmpRet;
                return ReplyFrame(pFrame);
            }
        }
        else
        {
            pFrame->mData = GetRegister(pFrame->mAddr);
            return ReplyFrame(pFrame);
        }
    }
    else  //>@�ӻ���������Ӧ
    {
        if(pFrame->mCmd & 0x01)  //>@д
        {
        }
        else
        {
            m_ReadRegState = Success;
            m_ReadRegBuffer = pFrame->mData;
        }
    }
    pFrame->deleteLater();
    return 1;
}

quint32 PhysicalProtocol::ParseConfFrame(FRAME_STRUCT *pFrame)
{
    QString tmpRecv = QString::fromLatin1(pFrame->mData.data());
    m_RcvConfInfo = SetCmdInfo(tmpRecv);
    IDE_DEBUG(QString("Parse Conf -- dev %1 [%2]").arg(pFrame->mDevType).arg(tmpRecv));
    quint8 paraCnt = m_RcvConfInfo.mPara.count();
    if(pFrame->IsFromHost())
    {
        //>@����ָ��֧֧��һ��һ��ķ�ʽ�����ڲ�ʶ���ָ�Ĭ�Ͻ�����Fail��
        switch(m_RcvConfInfo.mType)
        {
            case SC_Init:
            {
                if(paraCnt == 1)  //init#bootup@code@passwd@location
                {
                    QStringList tmpList = m_RcvConfInfo.mPara.at(0).split("@");
                    if(tmpList.count() == 4)
                    {
                        if(!tmpList.at(0).compare("bootup", Qt::CaseInsensitive))
                        {
                            tmpList.removeFirst();
                            emit sInit(tmpList);  // code  Passwd  Location
                            m_RcvConfInfo.SetSuccess(true);
                        }
                        else
                        {
                            m_RcvConfInfo.SetSuccess(false);
                        }
                    }
                    else
                    {
                        m_RcvConfInfo.SetSuccess(false);
                    }
                }
#ifdef DISPLAY
                else if(paraCnt >= 2)
                {
                    //>@������֤
                    if(m_DevManager)
                    {
                        QString tmpUsrName = m_DevManager->mDevCode;
                        QString tmpPasswd = m_DevManager->mPasswd;
                        if(tmpUsrName.compare(m_RcvConfInfo.mPara.at(0), Qt::CaseInsensitive) || tmpPasswd.compare(m_RcvConfInfo.mPara.at(1)))
                        {
                            m_RcvConfInfo.SetSuccess(false);
                            ReplyConfFrame(m_RcvConfInfo, pFrame);
                            return 0;
                        }
                        m_RcvConfInfo.mPara.clear();
                        QList<PH_Type> tmpPHList = m_DevManager->m_PHDevList.keys();
                        for(int i=0;i<tmpPHList.count();i++)
                        {
                            PhysicalDevice* tmpPHDev = m_DevManager->m_PHDevList.value(tmpPHList.at(i));
                            if(tmpPHDev != 0)
                            {
                                QString tmpString = tmpPHDev->GetInformation();
                                if(!tmpString.isEmpty())
                                    m_RcvConfInfo.mPara<<tmpString;
                            }
                        }
                        QList<LG_Type> tmpLGList = m_DevManager->m_LGDevList.keys();
                        for(int i=0;i<tmpLGList.count();i++)
                        {
                            LogicDevice* tmpLGDev = m_DevManager->m_LGDevList.value(tmpLGList.at(i));
                            if(tmpLGDev != 0)
                            {
                                QString tmpString = tmpLGDev->GetInformation();
                                if(!tmpString.isEmpty())
                                    m_RcvConfInfo.mPara<<tmpString;
                            }
                        }
                        m_RcvConfInfo.SetSuccess(true);
                    }
                    else
                    {
                        m_RcvConfInfo.SetSuccess(false);
                    }
                }
#endif
                else
                {
                    m_RcvConfInfo.SetSuccess(false);
                }
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
#ifdef DISPLAY
            case SC_Set:
            case SC_Get:
            {
                if(paraCnt < 1)
                {
                    m_RcvConfInfo.SetSuccess(false);
                    ReplyConfFrame(m_RcvConfInfo, pFrame);
                    return 0;
                }
                xmlParse *tmpSetParse = new xmlParse;
                tmpSetParse->SetForcesave(false);
                for(int m=0;m<m_RcvConfInfo.mPara.count();m++)
                {
                    if(tmpSetParse->setContent(m_RcvConfInfo.mPara.at(m)))
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
                                    if(m_RcvConfInfo.mType == SC_Set)
                                        tmpPHDev->SetPara(tmpPHElement, tmpSetParse);
                                    else if(m_RcvConfInfo.mType == SC_Get)
                                        tmpPHDev->GetPara(tmpPHElement, tmpSetParse);
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
                                    if(m_RcvConfInfo.mType == SC_Set)
                                        tmpLGDev->SetPara(tmpLGElement, tmpSetParse);
                                    else if(m_RcvConfInfo.mType == SC_Get)
                                        tmpLGDev->GetPara(tmpLGElement, tmpSetParse);
                                }
                            }
                            else if(!tmpCommonType.compare("Theme"))
                            {
                                LogicThemeManager* tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
                                if(!tmpThemeManager)
                                {
                                    pFrame->deleteLater();
                                    return 0;
                                }
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
                                    if(m_RcvConfInfo.mType == SC_Set)
                                        tmpCom->SetPara(tmpComElement, tmpSetParse);
                                    else if(m_RcvConfInfo.mType == SC_Get)
                                        tmpCom->GetPara(tmpComElement, tmpSetParse);
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
                                        if(m_RcvConfInfo.mType == SC_Set)
                                            tmpSubCom->SetPara(tmpSubComElement, tmpSetParse);
                                        else if(m_RcvConfInfo.mType == SC_Get)
                                            tmpSubCom->GetPara(tmpSubComElement, tmpSetParse);
                                    }
                                }
                            }
                            //>@�޸ķ���ֵ
                            m_RcvConfInfo.mPara.replace(m, tmpSetParse->toString());
                        }
                    }
                }
                tmpSetParse->deleteLater();
                m_RcvConfInfo.SetSuccess(true);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
#endif
            case SC_Ls:
            {
                if(paraCnt < 1)
                {
                    m_RcvConfInfo.SetSuccess(false);
                    ReplyConfFrame(m_RcvConfInfo, pFrame);
                    return 0;
                }
                QString tmpRelPath = m_RcvConfInfo.mPara.first();
                if(tmpRelPath.isEmpty())
                {
                    m_RcvConfInfo.SetSuccess(false);
                    ReplyConfFrame(m_RcvConfInfo, pFrame);
                    return 0;
                }
                //>@ls#/usr/bst/#y/n(subfolder)  ->  ls#/usr/bst/abc.c:1024kb#...
                //>@������·���µ�Ŀ¼��Ϣ
                tmpRelPath.replace("\\", "/");
                if(!tmpRelPath.endsWith("/"))
                    tmpRelPath.append("/");
                QStringList tmpList = getAllInFloders(true, tmpRelPath, true);
                QString tmpFile;
                QString tmpFiles;
                foreach(tmpFile, tmpList)
                {
                    tmpFile.remove(tmpRelPath);
                    tmpFiles.append(tmpFile);
                    tmpFiles.append(QString(";"));
                }
                m_RcvConfInfo.mPara.append(tmpFiles);
                m_RcvConfInfo.SetSuccess(true);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_Copy:
            {
                if(m_RcvConfInfo.mPara.isEmpty())
                {
                    pFrame->deleteLater();
                    return 0;
                }
                QStringList tmpParaList = m_RcvConfInfo.mPara.first().split(";");
                bool flag = false;
                if(tmpParaList.count() >= 3)
                {
                    if(!(tmpParaList.at(0).compare("Y", Qt::CaseInsensitive) && QFile::exists(tmpParaList.at(2))))
                    {
                        QFileInfo tmpInfo(tmpParaList.at(1));
                        if(tmpInfo.isFile())
                        {
#if (defined(UBUNTU) || defined(LINUX))
                            QString tmpCmd = QString("rm %1").arg(tmpParaList.at(2));
                            system(tmpCmd.toLatin1().data());
                            IDE_TRACE_STR(tmpCmd);
                            tmpCmd = QString("cp %1 %2").arg(tmpParaList.at(1)).arg(tmpParaList.at(2));
                            system(tmpCmd.toLatin1().data());
                            IDE_TRACE_STR(tmpCmd);
#else
                            flag = CopyFile(tmpParaList.at(1), tmpParaList.at(2));
#endif
                        }
                        else
                            flag = CopyFolder(tmpParaList.at(1), tmpParaList.at(2));
                    }
                }
                m_RcvConfInfo.SetSuccess(flag);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_Cut:
            {
                if(m_RcvConfInfo.mPara.isEmpty())
                {
                    pFrame->deleteLater();
                    return 0;
                }
                QStringList tmpParaList = m_RcvConfInfo.mPara.first().split(";");
                bool flag = false;
                if(tmpParaList.count() >= 3)
                {
                    if(!(tmpParaList.at(0).compare("Y", Qt::CaseInsensitive) && QFile::exists(tmpParaList.at(2))))
                    {
                        flag = CutFile(tmpParaList.at(1), tmpParaList.at(2));
                    }
                }
                m_RcvConfInfo.SetSuccess(flag);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_Del:
            {
                if(m_RcvConfInfo.mPara.isEmpty())
                {
                    pFrame->deleteLater();
                    return 0;
                }
                QStringList tmpParaList = m_RcvConfInfo.mPara.first().split(";");
                bool flag = false;
                if(!tmpParaList.isEmpty())
                {
                    flag = DelFile(tmpParaList.at(0));
                }
                m_RcvConfInfo.SetSuccess(flag);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_Msg:
            {
                QString tmpMsg;
                int tmpDelay = 5000;
                if(paraCnt < 1)
                {
                    m_RcvConfInfo.SetSuccess(false);
                    ReplyConfFrame(m_RcvConfInfo, pFrame);
                    return 0;
                }
                else if(paraCnt < 2)
                {
                    tmpMsg = m_RcvConfInfo.toContent();
                }
                else
                {
                    tmpDelay = m_RcvConfInfo.mPara.takeLast().toInt();
                    tmpMsg = m_RcvConfInfo.toContent();
                }
                emit sMessage(tmpMsg, tmpDelay);
                pFrame->deleteLater();
                return 1;
            }
            case SC_Cmd:
            {
#ifdef DISPLAY   //>@ִ�б�׼ָ��
                for(int i=0;i<m_RcvConfInfo.mPara.count();i++)
                {
                    QString tmpCMD = m_RcvConfInfo.mPara.at(i);
                    if(!tmpCMD.compare("reboot", Qt::CaseInsensitive))
                    {
                        m_DevManager->PrepareRelease();
                    }
#ifdef WINDOWS
                    QProcess tmpProcess;
                    tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
                    tmpProcess.start(tmpCMD);
                    tmpProcess.waitForFinished();
                    tmpCMD.append('\n');
                    tmpCMD.append(tmpProcess.readAllStandardOutput());
                    tmpProcess.kill();
#else
                    system(tmpCMD.toLatin1().data());
#endif
                    m_RcvConfInfo.mPara.replace(i, tmpCMD+QString(":")+D_Success);
                }
#endif
                m_RcvConfInfo.SetSuccess(true);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_Zip:
            {
                emit sZip(pFrame);
                return 1;
            }
            case SC_Unzip:
            {
                emit sUnzip(pFrame);
                return 1;
            }
            case SC_SendFileTest:  //>@�ӻ����յ������ļ����Ե�ָ�ֻҪ�ж��Ƿ���Խ��մ��ļ�����
            {
                if(paraCnt < 4)  //SendFileTest#y#src#dst#filesize
                {
                    emit sProgress(QString("SendFile para count error!"), INVALIDRC);
                    m_RcvConfInfo.SetSuccess(false);
                    ReplyConfFrame(m_RcvConfInfo, pFrame);
                    return 0;
                }
                qint64 size = 0;
                QString tmpFilePath = m_RcvConfInfo.mPara.at(2);
                QString tmpFileName = getFileName(tmpFilePath);
                if(!StrToLongLong(m_RcvConfInfo.mPara.at(3), size) || (size<=0))
                {
                    emit sProgress(QString("Send %1 size error!").arg(tmpFileName), INVALIDRC);
                    m_RcvConfInfo.SetSuccess(false);
                    goto SENDACK;
                }
                if(m_RcvConfInfo.mPara.at(0).compare("y", Qt::CaseInsensitive) && QFile::exists(tmpFilePath)) //>@��������
                {
                    emit sProgress(QString("Send %1 override error!").arg(tmpFileName), INVALIDRC);
                    m_RcvConfInfo.SetSuccess(false);
                    goto SENDACK;
                }
                if(!m_WriteFileOp->bind(tmpFilePath, size))
                {
                    emit sProgress(QString("Send %1 bind error!").arg(tmpFileName), INVALIDRC);
                    m_RcvConfInfo.SetSuccess(false);
                    goto SENDACK;
                }
                m_RcvConfInfo.SetSuccess(true);
                emit sProgress(QString("Start Send %1 ...").arg(tmpFileName), INVALIDRC);
                m_FileTransState = Start;
SENDACK:
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_GetFileTest:  //>@�ӻ����յ������ļ����Ե�ָ���Ҫ�����ļ��Ĵ�С����
            {
                if(paraCnt < 3)
                {
                    emit sProgress(QString("GetFile para count error!"), INVALIDRC);
                    m_RcvConfInfo.SetSuccess(false);
                    ReplyConfFrame(m_RcvConfInfo, pFrame);
                    return 0;
                }
                if(paraCnt == 4)  //>@֡��С
                    SetMaxBlock(m_RcvConfInfo.mPara.at(3).toLongLong());
                QString tmpFilePath = m_RcvConfInfo.mPara.at(1);
                if(!m_ReadFileOp->bind(tmpFilePath))
                {
                    emit sProgress(QString("Get %1 bind error!").arg(QFileInfo(tmpFilePath).fileName()), INVALIDRC);
                    m_RcvConfInfo.SetSuccess(false);
                }
                else
                {
                    m_RcvConfInfo.mPara<<QString("%1").arg(m_ReadFileOp->mSize);
                    m_RcvConfInfo.SetSuccess(true);
                }
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 1;
            }
            case SC_StartGetFile:
            {
                if(m_ReadFileOp)
                {
                    QFileInfo tmpInfo(m_ReadFileOp->mFile);
                    emit sProgress(QString("Start Get %1 ...").arg(tmpInfo.fileName()), INVALIDRC);
                }
                m_FileTransState = Start;
                SendFileData(pFrame, true);
                return 1;
            }
            default:
            {
                m_RcvConfInfo.SetSuccess(false, true);
                ReplyConfFrame(m_RcvConfInfo, pFrame);
                return 0;
            }
        }
    }
    else
    {
        bool success = m_RcvConfInfo.isSuccess();
        m_ConfState = Success;//>@����Setcmd�ѽ��յ���Ӧ
        switch(m_RcvConfInfo.mType)
        {
            case SC_Init:
            {
                m_DevInfoList.clear();
                if(success)
                {
                    quint8 paraCnt = m_RcvConfInfo.mPara.count();
                    for(int i=0;i<paraCnt-1;i++)
                    {
                        QString tmpString = m_RcvConfInfo.mPara.at(i);
                        QStringList tmpList = tmpString.split(D_PARASPLIT);
                        if(tmpList.count()!=2)
                            continue;
                        m_DevInfoList.insert(tmpList.at(0), tmpList.at(1));
                    }
                    m_DevInfoTaskState = Success;
                }
                else
                {
                    m_DevInfoTaskState = Failed;
                }
                break;
            }
            case SC_Ls:
            {
                m_LsFileList.clear();
                //>@ls#path#list#success
                if(success && m_RcvConfInfo.mPara.count() >= 3)
                    m_LsFileList = m_RcvConfInfo.mPara.at(1).split(';');
                break;
            }
            case SC_Set:
            {
                m_SetTaskState = Success;
                break;
            }
            case SC_Get:
            {
                m_GetTaskState = Success;
                break;
            }
            case SC_Zip:
            {
                if(success)
                    m_ZipTaskState = Success;
                else
                    m_ZipTaskState = Failed;
                break;
            }
            case SC_Unzip:
            {
                if(success)
                    m_ZipTaskState = Success;
                else
                    m_ZipTaskState = Failed;
                break;
            }
            //>@�˴����ӻ�����ָ���б���ȥ��SendFileTest���������ļ��������ʱ�ڴӻ�������ȥ��SendFileTest
            case SC_SendFileTest:   //>@�������յ������ļ����Ե���Ӧ����������ͣ���ʼ����
            {
                if(m_FileTransState == Start && m_WaitTimeout)
                {
                    if(success)
                    {
                        SendFileData(pFrame, true);
                    }
                    else
                    {
                        m_WaitTimeout = 0;
                        m_FileTransState = Failed;
                    }
                }
                else
                {
                    m_FileTransState = Failed;
                }
                break;
            }
            case SC_GetFileTest:   //>@�������յ������ļ����Ե���Ӧ���ж��ļ���Ϣ�Ƿ���Խ���
            {
                if(m_FileTransState == Start && m_WaitTimeout)
                {
                    if(paraCnt>=5 && success)
                    {
                        qint64 size = 0;
                        if(!StrToLongLong(m_RcvConfInfo.mPara.at(paraCnt - 2), size) || (size <= 0))
                        {
                            m_FileTransState = Failed;
                            m_WaitTimeout = 0;
                            IDE_DEBUG(QString("GetFile -- dev %1 [SizeErr]").arg(mDevType));
                            break;
                        }
                        QString tmpFile = m_RcvConfInfo.mPara.at(2);
                        /*
                        if(!CheckCapacity(getFileDirectory(tmpFile), size))
                        {
                            m_FileTransState = Failed;
                            m_WaitTimeout = 0;
                            IDE_DEBUG(QString("GetFile -- dev %1 [CapacityErr]").arg(mDevType));
                            break;
                        }*/
                        if(!m_WriteFileOp->bind(tmpFile, size))
                        {
                            m_FileTransState = Failed;
                            m_WaitTimeout = 0;
                            IDE_DEBUG(QString("GetFile -- dev %1 [FileBindErr]").arg(mDevType));
                            break;
                        }
                        //>@m_WaitTimeout��Ϊ0�ͱ�ʾǰһ��ָ�û�г�ʱ��
                        if(m_WaitTimeout)
                        {
                            m_WaitTimeout = GetTransTimeout(size);
                        }
                        else
                        {
                            m_FileTransState = Failed;
                        }
                    }
                    else
                    {
                        m_WaitTimeout = 0;
                        m_FileTransState = Failed;
                    }
                }
                else
                {
                    m_FileTransState = Failed;
                }
                break;
            }
            default:
                break;
        }
        ProcConfQueue(m_RcvConfInfo, success);
        pFrame->deleteLater();
    }
    return 1;
}
/**************************************
  �����룺
    0��ʧ��
    1���ɹ�
    2������ʧ��
  eg��
    ��֡����
        �ӻ� ģ��һ��Ӧ��֡����֡������һ
        ���� ���յ���֡���嵥֡����λ��ͬʱ��������ֹλ��Ȼ�����֡�����������ݶ�
        �ӻ� ���յ����ݶ�,��������ֹ����
    ��ֹ���䣺
        �ӻ� ���յ���ֹ���䣬Ӧ��һ����ֹ���䡣
        ���� ���յ���ֹ����ֱ����ֹ��
*/
quint32 PhysicalProtocol::ParseFileFrame(FRAME_STRUCT *pFrame, bool pEmitPrg)
{
    if(pFrame->IsFromHost())  //>@�Է������ļ�������
    {
        bool tmpStop = pFrame->mCmd & Flag_Stop;
        if(tmpStop)
        {
            m_WriteFileOp->unbind();
            m_FileTransState = Failed;
            pFrame->deleteLater();

            if(pEmitPrg)
                emit sProgress(m_WriteFileOp->progress());
            return 1;
        }

        quint8 tmpDataPara = pFrame->mCmd & 0x0f;       //>@ֻ������λ
        quint8 tmpExtPara = pFrame->mCmd & 0xf0;        //>@ֻ������λ
        switch(tmpDataPara)
        {
            case Flag_FileData:
            case Flag_FileFooter:
            {
                if(m_FileTransState == Start)
                {
                    quint8 tmpState = 0;
                    if(!(m_WriteFileOp && m_WriteFileOp->mEnable && m_WriteFileOp->write(pFrame->mData)))  //>@���mData��NULL������True
                    {
                        tmpState = Flag_Stop;
                        m_FileTransState = Failed;
                        m_WriteFileOp->unbind();
                    }
                    //>@������
                    pFrame->ClearData();
                    //>@�鿴��չ����
                    if(tmpExtPara & Flag_OneTrans)  //>@����ǵ��δ��䣬��ô
                        tmpState = Flag_Stop;
                    //>@�޸Ĳ���ֵ
                    pFrame->mCmd = tmpDataPara + tmpState;
                }
                else
                {
                    m_FileTransState = Failed;
                    pFrame->mCmd = pFrame->mCmd&0x0f + Flag_Stop;
                    pFrame->ClearData();

                    if(m_WriteFileOp)
                        m_WriteFileOp->unbind();
                }
                quint32 ret = ReplyFrame(pFrame);
                if(pEmitPrg)
                    emit sProgress(m_WriteFileOp->progress());
                return ret;
            }
            case Flag_FileMD5:
            {
                //>@����Ҫ�����չ����
                //>@MD5У��
                if(m_FileTransState == Start)
                {
                    if(m_WriteFileOp && m_WriteFileOp->mEnable)
                    {
#if 0
                        QByteArray tmpArray = m_WriteFileOp->getmd5();
                        if(tmpArray != pFrame->mData)
                        {
                            pFrame->mData = tmpArray;
                            pFrame->mCmd = pFrame->mCmd&0x0f + Flag_Stop;  //>@�������ȣ�����MD5��ͬʱ����ֹλ
                            m_FileTransState = Failed;
                        }
                        else
                        {
                            pFrame->ClearData();
                            m_FileTransState = Success;
                        }
#else
                        pFrame->ClearData();
                        m_FileTransState = Success;
#endif
                    }
                    else
                    {
                        pFrame->mCmd = pFrame->mCmd&0x0f + Flag_Stop;
                        pFrame->ClearData();
                        m_FileTransState = Failed;
                    }
                }
                else
                {
                    m_FileTransState = Failed;
                    pFrame->mCmd = pFrame->mCmd&0x0f + Flag_Stop;
                    pFrame->ClearData();
                }
                quint32 ret = ReplyFrame(pFrame);
                if(m_WriteFileOp)
                    m_WriteFileOp->unbind();
                if(pEmitPrg)
                    emit sProgress(m_WriteFileOp->progress());
                return ret;
            }
            default:
            {
                pFrame->deleteLater();
                return 0;
            }
        }
    }
    else    //>@�Է�������Ӧ�����ݲ��������´η���
    {
        bool tmpStop = pFrame->mCmd & Flag_Stop;
        if(tmpStop)
        {
            IDE_TRACE();  //>@��֪��Ϊʲô�෢��һ��stop
            pFrame->deleteLater();
            m_ReadFileOp->unbind();
            m_FileTransState = Failed;

            if(pEmitPrg)
                emit sProgress(m_ReadFileOp->progress());
            return 1;
        }
        //>@
        quint8 tmpDataPara = pFrame->mCmd & 0x0f;       //>@ֻ������λ
        switch(tmpDataPara)
        {
            case Flag_FileData:
            {
                quint32 ret = 0;
                if(m_FileTransState == Start)
                {
                    ret = SendFileData(pFrame, pEmitPrg);  //>@�������Ѿ��н����ź��ˡ�
                }
                else
                {
                    pFrame->deleteLater();
                    m_ReadFileOp->unbind();
                    m_FileTransState = Failed;

                    if(pEmitPrg)
                        emit sProgress(m_ReadFileOp->progress());
                }
                return ret;//>@����ϴη����ļ�ʧ�ܣ����ش�, ����Ƿ��ͷ������ڷ������һ֡����ʱ�Զ��仯ΪFoot֡
             }
            case Flag_FileFooter: //>@�����յ�Footer��Ӧʱ����ʾ������ɣ���ʼ����MD5֡
            {
                quint32 ret = 0;
                if(m_FileTransState == Start)
                {
                    ret = SendFileMD5(pFrame);
                    m_FileTransState = Success;
                }
                else
                {
                    m_FileTransState = Failed;
                    pFrame->deleteLater();

                    if(m_ReadFileOp)
                        m_ReadFileOp->unbind();
                }

                //>@������MD5��Ӧʱ����ʾ����ɹ�
                if(pEmitPrg)
                    emit sProgress(m_ReadFileOp->progress());
                return ret;
            }
            default:
            {
                pFrame->deleteLater();
                return 0;
            }
        }
    }
    pFrame->deleteLater();
    return 0;
}

quint32 PhysicalProtocol::ParseStreamFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->IsFromHost())
    {
        switch(pFrame->mCmd)
        {
            case Flag_GetSnapShot:
            {
                pFrame->mData = PrintSence();
                break;
            }
            case Flag_GetCamera:
            {
                break;
            }
            case Flag_SendSnapShot:
            {
                m_SnapShotPix.loadFromData(pFrame->mData, "PNG");
                break;
            }
            case Flag_SendCamera:
            {
                break;
            }
            default:
            {
                pFrame->mData = D_Failed.toLatin1();
                break;
            }
        }
        return ReplyFrame(pFrame);
    }
    pFrame->deleteLater();
    return -1;
}

quint32 PhysicalProtocol::SendStream(STREAM_TYPE pType)
{
    QByteArray tmpArray;
    switch(pType)
    {
        case Flag_SendSnapShot:
        {
            tmpArray = PrintSence();
            break;
        }
        case Flag_SendCamera:
        {
            break;
        }
        default:
            return 0;
    }
    if(tmpArray.isEmpty())
    {
        IDE_TRACE();
        return 0;
    }
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->mType = Type_StreamTrans;
    tmpFrame->mCmd = pType;
    tmpFrame->mData = tmpArray;
    return SendFrame(tmpFrame);
}

quint32 PhysicalProtocol::GetStream(STREAM_TYPE pType)
{
    if(pType != Flag_GetSnapShot && pType != Flag_GetCamera)
        return 0;
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_StreamTrans;
    tmpFrame->mCmd = pType;
    return ReplyFrame(tmpFrame);
}

quint32 PhysicalProtocol::ParseHeartbeatFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)  //>@APP����������֡
    {
        m_HBState = Success;  //>@�����APP���յ���ʾ����������������֡��Ҳ���ǳɹ����յ�����
        if(pFrame->mCmd & 0x01)  //>@��������
        {
            if(pFrame->mData.count() == 4)
            {
                quint32 tmpTimeout = (CharToU32(pFrame->mData.at(0))<<24) +
                                     (CharToU32(pFrame->mData.at(1))<<16) +
                                     (CharToU32(pFrame->mData.at(2))<<8) +
                                     (CharToU32(pFrame->mData.at(3)));
                m_HBFreq[1].setnum(tmpTimeout);
                if(m_HBFreq[1].mNum)
                {
                    m_HBWait[1].SetTimeout(m_HBFreq[1].mNum);
                    m_HBWait[1].update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
            }
            else
            {
                m_HBFreq[1].setnum(0);
            }
        }
        else
        {
            m_HBFreq[1].setnum(0);
        }
        m_HBCount[1] = 0;  //>@�����յ�������Ӧ����HB��ʱ����
        if(pFrame->mCmd & 0x02)  //>@�Ƿ���Ӧ
        {
            return ReplyHBFrame(pFrame, QByteArray());
        }
    }
    else                              //>@
    {
        mAutoConnLog = false;
        m_HBCount[2] = 0;
        m_HBState = Success;
        if(!pFrame->mData.isEmpty())
        {

        }
    }
    pFrame->deleteLater();
    return 0;
}

//>@���pTimeoutΪ0��ʾ��������һ�����������մ˴ε���Ӧ�������ʾ����������������ôҲ��ʾ�ر�����
bool PhysicalProtocol::HB(quint32 pTimeout, bool pWait)
{
    quint32 tmpRet = SendHBFrame(pTimeout, true);
    if(!tmpRet)
        return false;

    //>@�������
    m_HBFreq[2].setnum(pTimeout);
    if(m_HBFreq[2].mNum)
    {
        m_HBWait[2].SetTimeout(m_HBFreq[2].mNum);
        m_HBWait[2].update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
    }

    if(pWait)
    {
        //>@�ȴ���һ֡����������
        if(!QtWait(m_HBState, Success, pTimeout))
            return false;
    }
    return true;
}

//>@���ڵ����ȴ��Է�������������֡������
bool PhysicalProtocol::WaitHB(quint32 pTimeout)
{
    m_HBState = Start;
    if(!QtWait(m_HBState, Success, pTimeout))
        return false;
    return true;
}

quint32 PhysicalProtocol::ReplyHBFrame(FRAME_STRUCT *pFrame, QByteArray pContent)
{
    if(!pFrame)
    {
        pFrame = new FRAME_STRUCT;
        pFrame->SetDev(m_CurDevType, m_CurTransDev);
        pFrame->mType = Type_Heartbeat;
    }
    pFrame->mData = pContent;
    IDE_DEBUG(QString("Reply Heartbeat..."));
    return ReplyFrame(pFrame);
}

quint32 PhysicalProtocol::SendHBFrame(quint32 pTimeout, bool pAck)
{
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_Heartbeat;
    if(pTimeout)
    {
        tmpFrame->mCmd = 0x01;
        tmpFrame->mData.resize(4);
        tmpFrame->mData[0] = (quint8)(pTimeout >> 24)&0xff;
        tmpFrame->mData[1] = (quint8)(pTimeout >> 16)&0xff;
        tmpFrame->mData[2] = (quint8)(pTimeout >> 8)&0xff;
        tmpFrame->mData[3] = (quint8)(pTimeout)&0xff;
    }
    else
    {
        tmpFrame->mCmd = 0x00;
        tmpFrame->mData.clear();
    }
    if(pAck)
        tmpFrame->mCmd |= 0x02;
    m_HBState = Start;
    IDE_DEBUG(QString("Send Heartbeat[%1]...").arg(pTimeout));
    return SendFrame(tmpFrame);
}

void PhysicalProtocol::slot_HBTimeout(quint32 pID)
{
    if(pID == 1)
    {
        Disconnected();
    }
    else if(pID == 2)
    {
        Disconnected();
    }
}

void PhysicalProtocol::slot_HB(quint32 pTimeout)
{
    SendHBFrame(pTimeout, true);
}

quint32 PhysicalProtocol::SendConfFrame(QString pInstruc)
{
    if(m_CurTransDev.isNull())
    {
        IDE_DEBUG(QString("Send Conf -- dev %1 [dev null]").arg(m_CurDevType));
        return 0;
    }
    IDE_DEBUG(QString("Send Conf -- dev %1 [%2]").arg(m_CurDevType).arg(pInstruc));
    m_ConfState = Start;
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_SetCmd;
    tmpFrame->mCmd = 0x01;
    tmpFrame->mData = pInstruc.toLatin1();
    return SendFrame(tmpFrame);
}

bool PhysicalProtocol::SendConfFrame(QString pInstruc, int pTimeout)
{
    if(SendConfFrame(pInstruc) >= D_FRAMEMINLEN)
    {
        if(QtWait((int &)m_ConfState, (int)Success, pTimeout))
        {
            //>@��ȡ��ǰ���յ�SetCmd����
            return true;
        }
    }
    return false;
}

quint32 PhysicalProtocol::ReplyConfFrame(SetCmdInfo pInfo, FRAME_STRUCT* pFrame)
{
    QString tmpInstruc = pInfo.toFrame();
    //IDE_DEBUG(QString("Reply Conf -- dev %1 [%2]").arg(pFrame->mDevType).arg(tmpInstruc));
    pFrame->mData = tmpInstruc.toLatin1();
    return ReplyFrame(pFrame);
}

/********************
    pPreFrameָ��һ֡���ݵ���Ӧ֡�����ݴ�֡ȷ����һ֡�ķ���
�����룺
    0������ʧ��
    1�����ͳɹ�
    >=5�����͵��ֽ���
*/
quint32 PhysicalProtocol::SendFileData(FRAME_STRUCT* pPreFrame, bool pEmitPrg)
{
    if(!pPreFrame)
        return 0;
    FRAME_STRUCT *tmpFrame = pPreFrame;
    if(tmpFrame->mType == Type_FileTrans && tmpFrame->mCmd & 0x03)  //>@�鿴�Ƿ�ΪData����Footer
    {
        bool tmpReTrans = tmpFrame->mCmd & Flag_ReTrans;
        bool tmpOneTrans = tmpFrame->mCmd & Flag_OneTrans;
        if(!tmpReTrans && !tmpOneTrans)  //>@�����֡�д��������Ҫ�ϵ��������᷵�ضϵ�����λ������֡���������ݶ�
            tmpFrame->mAddr++;
        tmpFrame->mCmd = Flag_FileData;
    }
    else
    {
        //>@�����һ֡��Ϊ��Ч������֡������Ϊ�ǵ�һ�δ���
        tmpFrame->mType = Type_FileTrans;
        tmpFrame->mCmd = Flag_FileData;
        tmpFrame->mAddr = 0;
    }
    tmpFrame->ClearData();
    //>@����ļ���������Stop��� �ӵ�0֡��ʼ����
    if(m_ReadFileOp && m_ReadFileOp->mEnable)
    {
        m_ReadFileOp->setpos(tmpFrame->mAddr*m_MaxBlockSize);
        if(m_ReadFileOp->mSurplus > m_MaxBlockSize)
        {
            if(m_ReadFileOp->read(m_MaxBlockSize))
                tmpFrame->mCmd = (tmpFrame->mCmd & 0xf0) + Flag_FileData;
            else
                tmpFrame->mCmd = (tmpFrame->mCmd & 0x0f) + Flag_Stop;
        }
        else
        {
            if(m_ReadFileOp->read(m_ReadFileOp->mSurplus))   //>@���Ҫ��ȡ�����ݸ���Ϊ0������false
                tmpFrame->mCmd = (tmpFrame->mCmd & 0xf0) + Flag_FileFooter;
            else
                tmpFrame->mCmd = (tmpFrame->mCmd & 0x0f) + Flag_Stop;
        }
        tmpFrame->mData = m_ReadFileOp->mCurPack;
    }
    else
    {
        tmpFrame->mCmd = (tmpFrame->mCmd & 0x0f) + Flag_Stop;
    }
    quint32 ret = SendFrame(tmpFrame);
    if(pEmitPrg && m_ReadFileOp)
        emit sProgress(m_ReadFileOp->progress());
    return ret;
}

//>@�����ж��Ƿ�Ϊ�ش�֡��Ȼ���ڷ���md5
quint32 PhysicalProtocol::SendFileMD5(FRAME_STRUCT* pPreFrame)
{
    if(!pPreFrame)
        return 0;
    FRAME_STRUCT *tmpFrame = pPreFrame;
    if(tmpFrame->mType == Type_FileTrans && tmpFrame->mCmd & 0x03)  //>@�鿴�Ƿ�ΪData����Footer
    {
        bool tmpReTrans = tmpFrame->mCmd & Flag_ReTrans;
        bool tmpOneTrans = tmpFrame->mCmd & Flag_OneTrans;
        if(tmpReTrans || tmpOneTrans)  //>@�����֡�д��������Ҫ�ϵ��������᷵�ضϵ�����λ������֡���������ݶ�
            return SendFileData(tmpFrame, false);
        tmpFrame->mCmd = Flag_FileMD5;
    }
    else
    {
        //>@�����һ֡��Ϊ��Ч������֡������Ϊ�ǵ�һ�δ���
        tmpFrame->mType = Type_FileTrans;
        tmpFrame->mCmd = Flag_FileMD5;
        tmpFrame->mAddr = 0;
    }
    tmpFrame->ClearData();
    //>@����ļ���������Stop����
    if(m_ReadFileOp && m_ReadFileOp->mEnable)
    {
        tmpFrame->mData = m_ReadFileOp->getmd5();
    }
    else
    {
        tmpFrame->mCmd += Flag_Stop;
    }
    return SendFrame(tmpFrame);
}

quint32 PhysicalProtocol::SendFileStop(FRAME_STRUCT* pPreFrame)
{
    FRAME_STRUCT *tmpFrame = 0;
    if(!pPreFrame)
    {
        tmpFrame = new FRAME_STRUCT;
        tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    }
    else
    {
        tmpFrame = pPreFrame;
    }
    tmpFrame->mType = Type_FileTrans;
    tmpFrame->mCmd = Flag_Stop;
    tmpFrame->mAddr = 0;
    tmpFrame->ClearData();
    return SendFrame(tmpFrame);
}

quint32 PhysicalProtocol::ReplyFileStop(FRAME_STRUCT* pPreFrame)
{
    IDE_TRACE();
    FRAME_STRUCT *tmpFrame = 0;
    if(!pPreFrame)
    {
        tmpFrame = new FRAME_STRUCT;
        tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    }
    else
    {
        tmpFrame = pPreFrame;
    }
    tmpFrame->mType = Type_FileTrans;
    tmpFrame->mCmd = Flag_Stop;
    tmpFrame->mAddr = 0;
    tmpFrame->ClearData();
    return ReplyFrame(tmpFrame);
}


quint32 PhysicalProtocol::ReplyReTrans(FRAME_STRUCT* pFrame, quint16 pAddr)
{
    if(!pFrame)
        return 0;
    if(pFrame->mType == Type_FileTrans)
    {
        pFrame->mCmd = (pFrame->mCmd & 0x0f) + Flag_ReTrans;
    }
    else
    {
        pFrame->mType = Type_FileTrans;
        pFrame->mCmd = Flag_ReTrans;
    }
    pFrame->mAddr = pAddr;
    pFrame->ClearData();
    return ReplyFrame(pFrame);
}

quint32 PhysicalProtocol::ReplyOneTrans(FRAME_STRUCT* pFrame, quint16 pAddr)
{
    if(!pFrame)
        return 0;
    if(pFrame->mType == Type_FileTrans)
    {
        pFrame->mCmd = (pFrame->mCmd & 0x0f) + Flag_OneTrans;
    }
    else
    {
        pFrame->mType = Type_FileTrans;
        pFrame->mCmd = Flag_OneTrans;
    }
    pFrame->mAddr = pAddr;
    pFrame->ClearData();
    return ReplyFrame(pFrame);
}

quint32 PhysicalProtocol::SendScanFrame(QString pContent)
{
    m_DevAddrList.clear();
    //>@
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(mDevType, this);
    tmpFrame->mType = Type_ScanDev;
    if(!pContent.isEmpty())
        tmpFrame->mData = pContent.toLatin1();
    return SendFrame(tmpFrame);
}

QStringList PhysicalProtocol::Scan(QString pContent, quint32 pTimeout)
{
    quint32 ret = SendScanFrame(pContent);
    if(ret)
    {
        QTime dieTime = QTime::currentTime().addMSecs(pTimeout);
        m_LoopFlag = true;
        while(m_Connected && QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        m_LoopFlag = false;
        if(m_DevAddrList.count() > 1)
            m_DevAddrList.removeDuplicates();
    }
    return m_DevAddrList;
}

void PhysicalProtocol::SendFloorFrame(quint32 pFloorNum, quint8 pFunc, quint32 pResvFunc)
{
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_FloorStatus2;
    tmpFrame->mData.resize(7);
    tmpFrame->mData[0] = (quint8)pFloorNum & 0x7f;
    tmpFrame->mData[1] = (quint8)(pFloorNum >> 8) & 0x7f;
    tmpFrame->mData[2] = (quint8)(pFloorNum >> 16) & 0x7f;
    tmpFrame->mData[3] = (quint8)pFunc & 0x7f;
    tmpFrame->mData[4] = (quint8)pResvFunc & 0x7f;
    tmpFrame->mData[5] = (quint8)(pResvFunc >> 8) & 0x7f;
    tmpFrame->mData[6] = (quint8)(pResvFunc >> 16) & 0x7f;
    SendFrame(tmpFrame);
}

void PhysicalProtocol::SendFloorFrame(quint8 pExFunc, quint32 pFloorNum, ARROWSTATE pArrowState, quint8 pRadioFun, quint8 pAudio)
{
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_FloorStatus;
    tmpFrame->mData.resize(7);
    if(pExFunc & 0x01)
    {
        tmpFrame->mData[0] = (quint8)pFloorNum & 0x7f;
        tmpFrame->mData[1] = (quint8)(pFloorNum >> 8) & 0x7f;
        tmpFrame->mData[2] = (quint8)(pFloorNum >> 16) & 0x7f;
    }
    if(pExFunc & 0x02)
        tmpFrame->mData[3] = (quint8)pArrowState & 0x7f;
    if(pExFunc & 0x04)
        tmpFrame->mData[4] = (quint8)pRadioFun & 0x7f;
    if(pExFunc & 0x08)
        tmpFrame->mData[5] = (quint8)pAudio & 0x7f;
    tmpFrame->mData[6] = (quint8)pExFunc & 0x7f;
    SendFrame(tmpFrame);
}

void PhysicalProtocol::ReplyFloorFrame(quint8 pTouch, quint8 pSwitch, quint8 pFun0)
{
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_FloorStatus;
    tmpFrame->mData.resize(7);
    tmpFrame->mData[0] = pTouch;
    tmpFrame->mData[1] = pSwitch;
    tmpFrame->mData[2] = pFun0;
    ReplyFrame(tmpFrame);
}

void PhysicalProtocol::SendButtonFrame(QVariant pBtnList, quint32 pDisablity)
{
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = (FRAME_TYPE)(Type_ButtonStatus+pDisablity);
    tmpFrame->mData.resize(7);
    QList<QVariant> tmpBtnList = pBtnList.toList();
    for(int i=0;i<tmpBtnList.count();i++)
    {
        int tmpBtn = tmpBtnList.at(i).toInt() + m_BtnBaseNum.mNum;
        if(tmpBtn > 42 || m_ValidBtns.isEmpty() || m_ValidBtns.contains(tmpBtn))
        {
            int row, colume;
            row = (tmpBtn-1)/7;
            colume = (tmpBtn-1)%7;
            tmpFrame->mData[row] = tmpFrame->mData[row] + (1<<colume);
        }
    }
    SendFrame(tmpFrame);
}

void PhysicalProtocol::ReplyButtonFrame(QVariant pBtnList, quint32 pDisablity)
{
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = (FRAME_TYPE)(Type_ButtonStatus+pDisablity);
    tmpFrame->mData.resize(7);
    for(int i=0;i<7;i++)
        tmpFrame->mData[i] = 0;
    QList<QVariant> tmpBtnList = pBtnList.toList();
    if(tmpBtnList.count() > 0)
    {
        for(int i=0;i<tmpBtnList.count();i++)
        {
            int tmpBtn = tmpBtnList.at(i).toInt() + m_BtnBaseNum.mNum;
            if(tmpBtn > 42 || m_ValidBtns.isEmpty() || m_ValidBtns.contains(tmpBtn))
            {
                int row, colume;
                row = (tmpBtn-1)/7;
                if(row >= 7)
                    continue;
                colume = (tmpBtn-1)%7;
                tmpFrame->mData[row] = tmpFrame->mData[row] + (1<<colume);
            }
        }
    }
    ReplyFrame(tmpFrame);
}

void PhysicalProtocol::slot_Zip(FRAME_STRUCT* pFrame)
{
    FRAME_STRUCT *tmpFrame = (FRAME_STRUCT *)pFrame;
    if(tmpFrame)
    {
        bool flag = false;
        SetCmdInfo tmpInfo(QString::fromLatin1(pFrame->mData.data()));
        if(tmpInfo.mPara.count() >= 2)
        {
            flag = Zip(tmpInfo.mPara.at(0), tmpInfo.mPara.at(1), 600000, true);  //>@��ʱʱ��10min
            tmpInfo.SetSuccess(flag);
        }
        ReplyConfFrame(tmpInfo, pFrame);
    }
}

void PhysicalProtocol::slot_Unzip(FRAME_STRUCT* pFrame)
{
    FRAME_STRUCT *tmpFrame = (FRAME_STRUCT *)pFrame;
    if(tmpFrame)
    {
        bool flag = false;
        SetCmdInfo tmpInfo(QString::fromLatin1(pFrame->mData.data()));
        if(tmpInfo.mPara.count() >= 2)
        {
            flag = UnZip(tmpInfo.mPara.at(0), tmpInfo.mPara.at(1), 600000, true);  //>@��ʱʱ��10min
            tmpInfo.SetSuccess(flag);
        }
        ReplyConfFrame(tmpInfo, pFrame);
    }
}

void PhysicalProtocol::slot_ZipTaskEnd()
{
    IDE_TRACE();
    m_ZipTaskState = Success;
    emit sProgress(QString("Un/Zip end"), 100);
}

void PhysicalProtocol::slot_ZipTaskErr()
{
    //IDE_TRACE();
    m_ZipTaskState = Failed;
    emit sProgress(QString("Un/Zip error"), 100);
}

void PhysicalProtocol::slot_ZipTaskProgress(int ratio)
{
    //IDE_TRACE_INT(ratio);
    emit sProgress(QString("Un/Zipping ..."), ratio);
}

//UnZip("D:/MyProject/IDE/BST_DISPLAY/SCENE/debug/RuntimeRC/FSRC/THEME/Sodimas.zip", "D:/MyProject/IDE/BST_DISPLAY/SCENE/debug/RuntimeRC/FSRC/THEME/", 60000, true);
bool PhysicalProtocol::Zip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal)
{
    if(isLocal)
    {
#ifdef SURRPORT_ZIP
        QThread ZipThread;
        ZipTask compressZipTask;
        connect(this,SIGNAL(sZip(QString, QString, bool, bool)),&compressZipTask,SLOT(Zip(QString, QString, bool, bool)),Qt::QueuedConnection);
        connect(&compressZipTask,SIGNAL(end()),this,SLOT(slot_ZipTaskEnd()),Qt::QueuedConnection);
        connect(&compressZipTask,SIGNAL(error()),this,SLOT(slot_ZipTaskErr()),Qt::QueuedConnection);
        connect(&compressZipTask,SIGNAL(progress(quint32)),this,SIGNAL(sProgress(quint32)),Qt::QueuedConnection);
        compressZipTask.moveToThread(&ZipThread);
        ZipThread.start(QThread::LowPriority);
        ZipThread.wait(10);
        m_ZipTaskState = Start;
        emit sZip(pSrc, pDst, true, false);
        emit sInfo(QString("Zipping Started"), MSG_PERMANENT, DM_NORMAL);
        while((m_ZipTaskState == Start) && (pTimeout > 0))
        {
            pTimeout -= 100;
            QtSleep(100);
        }
        if(m_ZipTaskState == Success)
            emit sInfo(QString("Zipping Success"), MSG_TEMPORARY, DM_NORMAL);
        else
            emit sInfo(QString("Zipping Failed"), MSG_TEMPORARY, DM_NORMAL);
        disconnect(this,SIGNAL(sZip(QString, QString, bool, bool)), 0, 0);
        disconnect(&compressZipTask,SIGNAL(end()), 0, 0);
        disconnect(&compressZipTask,SIGNAL(error()), 0, 0);
        disconnect(&compressZipTask,SIGNAL(progress(quint32)), 0, 0);
        ZipThread.terminate();
        ZipThread.wait(100);
        return (m_ZipTaskState == Success);
#endif
    }
    else
    {
        m_ZipTaskState = Start;
        SendConfFrame(QString("Zip#%1#%2").arg(pSrc).arg(pDst));  //Zip#dir#file.zip
        while(m_Connected && (pTimeout>0) && (m_ZipTaskState==Start))
        {
            pTimeout -= 100;
            QtSleep(100);
        }
        if(m_Connected)
        {
            if(m_ZipTaskState == Start)  //>@�����ʱ��������״̬ΪΪFailed
                m_ZipTaskState = Timeout;
        }
        else
        {
            m_ZipTaskState = Stopped;
        }
        return (m_ZipTaskState == Success);
    }
    return false;
}

bool PhysicalProtocol::UnZip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal)
{
    if(isLocal)
    {
#ifdef SURRPORT_ZIP
        QThread ZipThread;
        ZipTask decompressZipTask;
        connect(this,SIGNAL(sUnzip(QString, QString, bool, bool)),&decompressZipTask,SLOT(Unzip(QString, QString, bool, bool)),Qt::QueuedConnection);
        connect(&decompressZipTask,SIGNAL(end()),this,SLOT(slot_ZipTaskEnd()),Qt::QueuedConnection);
        connect(&decompressZipTask,SIGNAL(error()),this,SLOT(slot_ZipTaskErr()),Qt::QueuedConnection);
        connect(&decompressZipTask,SIGNAL(progress(quint32)),this,SIGNAL(sProgress(quint32)),Qt::QueuedConnection);
        decompressZipTask.moveToThread(&ZipThread);
        ZipThread.start(QThread::LowPriority);
        ZipThread.wait(10);
        m_ZipTaskState = Start;
        emit sUnzip(pSrc, pDst, true, false);  //>@���һ������ָ�Ƿ���Խ�����Ϣ
        emit sInfo(QString("Unzipping Started"), MSG_PERMANENT, DM_NORMAL);
        while((m_ZipTaskState == Start) && (pTimeout > 0))
        {
            pTimeout -= 10;
            QtSleep(10);
        }
        if(m_ZipTaskState == Success)
            emit sInfo(QString("Unzipping Success"), MSG_TEMPORARY, DM_NORMAL);
        else
            emit sInfo(QString("Unzipping Failed"), MSG_TEMPORARY, DM_NORMAL);
        disconnect(this,SIGNAL(sUnzip(QString, QString, bool, bool)), 0, 0);
        disconnect(&decompressZipTask,SIGNAL(end()), 0, 0);
        disconnect(&decompressZipTask,SIGNAL(error()), 0, 0);
        disconnect(&decompressZipTask,SIGNAL(progress(quint32)), 0, 0);
        ZipThread.terminate();
        ZipThread.wait(100);
        return (m_ZipTaskState == Success);
#endif
    }
    else
    {
        m_ZipTaskState = Start;
        SendConfFrame(QString("Unzip#%1#%2").arg(pSrc).arg(pDst));  //Zip#file.zip#dir
        while(m_Connected && (pTimeout>0) && (m_ZipTaskState==Start))
        {
            pTimeout -= 100;
            QtSleep(100);
        }
        if(m_Connected)
        {
            if(m_ZipTaskState == Start)  //>@�����ʱ��������״̬ΪΪFailed
                m_ZipTaskState = Timeout;
        }
        else
        {
            m_ZipTaskState = Stopped;
        }
        return (m_ZipTaskState == Success);
    }
    return false;
}

void PhysicalProtocol::Bootup()
{
#ifdef DISPLAY
    SendConfFrame(QString("Init#bootup@%1@%2@%3").arg(m_DevManager->mDevCode).arg(m_DevManager->mPasswd).arg(m_DevManager->mLocation));
#endif
}

bool PhysicalProtocol::WriteReg(quint16 pRegAddr, QByteArray pContent, bool pNeedAck)
{
    if(m_CurTransDev.isNull())
    {
        IDE_DEBUG(QString("Write Register -- dev %1 [dev null]").arg(m_CurDevType));
        return 0;
    }
    //IDE_DEBUG(QString("Write Register -- dev %1").arg(m_CurDevType));
    m_ConfState = Start;
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_RegCmd;
    if(pNeedAck)
        tmpFrame->mCmd = 0x03;
    else
        tmpFrame->mCmd = 0x01;
    tmpFrame->mAddr = pRegAddr;
    tmpFrame->mData = pContent;
    return SendFrame(tmpFrame);
}

QByteArray PhysicalProtocol::ReadReg(quint16 pRegAddr, quint32 pTimeout)
{
    if(m_CurTransDev.isNull())
    {
        IDE_DEBUG(QString("Write Register -- dev %1 [dev null]").arg(m_CurDevType));
        return 0;
    }
    //IDE_DEBUG(QString("Read Register -- dev %1").arg(m_CurDevType));
    m_ConfState = Start;
    FRAME_STRUCT *tmpFrame = new FRAME_STRUCT;
    tmpFrame->SetDev(m_CurDevType, m_CurTransDev);
    tmpFrame->mType = Type_RegCmd;
    tmpFrame->mCmd = 0x02;
    tmpFrame->mAddr = pRegAddr;
    bool tmpRet = SendFrame(tmpFrame);
    if(tmpRet)
    {
        m_ReadRegState = Start;
        while(m_Connected && (pTimeout>0) && (m_ReadRegState==Start))
        {
            pTimeout -= 100;
            QtSleep(100);
        }
        if(m_ReadRegState != Success)
            return QByteArray();
        return m_ReadRegBuffer;
    }
    return QByteArray();
}

bool PhysicalProtocol::Set(QString pContent, qint64 pTimeout)
{
    if(pTimeout <= 0)
        return (bool)(SendConfFrame(QString("Set#%1").arg(pContent)) > 0);
    m_SetTaskState = Start;
    quint32 ret = SendConfFrame(QString("Set#%1").arg(pContent));
    if(!ret)
    {
        m_SetTaskState = Failed;
        return false;
    }
    if(!m_Connected || !QtWait(m_SetTaskState, Success, pTimeout))
    {
        return false;
    }
    return true;
}

bool PhysicalProtocol::Get(QString pContent, qint64 pTimeout)
{
    if(pTimeout <= 0)
        return (bool)(SendConfFrame(QString("Get#%1").arg(pContent)) > 0);
    m_SetTaskState = Start;
    quint32 ret = SendConfFrame(QString("Set#%1").arg(pContent));
    if(!ret)
    {
        m_SetTaskState = Failed;
        return false;
    }
    if(!QtWait(m_SetTaskState, Success, pTimeout))
    {
        return false;
    }
    return true;
}

void PhysicalProtocol::ClearConfQueue()
{
    m_ConfQueue.clear();
}

void PhysicalProtocol::PushConfQueue(SetCmdInfo pInfo)
{
    if((pInfo.mType == SC_Unknow) || (pInfo.mDietime < 5))
        return;
    if(m_ConfQueue.isEmpty())
    {
        m_ConfQueue.enqueue(pInfo);
        SendConfFrame(pInfo);
    }
    else
    {
        m_ConfQueue.enqueue(pInfo);
    }
}

quint32 PhysicalProtocol::SendConfFrame(SetCmdInfo pInfo)
{
    if(pInfo.mType == SC_Unknow)
        return 0;
    m_SendConfInfo = pInfo;
    //>@���������齨Frame
    switch(pInfo.mType)
    {
        case SC_SendFileTest:
        {
            if(pInfo.mPara.count() >= 3)
            {
                QString tmpFilePath = pInfo.mPara.at(1);
                bool flag = m_ReadFileOp->bind(tmpFilePath);
                if(flag)
                {
                    qint64 tmpSize = m_ReadFileOp->mSize;
                    if(tmpSize)
                    {
                        m_WaitTimeout = GetTransTimeout(tmpSize);
                        pInfo.mPara.append(QString::number(tmpSize));
                        goto SENDFRAME;
                    }
                }
            }
            m_WaitTimeout = 0;
            //>@�����ǰָ��ִ��ʧ�ܣ���ֱ�ӽ��뻺�崦�����
            ProcConfQueue(pInfo, false);
            return 0;
        }
        case SC_GetFileTest:
        {
            if(pInfo.mPara.count() >= 3)
            {
                QString tmpFilePath = pInfo.mPara.at(2);
                if(!(pInfo.mPara.at(0).compare("y", Qt::CaseInsensitive) && QFile::exists(tmpFilePath))) //>@�������ļ�
                {
                    goto SENDFRAME;
                }
            }
            m_WaitTimeout = 0;
            ProcConfQueue(pInfo, false);
            return 0;
        }
        default:
            break;
    }
SENDFRAME:
    StartConfMonitor(pInfo.mDietime);
    return SendConfFrame(pInfo.toFrame());
}

void PhysicalProtocol::SendFileTest(QString pCover, QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd)
{
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_SendFileTest;
    tmpInfo.mPara<<pCover<<pSrc<<pDst;
    tmpInfo.mRatio = pRation;
    tmpInfo.mDietime = pDietime * pRation;
    tmpInfo.mFatherType = pFatherType;
    tmpInfo.mEndFlag = pEnd;
    PushConfQueue(tmpInfo);
}

void PhysicalProtocol::GetFileTest(QString pCover, QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd)
{
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_GetFileTest;
    tmpInfo.mPara<<pCover<<pSrc<<pDst;
    tmpInfo.mRatio = pRation;
    tmpInfo.mDietime = pDietime * pRation;
    tmpInfo.mFatherType = pFatherType;
    tmpInfo.mEndFlag = pEnd;
    PushConfQueue(tmpInfo);
}

void PhysicalProtocol::StartGetFile(quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd)
{
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_StartGetFile;
    tmpInfo.mRatio = pRation;
    tmpInfo.mDietime = pDietime * pRation;
    tmpInfo.mFatherType = pFatherType;
    tmpInfo.mEndFlag = pEnd;
    PushConfQueue(tmpInfo);
}

void PhysicalProtocol::ListPath(QString pPath, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd)
{
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_Ls;
    tmpInfo.mPara<<pPath;
    tmpInfo.mRatio = pRation;
    tmpInfo.mDietime = pDietime * pRation;
    tmpInfo.mFatherType = pFatherType;
    tmpInfo.mEndFlag = pEnd;
    PushConfQueue(tmpInfo);
}

void PhysicalProtocol::Zip(QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd)
{
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_Zip;
    tmpInfo.mPara<<pSrc<<pDst;
    tmpInfo.mRatio = pRation;
    tmpInfo.mDietime = pDietime * pRation;
    tmpInfo.mFatherType = pFatherType;
    tmpInfo.mEndFlag = pEnd;
    PushConfQueue(tmpInfo);
}

void PhysicalProtocol::Unzip(QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd)
{
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_Unzip;
    tmpInfo.mPara<<pSrc<<pDst;
    tmpInfo.mRatio = pRation;
    tmpInfo.mDietime = pDietime * pRation;
    tmpInfo.mFatherType = pFatherType;
    tmpInfo.mEndFlag = pEnd;
    PushConfQueue(tmpInfo);
}

bool PhysicalProtocol::StopTask()
{
    ClearConfQueue();
    m_StopTaskFlag = true;
}

/**************************************
  �����룺
    0����������
    1���ɹ�
    2���ǻ�����ָ��
  ���յ���Ӧ֡�󣬴���ǰ��ָ�������pSuccess��ʾ��ǰָ���Ƿ�ִ��ʧ��
*/
quint32 PhysicalProtocol::ProcConfQueue(SetCmdInfo &pInfo, bool pSuccess)
{
    if(m_ConfQueue.isEmpty())
        return 0;
    SetCmdInfo tmpInfo = m_ConfQueue.head();  //>@��ȡ��ǰ����ִ�еĻ���ָ��
    if(tmpInfo.mType != pInfo.mType) //>@�鿴��ǰ�����ָ���Ƿ�Ϊ�������ĵ�ǰָ��
    {
        IDE_TRACE_INT(tmpInfo.mType);
        return 2;
    }

    //>@������ص��ǵ�ǰ���ڴ����ָ���رճ�ʱ��ⶨʱ��
    StopConfMonitor();

    if(tmpInfo.mFatherType == SC_Unknow)   //>@�����ǰָ�����ĳ������ָ���е���ָ��
    {
        IDE_TRACE_INT(tmpInfo.mType);
        return 1;
    }

    m_QueueProgress += 100 * tmpInfo.mRatio;
    emit sProgress(m_QueueProgress);

    //>@��Ե�ǰָ������ִ�гɹ�������ǰָ���
    SETCMD_TYPE tmpFtherType = SC_Unknow;  //>@����ָ������
    if(pSuccess)
    {
        m_ConfQueue.dequeue();
        if(tmpInfo.mEndFlag)
            tmpFtherType = tmpInfo.mFatherType;
    }
    else
    {
        //>@���ĳ����ָ��ִ�в��ɹ�����ôɾ���������ָ��
        m_ConfQueue.dequeue();
        while(!m_ConfQueue.isEmpty())
        {
            SetCmdInfo headEvent = m_ConfQueue.head();
            m_ConfQueue.dequeue();
            if(headEvent.mEndFlag)
            {
                tmpFtherType = tmpInfo.mFatherType;
                break;
            }
        }
    }
    //>@��ǰָ���յ���Ӧ�󣬷�����һ��ָ��
    if(!m_ConfQueue.isEmpty())
        SendConfFrame(m_ConfQueue.head());

    //>@����ǰ����ָ���ȡ����ָ��Ľ��
    switch(tmpFtherType)
    {
        case SC_GetDir:
        {
            m_GetDirTaskState = (TaskState)pSuccess;
            emit sGetDir(pSuccess);
            emit sProgress(0);
            return 1;
        }
        case SC_SendDir:
        {
            m_SendDirTaskState = (TaskState)pSuccess;
            emit sSendDir(pSuccess);
            emit sProgress(0);
            return 1;
        }
        default:
            return 1;
    }
    return 1;
}

bool PhysicalProtocol::StartConfMonitor(qint64 pTime)
{
    if(pTime <= 0)
        return false;
    m_ConfMonitorTimer.start(pTime);
    return true;
}

void PhysicalProtocol::StopConfMonitor()
{
    m_ConfMonitorTimer.stop();
}

void PhysicalProtocol::slot_ConfMonitorTimeout()
{
    if(m_ConfQueue.isEmpty())
        return;
    ProcConfQueue(m_ConfQueue.head(), false);  //>@��ʾֱ�Ӹ��߻�������ǰָ��ִ�г����ˡ�
}

bool PhysicalProtocol::GetDevInfo(QString pName, QString pPasswd, quint64 pLife)
{
    m_DevInfoTaskState = Start;
    m_DevInfoList.clear();
    quint32 ret = SendConfFrame(QString("Init#%1#%2").arg(pName).arg(pPasswd));
    if(ret > 0)
    {
        while(m_Connected && (pLife>0) && (m_DevInfoTaskState==Start))
        {
            pLife -= 100;
            QtSleep(100);
        }
    }
    if(m_DevInfoTaskState == Failed)
        return false;
    else if(m_DevInfoTaskState == Success)
        return true;
    return false;
}

bool PhysicalProtocol::SendFile(QString pCover, QString pSrc, QString pDst)
{
    emit sProgress(QString("Send %2 ...").arg(getFileName(pSrc)), INVALIDRC);
__SendFile_:
    m_RetransFile = false;
    m_WaitTimeout = 0;
    m_FileTransState = Start;
    SendFileTest(pCover, pSrc, pDst, D_SETFRAME_TIMEOUT, 1, SC_SendFile, true);
    m_WaitTimeout += D_SETFRAME_TIMEOUT; //>@��ProcConf�и����ļ���С�ٴν��и�ֵ
    while(m_Connected && (m_WaitTimeout>0) && (m_FileTransState==Start) && !m_RetransFile)
    {
        m_WaitTimeout -= 100;
        QtSleep(100);
    }
    if(m_Connected)
    {
        if(m_FileTransState==Start)
        {
            m_FileTransState = Timeout;
            SendFileStop(0);
        }
        //>@��ʾ��ǰ�����Զ�����״̬
        if(m_RetransFile)
        {
            m_ReadFileOp->unbind();
            goto __SendFile_;
        }
    }
    else
    {
        m_FileTransState = Stopped;
    }
    m_ReadFileOp->unbind();
    if(m_FileTransState == Success)
        return true;
    return false;
}

bool PhysicalProtocol::GetFile(QString pCover, QString pSrc, QString pDst)
{
    emit sProgress(QString("Get %2 ...").arg(getFileName(pSrc)), INVALIDRC);
__GetFile_:
    m_RetransFile = false;
    m_WaitTimeout = 0;
    m_FileTransState = Start;
    GetFileTest(pCover, pSrc, pDst, D_SETFRAME_TIMEOUT, 1, SC_GetFile, false);
    m_WaitTimeout += D_SETFRAME_TIMEOUT;
    StartGetFile(D_SETFRAME_TIMEOUT, 1, SC_GetFile, true);
    m_WaitTimeout += D_SETFRAME_TIMEOUT;
    QtSleep(100);
    while(m_Connected && (m_WaitTimeout>0) && (m_FileTransState==Start) && !m_RetransFile)
    {
        m_WaitTimeout -= 100;
        QtSleep(100);
    }
    if(m_Connected)
    {
        if(m_FileTransState == Start)  //>@�����ʱ��������״̬ΪΪFailed
        {
            m_FileTransState = Timeout;
            ReplyFileStop(0);
        }
        //>@��ʾ��ǰ�����Զ�����״̬
        if(m_RetransFile)
        {
            m_WriteFileOp->unbind();
            goto __GetFile_;
        }
    }
    else
    {
        m_FileTransState = Stopped;
    }
    m_WriteFileOp->unbind();
    bool ret = (bool)(m_FileTransState == Success);
    //>@ֻҪ��֤�´�����SetCmdInfo��Quene�е�type��ͬ���ɡ�
    SetCmdInfo tmpInfo;
    tmpInfo.mType = SC_StartGetFile;
    ProcConfQueue(tmpInfo, ret);
    return ret;
}

//>@�����ļ���pSrc����pDirĿ¼��
bool PhysicalProtocol::GetDir(QString pSrc, QString pDir)
{
    if(!pSrc.endsWith("/"))
        pSrc.append("/");
    if(!pDir.endsWith("/"))
        pDir.append("/");
#if 0
    m_DownLoadTaskState = Start;
    QString tempDir;
    QString dir;
    QString name;
    QString src;
    QString dst;
    QStringList list = pSrc.split("/");
    if(list.count() < 2)
        return false;
    int i = 0;
    for(; i<list.count()-3; i++)
    {
        dir.append(list.at(i));
        dir.append("/");
    }
    dir.append(list.at(i));
    name = list.at(list.count()-2);
    //>@��������Ŀ¼
    if(!dir.endsWith("/"))
        dir.append("/");
    tempDir = QString("%1Output/").arg(dir);
    DelPath(tempDir, pTime, 0.04, SC_GetPath, false);
    MakePath(tempDir, pTime, 0.01, SC_GetPath, false);
    //>@ѹ���ļ��е���ǰ�ļ��е�outputĿ¼(ǰ���Ǳ�����ɾ��output�ļ��У�����Ὣ���ļ���Ҳѹ����ȥ)
    dst = QString("%1%2").arg(tempDir).arg(name);
    Compress(pSrc, dst, pTime, 0.60, SC_GetPath, false);
    //>@��ȡѹ������Ŀ��Ŀ¼
    src = QString("%1%2").arg(dst).arg(".zip");
    //>@BufferPath+name/+OUTPUTDIR+name.zip
    dst = QString("%1%2/%3/%2%4").arg(pDir).arg(name).arg(OUTPUTDIR).arg(".zip");
    GetFileTest("y", src, dst, pTime, 0.01, SC_GetPath, false);
    StartGetFile(pTime, 0.24, SC_GetPath, false);
    //>@�������Ŀ¼
    DelPath(tempDir ,pTime, 0.1, SC_GetPath, true);
    while(pTime>0 && (m_DownLoadTaskState==Start))
    {
        pTime -= 100;
        QtSleep(100);
    }
    if(m_DownLoadTaskState == Success)
    {
        //>@��ѹ��
        if(UnZip(dst, pDir, 600000, true))
        {
            IDE_TRACE();
            return true;
        }
    }
    emit sProgress(0);
#else
    //>@����ls�����ȡ�б�
    if(!SendConfFrame(tr("Ls#%1").arg(pSrc), 10000))
    {
        IDE_TRACE();
        return false;
    }

    emit sProgress(tr("Get Dir %1 ...").arg(pSrc), 0);
    int count = m_LsFileList.count();
    qreal diff = 100.0/count;
    for(int i=0;(i<m_LsFileList.count())&&m_Connected;i++)
    {
        QStringList tmpInfoList = m_LsFileList.at(i).split(':');
        if(tmpInfoList.isEmpty())
            continue;
        QString tmpFileName = tmpInfoList.at(0);
        emit sProgress(tr("Get %1 ...").arg(tmpFileName), i*diff+1);
        bool ret = GetFile("Y", pSrc+tmpFileName, pDir+tmpFileName);
        emit sProgress(tr("%1").arg(ret?D_Success:D_Failed), INVALIDRC);
        QtSleep(10);
    }
    if(m_Connected)
    {
        emit sProgress(tr("Get Dir %1 success!").arg(pSrc), 100);
        return true;
    }
#endif
    return false;
}

//>@��ĳ��·���ϴ���ĳ��Ŀ¼��
bool PhysicalProtocol::SendDir(QString pSrc, QString pDir)
{
    if(!pSrc.endsWith("/"))
        pSrc.append("/");
    if(!pDir.endsWith("/"))
        pDir.append("/");
#if 0
    m_UpLoadTaskState = Start;
    QString name = GetDirectoryName(pSrc);
    //>@����ɾ�����ļ��е�OUTPUTĿ¼����ѹ���ļ��е�Output��
    QString tmpSrcOutputDir = pSrc + OUTPUTDIR + QString("/");
    ClearFolder(tmpSrcOutputDir);
    QString tmpSrcFile = tmpSrcOutputDir + name + ".zip";
    if(!Zip(pSrc, tmpSrcFile, 600000, true))
    {
        IDE_TRACE();
        return false;
    }
    QFile tmpFile(tmpSrcFile);
    qint64 tmpSize = tmpFile.size();
    if(tmpSize)
    {
        int tmpTimeout = GetTransTimeout(tmpSize);
        //>@��������Ŀ¼
        QString tmpDstOutputDir = pDir + OUTPUTDIR + QString("/");
        DelPath(tmpDstOutputDir, tmpTimeout, 0.04, SC_SendPath, false);
        MakePath(tmpDstOutputDir, tmpTimeout, 0.01, SC_SendPath, false);
        QString tmpDstFile = tmpDstOutputDir + name + ".zip";
        //>@����ѹ����������Ŀ¼
        SendFileTest("y", tmpSrcFile, tmpDstFile, tmpTimeout, 0.05, SC_SendPath, false);
        //StartSendFile(tmpTimeout, 0.2, SC_SendPath, false);
        //>@ѹ������ѹ������Ŀ¼
        Decompress(tmpDstFile, pDir, tmpTimeout, 0.6, SC_SendPath, false);
        //>@�������Ŀ¼
        //DelFolder(tmpSrcOutputDir);
        DelPath(tmpDstOutputDir, tmpTimeout, 0.1, SC_SendPath, true);
        while(tmpTimeout>0 && (m_UpLoadTaskState==Start))
        {
            tmpTimeout -= 100;
            QtSleep(100);
        }
        if(m_UpLoadTaskState == Success)
            return true;
        emit sProgress(0);
    }
    else
    {
        IDE_DEBUG(tr("%1 is empty!").arg(tmpFile.fileName()));
    }
#else
    QStringList tmpList = getAllInFloders(true, pSrc, false);
    if(tmpList.isEmpty())
        return true;
    emit sProgress(tr("Send Dir %1 ...").arg(GetDirectoryName(pSrc)), 0);
    int count = tmpList.count();
    int tmpSrcLen = pSrc.count();
    qreal diff = 100.0/count;
    for(int i=0;(i<count)&&m_Connected;i++)
    {
        QString tmpSrcFile = tmpList.at(i);
        QString tmpDstFile = pDir + tmpSrcFile.mid(tmpSrcLen);
        QFileInfo tmpFileInfo(tmpSrcFile);
        if(tmpFileInfo.isFile())
        {
            QString tmpFileName = tmpFileInfo.fileName();
            emit sProgress(tr("Send %1 ...").arg(tmpFileName), i*diff+1);
            bool ret = SendFile("Y", tmpSrcFile, tmpDstFile);
            emit sProgress(tr("%1").arg(ret?D_Success:D_Failed), INVALIDRC);
            QtSleep(10);
        }
    }
    if(m_Connected)
    {
        emit sProgress(tr("Send Dir %1 success!").arg(GetDirectoryName(pSrc)), 100);
        return true;
    }
    else
    {
        emit sProgress(tr("Network is broken!").arg(GetDirectoryName(pSrc)), 100);
        return false;
    }
#endif
    return false;
}

PhysicalHost::PhysicalHost(QString pAddr, QString pBufferPath, QObject *parent) :
    PhysicalProtocol(parent)
{
    mDevState = S_OFF;
    mDevType = TYPE_LOCAL;
    ChangeDevAddr(pAddr);
    ChangeBuffer(pBufferPath);
    mIsBusy = false;
}

PhysicalHost::~PhysicalHost()
{
}

void PhysicalHost::SetDevCan(QTreeWidgetItem *pDevItem, DEVICESTATE pState)
{
    mDevCan.mItem = pDevItem;
    mDevCan.mType = TYPE_TREE;
    mDevState = pState;
}

void PhysicalHost::SetDevCan(QListWidgetItem *pDevItem, DEVICESTATE pState)
{
    mDevCan.mItem = pDevItem;
    mDevCan.mType = TYPE_LIST;
    mDevState = pState;
}

void PhysicalHost::SetDevCan(QTableWidgetItem *pDevItem, DEVICESTATE pState)
{
    mDevCan.mItem = pDevItem;
    mDevCan.mType = TYPE_TABLE;
    mDevState = pState;
}

bool PhysicalHost::isValid()
{
    if(mDevCan.mType == TYPE_TREE)
    {
        if(mDevCan.mItem)
            return true;
    }
    else if(mDevCan.mType == TYPE_LIST)
    {
        if(mDevCan.mItem)
            return true;
    }
    else if(mDevCan.mType == TYPE_TABLE)
    {
        if(mDevCan.mItem)
            return true;
    }
    return isSync();
}

bool PhysicalHost::Scan()
{
    if(!isValid())
        return false;
    //>@ɾ����ǰ�豸�б��е���Ŀ
    Clear();
    //>@��������Ŀ¼�еĿ��ù���
    QList<RC_CAN> tmpList;
    mIsBusy = true;
    tmpList = ScanDirectiory(mBufferPath);
    mIsBusy = false;
    return true;
}

QList<RC_CAN> PhysicalHost::ScanDirectiory(QString pPath)
{
    QList<RC_CAN> tmpList;
    if(!isValid() || pPath.isEmpty())
        return tmpList;
    if(!pPath.endsWith('/'))
        pPath.append('/');
    QDir dir(pPath);
    if(!dir.exists())
        return tmpList;
    QFileInfoList fileInfoList = dir.entryInfoList();
    QString tmpFindFilePath;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.isDir())
        {
            QString tmpFindDirPath = pPath + fileInfo.fileName();
            if(!tmpFindDirPath.endsWith('/'))
                tmpFindDirPath.append('/');
            QDir tmpDir(tmpFindDirPath);
            if(!dir.exists())
                continue;
            QFileInfoList findFileInfoList = tmpDir.entryInfoList(QDir::Files);
            foreach(QFileInfo findFileInfo, findFileInfoList)
            {
                if(findFileInfo.isFile())
                {
                    tmpFindFilePath = tmpFindDirPath + findFileInfo.fileName();
                    if(getFileType(tmpFindFilePath) == TYPE_PRO)
                    {
                        RC_CAN tmpRC;
                        tmpRC.mType = TYPE_LOCAL;
                        tmpRC.mPath = tmpFindFilePath;
                        tmpList.append(tmpRC);
                    }
                }
            }
        }
    }
    return tmpList;
}

void PhysicalHost::ChangeDevAddr(QString pDevAddr)
{
    //>@�鿴�Ƿ���ͬ
    if(mDevAddr.compare(pDevAddr) == 0)
        return;
    //>@ɾ������״̬��Ϣ
    mDevState = S_OFF;
    mIsBusy = false;
    mDevAddr = pDevAddr;
}

void PhysicalHost::ChangeBuffer(QString pBufferPath)
{
    mBufferPath = pBufferPath;
    if(!mBufferPath.isEmpty() && !mBufferPath.endsWith('/'))
        mBufferPath.append('/');
}

void PhysicalHost::Connected()
{
    mDevState = S_ON;
    PhysicalProtocol::Connected();
}

void PhysicalHost::Disconnected()
{
    mDevState = S_OFF;
    mIsBusy = false;
    PhysicalProtocol::Disconnected();
}

bool PhysicalHost::Sync(quint32 pTimeout)
{
    Sync();
    int tmpNum = pTimeout/10;
    int tmpCount = 0;
    while((tmpCount++<tmpNum) && !isSync())
        QtSleep(10);
    return isSync();
}

bool PhysicalHost::isSync(quint32 pTimeout)
{
    return isConnected();
}

bool PhysicalHost::Async(quint32 pTimeout)
{
    Async();
    int tmpNum = pTimeout/10;
    int tmpCount = 0;
    while((tmpCount++<tmpNum) && isSync())
        QtSleep(10);
    return !isSync();
}

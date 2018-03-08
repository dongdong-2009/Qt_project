#include "uiparser.h"
#include "globalfun.h"

const QString UiParser::UiSuffix = ".ui";
const QString UiParser::UiFileType = "BST UI File";

QRect GetRectFromString(QString pRectString)
{
    if(pRectString.isEmpty())
        return QRect();
    QStringList tmpList = pRectString.split(",");
    if(tmpList.count() != 4)
    {
        return QRect();
    }
    int tmpX = tmpList.at(0).toInt();
    int tmpY = tmpList.at(1).toInt();
    int tmpW = tmpList.at(2).toInt();
    int tmpH = tmpList.at(3).toInt();
    if(tmpW<=0 || tmpH<=0)
    {
        return QRect();
    }
    return QRect(tmpX, tmpY, tmpW, tmpH);
}

QString GetStringFromRect(QRect pRect)
{
    if(pRect.isEmpty())
        return QString();
    return QString("%1,%2,%3,%4").arg((int)pRect.left()).arg((int)pRect.top()).arg((int)pRect.width()).arg((int)pRect.height());
}

QRectF GetRotateRect(QSizeF pParentSize, QRectF pRect, ROTATE_ANGLE pAngle)
{
    if(pParentSize.isValid() == false || pRect.isValid() == false)
        return QRectF();
    qreal ParWidth = pParentSize.width();
    qreal ParHeight = pParentSize.height();
    QRectF tmpRect;
    int value;
    switch(pAngle)
    {
        case ROTATE_90: //>@��Ϊ˳ʱ����ת
        {
            //>@��ȡ���½Ƕ�������ڸ��������½Ƕ���ľ���
            value = ParHeight - pRect.top() - pRect.height();
            tmpRect.setLeft(value);
            value = pRect.left();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.height());
            tmpRect.setHeight(pRect.width());
            break;
        }
        case ROTATE_180:
        {
            //>@��ȡ���½Ƕ���Ϊ���
            value = ParHeight - pRect.top() - pRect.height();
            tmpRect.setLeft(value);
            value = ParWidth - pRect.left() - pRect.width();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.width());
            tmpRect.setHeight(pRect.height());
            break;
        }
        case ROTATE_270:
        {
            //>@��ȡ���ϽǶ���Ϊ���
            value = pRect.top();
            tmpRect.setLeft(value);
            value = ParWidth - pRect.left() - pRect.width();
            tmpRect.setTop(value);
            tmpRect.setWidth(pRect.height());
            tmpRect.setHeight(pRect.width());
            break;
        }
        default:
            return QRectF();
    }
    return tmpRect;
}

CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle)
{
    CONSTRAINT tmpConstraint;
    switch(pAngle)
    {
        case ROTATE_90: //>@��Ϊ˳ʱ����ת
        {
            tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
            tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
            break;
        }
        case ROTATE_180:
        {
            tmpConstraint.mDirection = (DIRECTION)(pConstraint.mDirection);
            tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
            break;
        }
        case ROTATE_270:
        {
            tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
            tmpConstraint.mOrientation = (ORIENTATION)(pConstraint.mOrientation);
            break;
        }
        default:
            return pConstraint;
    }
    return tmpConstraint;
}

UI_TYPE GetUiType(QString pUiName)
{
    UI_TYPE tmpComType = UI_INVALID;
    if (pUiName.isNull())
        return tmpComType;
    else if (pUiName.isEmpty() ||
             pUiName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             pUiName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_DESKTOP;
    }
    else if(pUiName.compare(QString("SetBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_SETBAR;
    }
    else if(pUiName.compare(QString("MessageBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_MESSAGEBAR;
    }
    else if(pUiName.compare(QString("ProgressBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_PROGRESSBAR;
    }
    else if(pUiName.compare(QString("SrnSaveForm"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_SRNSAVEFORM;
    }
    else if(!pUiName.compare(QString("Animal"), Qt::CaseInsensitive))
    {
        tmpComType = UI_ANIMAL;
    }
    else if(!pUiName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLOOR;
    }
    else if(!pUiName.compare(QString("DtnEle"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DTNELE;
    }
    else if(!pUiName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpComType = UI_ARROW;
    }
    else if(!pUiName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FUNCTION;
    }
    else if(!pUiName.compare(QString("FlrSign"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLRSIGN;
    }
    else if(!pUiName.compare(QString("RenCode"), Qt::CaseInsensitive))
    {
        tmpComType = UI_RENCODE;
    }
    else if(!pUiName.compare(QString("Media"), Qt::CaseInsensitive))
    {
        tmpComType = UI_MEDIA;
    }
    else if(!pUiName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpComType = UI_IMAGE;
    }
    else if(!pUiName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpComType = UI_WEBVIEW;
    }
    else if(!pUiName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpComType = UI_PHONE;
    }
    else if(!pUiName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TEXT;
    }
    else if(!pUiName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpComType = UI_LOGO;
    }
    else if(!pUiName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpComType = UI_WEATHER;
    }
    else if(!pUiName.compare(QString("Clock"), Qt::CaseInsensitive))
    {
        tmpComType = UI_CLOCK;
    }
    else if(!pUiName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpComType = UI_NAMEPLATE;
    }
    else if(!pUiName.compare(QString("FlrLCD"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLRLCD;
    }
    else if(!pUiName.compare(QString("FlrBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLRBTN;
    }
    else if(!pUiName.compare(QString("KeyboardBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_KEYBOARDBTN;
    }
    else if(!pUiName.compare(QString("CallBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_CALLBTN;
    }
    else if(!pUiName.compare(QString("Talkback"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TALKBACKBTN;
    }
    else if(!pUiName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_EMERGENCYBTN;
    }
    else if(!pUiName.compare(QString("TalkbackBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TALKBACKBTN;
    }
    else if(!pUiName.compare(QString("DoorBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DOORBTN;
    }
    else if(!pUiName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_SETBTN;
    }
    else if(!pUiName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DISABILITYBTN;
    }
    else if(!pUiName.compare(QString("Status"), Qt::CaseInsensitive))
    {
        tmpComType = UI_STATUS;
    }
    else if(pUiName.compare(QString("enum"), Qt::CaseInsensitive) == 0)
        return UI_ENUM;
    else if(pUiName.compare(QString("string"), Qt::CaseInsensitive) == 0)
        return UI_STRING;
    else if(pUiName.compare(QString("enetaddr"), Qt::CaseInsensitive) == 0)
        return UI_ENETADDR;
    else if(pUiName.compare(QString("num"), Qt::CaseInsensitive) == 0)
        return UI_NUM;
    else if(pUiName.compare(QString("check"), Qt::CaseInsensitive) == 0)
        return UI_CHECK;
    else if(pUiName.compare(QString("button"), Qt::CaseInsensitive) == 0)
        return UI_BUTTON;
    else if(pUiName.compare(QString("bool"), Qt::CaseInsensitive) == 0)
        return UI_BOOL;
    else if(pUiName.compare(QString("link"), Qt::CaseInsensitive) == 0)
        return UI_LINK;
    else if(pUiName.compare(QString("label"), Qt::CaseInsensitive) == 0)
        return UI_LABEL;
    return tmpComType;
}

QString GetUiName(UI_TYPE pUiType)
{
    switch(pUiType)
    {
        case UI_DESKTOP:
            return QString("Desktop");
        case UI_SETBAR:
            return QString("SetBar");
        case UI_MESSAGEBAR:
            return QString("MessageBar");
        case UI_PROGRESSBAR:
            return QString("ProgressBar");
        case UI_SRNSAVEFORM:
            return QString("SrnSaveForm");
        case UI_ANIMAL:
            return QString("Animal");
        case UI_FLOOR:
            return QString("Floor");
        case UI_DTNELE:
            return QString("DtnEle");
        case UI_ARROW:
            return QString("Arrow");
        case UI_FUNCTION:
            return QString("Function");
        case UI_RENCODE:
            return QString("RenCode");
        case UI_FLRSIGN:
            return QString("FlrSign");
        case UI_MEDIA:
            return QString("Media");
        case UI_IMAGE:
            return QString("Image");
        case UI_WEBVIEW:
            return QString("WebView");
        case UI_PHONE:
            return QString("Phone");
        case UI_TEXT:
            return QString("Text");
        case UI_LOGO:
            return QString("Logo");
        case UI_WEATHER:
            return QString("Weather");
        case UI_CLOCK:
            return QString("Clock");
        case UI_NAMEPLATE:
            return QString("NamePlate");
        case UI_FLRLCD:
            return QString("FlrLCD");
        case UI_FLRBTN:
            return QString("FlrBtn");
        case UI_KEYBOARDBTN:
            return QString("KeyboardBtn");
        case UI_CALLBTN:
            return QString("CallBtn");
        case UI_PHONEBTN:
            return QString("PhoneBtn");
        case UI_EMERGENCYBTN:
            return QString("EmergencyBtn");
        case UI_TALKBACKBTN:
            return QString("TalkbackBtn");
        case UI_DOORBTN:
            return QString("DoorBtn");
        case UI_SETBTN:
            return QString("SetBtn");
        case UI_DISABILITYBTN:
            return QString("DisabilityBtn");
        case UI_STATUS:
            return QString("Status");
        case UI_ENUM:
            return QString("enum");
        case UI_STRING:
            return QString("string");
        case UI_ENETADDR:
            return QString("enetaddr");
        case UI_NUM:
            return QString("num");
        case UI_CHECK:
            return QString("check");
        case UI_BUTTON:
            return QString("button");
        case UI_BOOL:
            return QString("bool");
        case UI_LINK:
            return QString("link");
        case UI_LABEL:
            return QString("label");
        default:
            return QString();
    }
    return QString();
}

QString GetUiPath(UI_TYPE pType)
{
    if(pType == UI_DESKTOP)
        return QString("/");
    QString tmpName = GetUiName(pType);
    if(!tmpName.isEmpty())
        return tmpName.prepend('/');
    return tmpName;
}

UI_TYPE GetParentUiType(UI_TYPE pType)
{
    if(pType > UI_AREA && pType < UI_COMPONENT)
    {
        return UI_DESKTOP;
    }
    return UI_INVALID;
}

UI_TYPE GetParentUiType(QString pUiPath)
{
    if(pUiPath.isEmpty())
        return UI_INVALID;
    int index1,index2;
    QString tmpString;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return UI_INVALID;
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0) //>@desktop and rect
    {
        tmpString = pUiPath.mid(index1);
        if(tmpString.length() == 1)
        {
            return UI_INVALID;
        }
        return UI_DESKTOP;
    }
    else
    {
        tmpString = pUiPath.mid(index1+1, index2-index1-1);
    }
    return GetUiType(tmpString);
}

QString GetParentUiPath(QString pUiPath)
{
    int index1,index2;
    QString tmpString;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return tmpString;
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = pUiPath.mid(index1);
        if(tmpString.length() == 1)
            return QString("");     //>@Empty��ʾ��ǰ�ؼ�ΪDesktop
        else
            return QString("/");
    }
    else
    {
        tmpString = pUiPath.mid(index1, index2-index1);
    }
    return tmpString;
}

QString GetCurUiName(QString pUiPath)
{
    if(pUiPath.isEmpty())
        return QString();
    int index1,index2;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        if(pUiPath.compare(QString("Desktop"), Qt::CaseInsensitive) == 0)
            return QString("Desktop");
        else
            return QString();
    }
    QString tmpString;
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = pUiPath.mid(index1+1);
        if(tmpString.isEmpty())
            return QString("Desktop");     //>@Empty��ʾDesktop
        else
            return tmpString;
    }
    else
    {
        tmpString = pUiPath.mid(index2+1);
    }
    return tmpString;
}

UI_TYPE GetCurUiType(QString pUiPath)
{
    QString tmpString;
    int index1,index2;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        return GetUiType(pUiPath);
    }
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = pUiPath.mid(index1+1);
        if(tmpString.isEmpty())
            return UI_DESKTOP;     //>@Empty��ʾDesktop
    }
    else
    {
        tmpString = pUiPath.mid(index2+1);
    }
    return GetUiType(tmpString);
}

/*
Para�������ͣ�
    Cache:0@true,false;
    Volume:80@0~100;
    Interval:2@1000,1500,2000,3000;
*/
QStringList PreprocPara(QString &pContent)
{
    QStringList tmpRetList;
    int index = pContent.indexOf('@');
    if(index > 0)
    {
        QString tmpValue = pContent.mid(0, index);

        tmpRetList = pContent.mid(index+1).split(D_ENUMSPLIT);
        if(!tmpRetList.isEmpty())
        {
            int tmpNum = tmpValue.toInt();
            if(tmpNum >= tmpRetList.count())
                tmpNum = tmpRetList.count() - 1;
            pContent = tmpRetList.at(tmpNum);
            tmpRetList.prepend(D_ENUMSPLIT);
        }

        tmpRetList = pContent.mid(index+1).split(D_RANGESPLIT);
        if(tmpRetList.count() == 2)
        {
            int tmpNum = tmpValue.toInt();
            int tmpMax = tmpRetList.at(1).toInt();
            int tmpMin = tmpRetList.at(0).toInt();
            if(tmpNum < tmpMin)
                tmpNum = tmpMin;
            else if(tmpNum > tmpMax)
                tmpNum = tmpMax;
            pContent = QString::number(tmpNum);
            tmpRetList.prepend(D_RANGESPLIT);
        }
    }
    return tmpRetList;
}

UiParaParser::UiParaParser(UiComParser *pComParser)
{
    mComParser = pComParser;
}

//xx:xx;
UiParaParser::UiParaParser(QString pString, UiComParser *pComParser)
{
    mComParser = pComParser;
    SetString(pString);
}

UiParaParser::~UiParaParser()
{
    mParaHash.clear();
}

void UiParaParser::SetString(QString pString)
{
    if(pString.isEmpty())
        return;
    QStringList tmpParaList = pString.split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParsePara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
}

QVariant UiParaParser::GetPara(int pName)
{
    QVariant tmpVar = mParaHash.value(pName);
    if(tmpVar.type() == QVariant::List)
    {
        QList<QVariant> tmpList1 = tmpVar.toList();
        int index = tmpList1.at(0).toInt();
        QList<QVariant> tmpList2 = tmpList1.at(1).toList();
        if(index < tmpList2.count())
            return tmpList2.at(index);
        else
            return QVariant();
    }
    return tmpVar;
}

void UiParaParser::SetPara(int pName, QVariant pValue)
{
    QVariant tmpVar = mParaHash.value(pName);
    if(tmpVar.isNull())
    {
        mParaHash.insert(pName, pValue);
    }
    else
    {
        if(tmpVar.type() == QVariant::List) //>@�����XX:1@xx,xx���͵ģ���pValue�����
        {
            QList<QVariant> tmpList1 = tmpVar.toList();

            QList<QVariant> tmpList;
            tmpList.append(pValue);
            tmpList.append(tmpList1.at(1));

            mParaHash.insert(pName, tmpList);
        }
        else
        {
            mParaHash.insert(pName, pValue);
        }
    }
}

QString UiParaParser::GetString(_MergePara pMergeParaFun)
{
    QString tmpParaString;
    QHash<int, QVariant>::const_iterator i = mParaHash.constBegin();
     while (i != mParaHash.constEnd())
     {
         int tmpKey = i.key();
         tmpParaString.append(pMergeParaFun(tmpKey, i.value()));
         tmpParaString.append(';');
         ++i;
     }
     return tmpParaString;
}

UiRectParser::UiRectParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mHVSame = pElement.hasAttribute("HV");
    if(mHVSame)
    {
        mRect[0] = GetRectFromString(pElement.attribute("HV"));
    }
    else
    {
        mRect[Vertical] = GetRectFromString(pElement.attribute("V"));
        mRect[Horizontal] = GetRectFromString(pElement.attribute("H"));
    }
    mElement = pElement;
}

QRect UiRectParser::GetRect(DIRECTION pDirection)
{
    if(pDirection < HVBoth)
        return mRect[pDirection];
    return QRect();
}

void UiRectParser::SetRect(DIRECTION pDirection, QRect pRect)
{
    if(pDirection < HVBoth)
    {
        mHVSame = false;
        mRect[pDirection] = pRect;
        mElement.setAttribute(GetDirectionString(pDirection), GetStringFromRect(pRect));
        //>@�鿴���������Ƿ���ڣ��������򴴽�
        //>@��ͬ���޸���һ����������ʱ��ֻ�޸Ŀ�߶ȣ����޸����Ͻ����ꣻ
        //>@����������²������꣬������Ͻ�������Ϊ0��������޸����꣬�򱣳����Ͻ����겻��
        DIRECTION tmpOppside;
        if(pDirection == Horizontal)
            tmpOppside = Vertical;
        else
            tmpOppside = Horizontal;
        mRect[tmpOppside] = QRect(mRect[tmpOppside].topLeft(), pRect.size());
        mElement.setAttribute(GetDirectionString(tmpOppside), GetStringFromRect(mRect[tmpOppside]));
    }
    else if(pDirection == HVBoth)
    {
        mHVSame = true;
        mRect[0] = pRect;
        mElement.removeAttribute("H");
        mElement.removeAttribute("V");
        mElement.setAttribute("HV", GetStringFromRect(pRect));
    }
}

UiRectListParser::UiRectListParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    QDomNodeList tmpRectList = pElement.childNodes();
    for(int i=0;i<tmpRectList.count();i++)
    {
        QDomElement tmpRectElement = tmpRectList.at(i).toElement();
        if(tmpRectElement.isNull())
            continue;
        QString tmpLayoutStr = tmpRectElement.tagName();
        if(!tmpLayoutStr.startsWith("Layout", Qt::CaseInsensitive))
            continue;
        int tmpLayout = tmpLayoutStr.mid(6).toInt();//layout
        UiRectParser *tmpRectParser = new UiRectParser(tmpRectElement, mComParser);
        mRects.insert(tmpLayout, tmpRectParser);
    }
    SetString(pElement.attribute(D_PARA));
    mElement = pElement;
}

UiRectListParser::~UiRectListParser()
{
    foreach(UiRectParser* tmpRectPair, mRects)
    {
        if(tmpRectPair)
            delete tmpRectPair;
    }
}

UiRectParser* UiRectListParser::GetRect(int pLayout)
{
    return mRects.value(pLayout);
}

QRect UiRectListParser::GetRect(int pLayout, DIRECTION pDirection)
{
    UiRectParser *tmpRectPair = GetRect(pLayout);
    if(!tmpRectPair)
        return QRect();
    if(pDirection < HVBoth)
        return tmpRectPair->mRect[pDirection];
    return QRect();
}

bool UiRectListParser::SetRect(int pLayout, DIRECTION pDirection, QRect pRect)
{
    UiRectParser *tmpRectPair = GetRect(pLayout);
    if(!tmpRectPair)
        return false;
    tmpRectPair->SetRect(pDirection, pRect);
    return true;
}

UiRectParser* UiRectListParser::AddRect(int pLayout, XmlParser* pParser)
{
    UiRectParser *tmpRectPair = GetRect(pLayout);
    if(!tmpRectPair)
    {
        QDomElement tmpRectElement = pParser->createElement(QString("Layout%1").arg(pLayout));
        //>@˳�����
        QList<int> tmpList = mRects.keys();
        qSort(tmpList.begin(), tmpList.end());
        QDomElement tmpAfter;
        foreach(int tmpLayout, tmpList)
        {
            if(pLayout < tmpLayout)
            {
                tmpAfter = mRects.value(tmpLayout)->mElement;
                break;
            }
        }
        mElement.insertBefore(tmpRectElement, tmpAfter);
        //
        tmpRectPair = new UiRectParser(tmpRectElement, mComParser);
        mRects.insert(pLayout, tmpRectPair);
    }
    return tmpRectPair;
}

void UiRectListParser::DelRect(int pLayout)
{
    UiRectParser *tmpRectPair = mRects.take(pLayout);
    if(tmpRectPair)
    {
        mElement.removeChild(tmpRectPair->mElement);
        delete tmpRectPair;
    }
}

void UiRcParser::UpdateHV(bool pSame)
{
    if(pSame)
    {
        mHVSame = true;
        mElement.removeAttribute("H");
        mElement.removeAttribute("V");
    }
    else
    {
        mHVSame = false;
        mElement.removeAttribute("HV");
    }
}

UiRcParser::UiRcParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mHVSame = pElement.hasAttribute("HV");
    if(!mHVSame)
    {
        mFile[Vertical] = pElement.attribute("V");
        mFile[Horizontal] = pElement.attribute("H");
        if(!mFile[Vertical].isEmpty() && !mFile[Vertical].compare(mFile[Horizontal]))
        {
            UpdateHV(true);
            mElement.setAttribute("HV", mFile[Vertical]);
        }
    }
    else
    {
        mFile[Vertical] = mFile[Horizontal] = pElement.attribute("HV");
    }
    if(!mFile[Vertical].isEmpty())
    {
        mFile[Vertical].prepend(pComParser->mUiParser->m_FileDir);
    }
    if(!mFile[Horizontal].isEmpty())
    {
        mFile[Horizontal].prepend(pComParser->mUiParser->m_FileDir);
    }
    if(mHVSame)
    {
        mType[Vertical] = mType[Horizontal] = GetRcType(mFile[Vertical]);
    }
    else
    {
        mType[Vertical] = GetRcType(mFile[Vertical]);
        mType[Horizontal] = GetRcType(mFile[Horizontal]);
    }
    //
    QString tmpMapStr = pElement.attribute(D_MAP);
    if(!tmpMapStr.isEmpty())
    {
        QStringList tmpMapList = tmpMapStr.split(';');
        foreach(QString tmpMap, tmpMapList)
        {
            QStringList tmpMapStrList = tmpMap.split(':');
            if(tmpMapStrList.count() != 2)
                continue;
            UI_TYPE tmpType = GetUiType(tmpMapStrList.at(0));
            int tmpKey;
            AREA_OPERATE tmpOperate;
            QStringList tmpMapParaList = tmpMapStrList.at(1).split('@');
            if(tmpMapParaList.count() == 1)
            {
                tmpKey = tmpMapParaList.at(0).toInt();
                tmpOperate = OPERATE_CHANGE;
            }
            else if(tmpMapParaList.count() == 2)
            {
                tmpKey = tmpMapParaList.at(0).toInt();
                tmpOperate = GetOperateType(tmpMapParaList.at(1));
            }
            else
                continue;
            UD_MAPITEM *tmpMapItem = new UD_MAPITEM;
            tmpMapItem->mKey = tmpKey;
            tmpMapItem->mOperate = tmpOperate;
            mMaps.insert(tmpType, tmpMapItem);
        }
    }
    QStringList tmpParaList = pElement.attribute(D_PARA).split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParseSubRcPara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
    mElement = pElement;
    mCurIndex = HVNone;
    mBuffer.mVAR = 0;
}

UiRcParser::~UiRcParser()
{
    Release();
    foreach(UD_MAPITEM *tmpItem, mMaps)
    {
        if(tmpItem)
            delete tmpItem;
    }
    mMaps.clear();
}

void UiRcParser::Release()
{
    if(mCurIndex >= HVBoth)
        return;
    if(mType[mCurIndex] == RC_INVALID)
        return;
    if(mType[mCurIndex] <= RC_PNG)
    {
        if(mBuffer.mIMG)
        {
            delete mBuffer.mIMG;
            mBuffer.mIMG = 0;
        }
    }
    else if(mType[mCurIndex] == RC_GIF)
    {
        if(mBuffer.mGIF)
        {
            mBuffer.mGIF->deleteLater();
            mBuffer.mGIF = 0;
        }
    }
#ifdef  SUPPORT_SVG
    else if(mType[mCurIndex] == RC_SVG)
    {
        if(mBuffer.mSVG)
        {
            mBuffer.mSVG->deleteLater();
            mBuffer.mSVG = 0;
        }
    }
#endif
#ifdef  SUPPORT_QUI
    else if(mType[mCurIndex] == RC_QUI)
    {
        if(mBuffer.mQUI)
        {
            mBuffer.mQUI->deleteLater();
            mBuffer.mQUI = 0;
        }
    }
#endif
}

void UiRcParser::SetRc(QString pDir, QString pFile, DIRECTION pDirection)
{
    if(!pFile.startsWith(pDir, Qt::CaseInsensitive))  //>@���������Դ
        return;
    if(!QFile::exists(pFile))
        return;
    QString tmpFile = pFile.mid(pDir.size());
    if(pDirection < HVBoth)
    {
        DIRECTION tmpOppside;
        if(pDirection == Horizontal)
            tmpOppside = Vertical;
        else
            tmpOppside = Horizontal;

        if(pFile.compare(mFile[tmpOppside], Qt::CaseInsensitive))
        {
            mHVSame = false;
            mFile[pDirection] = pFile;
            mElement.setAttribute(GetDirectionString(pDirection), tmpFile);
            mType[pDirection] = GetRcType(pFile);

            if(pDirection == mCurIndex)
                Release();
        }
        else
        {
            mFile[0] = pFile;
            mType[0] = GetRcType(pFile);
            UpdateHV(true);
            mElement.setAttribute("HV", tmpFile);

            if(0 == mCurIndex)
                Release();
        }
    }
    else if(pDirection == HVBoth)
    {
        mFile[0] = pFile;
        mType[0] = GetRcType(pFile);
        UpdateHV(true);
        mElement.setAttribute("HV", tmpFile);

        if(0 == mCurIndex)
            Release();
    }
}

bool UiRcParser::IsImage(DIRECTION pDirection)
{
    if(mType[pDirection] > RC_INVALID && mType[pDirection] < RC_GIF)
        return true;
    return false;
}

RC_TYPE UiRcParser::GetType()
{
    if(mCurIndex < HVBoth)
        return mType[mCurIndex];
    return RC_INVALID;
}

QImage* UiRcParser::GetImage(DIRECTION pDirection, QSize pSize)
{
    if(IsImage(pDirection) && GetRc(pDirection, pSize))
        return static_cast<QImage*>(mBuffer.mIMG);
    return (QImage*)0;
}

bool UiRcParser::GetRc(DIRECTION pDirection, QSize pSize)
{
    if(pDirection >= HVBoth)
        return false;
    if(mType[pDirection] == RC_INVALID)
        return false;
    if(pDirection == mCurIndex)
        return true;
    mCurIndex = pDirection;
    if(mType[pDirection] <= RC_PNG)
    {
        if(!mBuffer.mIMG)
            mBuffer.mIMG = new QImage();
        if(mBuffer.mIMG->load(mFile[pDirection]) == false)
            return false;
#ifdef RGB565  //>@��Ҫ�Ƕ�ͼ��������ص���
        if(ConvertImageTo565(&mBuffer.mIMG) == false)
            return false;
#endif
        return true;
    }
    else if(mType[mCurIndex] == RC_GIF)
    {
        if(!mBuffer.mGIF)
            mBuffer.mGIF = new QMovie();
        mBuffer.mGIF->setFileName(mFile[pDirection]);
        return true;
    }
#ifdef  SUPPORT_SVG
    else if(mType[mCurIndex] == RC_SVG)
    {
        if(!mBuffer.mSVG)
            mBuffer.mSVG = new QSvgRenderer();
        if(mBuffer.mSVG->load(mFile[pDirection]) == false)
            return false;
        return true;
    }
#endif
#ifdef  SUPPORT_QUI
    else if(mType[mCurIndex] == RC_QUI)
    {
        if(GlobalApp->m_UiLoader.isNull())
            return false;
        QFile tmpFormFile(mBuffer);
        if(!tmpFormFile.open(QFile::ReadOnly))
            return false;
        mBuffer.mQUI = GlobalApp->m_UiLoader->load(&tmpFormFile);
        tmpFormFile.close();
        return true;
    }
#endif
    return false;
}

UiRcListParser::UiRcListParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mCurKey = -1;
    mElement = pElement;
    QString tmpInitValue = pElement.attribute(D_INITVALUE);
    if(!tmpInitValue.isEmpty())
        mCurKey = tmpInitValue.toInt();
    mPrefix = pElement.attribute(D_PREFIX);
    if(mPrefix.isEmpty())
        mPrefix = D_DEFAULT_PREFIX;
    QDomNodeList tmpNodeList = pElement.childNodes();
    for(int i=0;i<tmpNodeList.count();i++)              //>@����ui����Ԫ��
    {
        QDomElement tmpElement = tmpNodeList.at(i).toElement();
        QString tmpName = tmpElement.tagName();
        int tmpKey = GetKey(tmpName);
        if(tmpKey < 0)
            continue;
        UiRcParser* tmpRcInfo = mRCs.take(tmpKey);
        if(tmpRcInfo)
            delete tmpRcInfo;
        mRCs.insert(tmpKey, new UiRcParser(tmpElement, mComParser));
    }
    QStringList tmpParaList = pElement.attribute(D_PARA).split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParseRcPara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
}

UiRcListParser::~UiRcListParser()
{
    mCurKey = -1;
    if(!mRCs.isEmpty())
    {
        foreach(UiRcParser* tmpRcInfo, mRCs)
        {
            if(tmpRcInfo)
                delete tmpRcInfo;
        }
        mRCs.clear();
    }
}

UiRcParser* UiRcListParser::NextRc()
{
    QList<int> tmpKeyList = mRCs.keys();
    if(tmpKeyList.count() > 0)
    {
        qSort(tmpKeyList.begin(), tmpKeyList.end());
        int index = tmpKeyList.indexOf(mCurKey);
        if(index < 0)
        {
            mCurKey = tmpKeyList.last();  //>@����Ҳ������ͷ������һ��
        }
        else
        {
            if((index+1) < tmpKeyList.count())
                mCurKey = tmpKeyList.at(index+1);
            else
                mCurKey = tmpKeyList.first();
        }
        return mRCs.value(mCurKey);
    }
    mCurKey = -1;
    return (UiRcParser *)0;
}

UiRcParser* UiRcListParser::PrevRc()
{
    QList<int> tmpKeyList = mRCs.keys();
    if(tmpKeyList.count() > 0)
    {
        qSort(tmpKeyList.begin(), tmpKeyList.end());
        int index = tmpKeyList.indexOf(mCurKey);
        if(index < 0)
        {
            mCurKey = tmpKeyList.first();    //>@����Ҳ������ͷ��ص�һ��
        }
        else
        {
            if((index-1) >= 0)
                mCurKey = tmpKeyList.at(index-1);
            else
                mCurKey = tmpKeyList.last();
        }
        return mRCs.value(mCurKey);
    }
    mCurKey = -1;
    return (UiRcParser *)0;
}

bool UiRcListParser::Contains(int pKey)
{
    return mRCs.contains(pKey);
}

UiRcParser* UiRcListParser::GetRc(int pKey)
{
    mCurKey = pKey;
    return mRCs.value(pKey);
}

void UiRcListParser::AddRc(int pKey, XmlParser* pParser)
{
    if(!mRCs.contains(pKey))
    {
        QDomElement tmpElement = pParser->createElement(QString("RC%1").arg(pKey));
        //>@˳�����
        QList<int> tmpList = mRCs.keys();
        qSort(tmpList.begin(), tmpList.end());
        QDomElement tmpAfter;
        foreach(int tmpKey, tmpList)
        {
            if(pKey < tmpKey)
            {
                tmpAfter = mRCs.value(tmpKey)->mElement;
                break;
            }
        }
        mElement.insertBefore(tmpElement, tmpAfter);
        //
        UiRcParser *tmpRcParser = new UiRcParser(tmpElement, mComParser);
        mRCs.insert(pKey, tmpRcParser);
    }
}

void UiRcListParser::AddRc(int pKey, UiRcParser* pRcInfo)
{
    if(pKey < 0 || !pRcInfo)
        return;
    UiRcParser* tmpRcInfo = mRCs.take(pKey);
    if(tmpRcInfo)
    {
        mElement.removeChild(tmpRcInfo->mElement);
        delete tmpRcInfo;
    }
    //>@˳�����
    QList<int> tmpList = mRCs.keys();
    qSort(tmpList.begin(), tmpList.end());
    QDomElement tmpAfter;
    foreach(int tmpKey, tmpList)
    {
        if(pKey < tmpKey)
        {
            tmpAfter = mRCs.value(tmpKey)->mElement;
            break;
        }
    }
    mElement.insertBefore(pRcInfo->mElement, tmpAfter);
    mRCs.insert(pKey, pRcInfo);
}

void UiRcListParser::DelRc(UiRcParser* pRcInfo)
{
    if(pRcInfo)
    {
        int tmpKey = mRCs.key(pRcInfo);
        if(mCurKey == tmpKey)
            mCurKey = INVALIDRC;
        mRCs.remove(tmpKey);
        mElement.removeChild(pRcInfo->mElement);
        delete pRcInfo;
    }
}

void UiRcListParser::DelRc(int pKey)
{
    if(mCurKey == pKey)
        mCurKey = INVALIDRC;
    UiRcParser *tmpRcInfo = mRCs.take(pKey);
    if(tmpRcInfo)
    {
        mElement.removeChild(tmpRcInfo->mElement);
        delete tmpRcInfo;
    }
}

UiOperateParser::UiOperateParser(EffectType pType, QDomElement pElement, UiComParser *pComParser) : QThread()
{
    mRunMode = QTimeLine::NotRunning;
    mComParser = pComParser;
    mAnimatePara = 0;
    Render(pComParser);
    //
    QStringList tmpParaList = pElement.attribute("Para").split(';');
    if(!tmpParaList.isEmpty())
    {
        mAnimatePara = new AREA_ANIMATE;
        mAnimatePara->mEffectType = pType;
        foreach(QString tmpPara, tmpParaList)
        {
            if(tmpPara.isEmpty())
                continue;
            QStringList tmpList = tmpPara.split(':');
            if(tmpList.count() != 2)
                continue;
            mComParser->ParseSubOperationPara(tmpList.at(0), tmpList.at(1), mAnimatePara);
        }
    }
}

UiOperateParser::~UiOperateParser()
{
    Stop();
    if(mAnimatePara)
        delete mAnimatePara;
}

void UiOperateParser::Start()
{
    if(isRunning())
        return;
    start(TimeCriticalPriority);
}

void UiOperateParser::Stop()
{
    if(isRunning())
    {
        //emit sStop();
        quit();
        wait(1000);
    }
}

void UiOperateParser::SetPause(bool pFlag)
{
    if(pFlag)
    {
        if(mRunMode == QTimeLine::Running)
        {
            mRunMode = QTimeLine::Paused;
            emit sSetPaused(pFlag);
        }
    }
    else
    {
        if(mRunMode == QTimeLine::Paused)
        {
            mRunMode = QTimeLine::Running;
            emit sSetPaused(pFlag);
        }
    }
}

void UiOperateParser::slot_FrameChanged(int pFrame)
{
    IDE_TRACE_INT(pFrame);
    foreach(UiComParser *tmpCom, mRenderComs)
    {
        if(tmpCom)
        {
            tmpCom->PaintBuffer(this, pFrame);
        }
    }
}

void UiOperateParser::Render(UiComParser *pRenderCom)
{
    if(!mRenderComs.contains(pRenderCom))
    {
        mRenderComs.append(pRenderCom);
    }
}

void UiOperateParser::UnRender(UiComParser *pRenderCom)
{
    if(pRenderCom)
    {
        mRenderComs.removeOne(pRenderCom);
    }
}

QTimeLine::State UiOperateParser::state()
{
    return mRunMode;
}

void UiOperateParser::run()
{
    if(mAnimatePara)
    {
        QTimeLine tmpAnimator;
        connect(&tmpAnimator, SIGNAL(frameChanged(int)), this, SLOT(slot_FrameChanged(int)), Qt::DirectConnection);
        connect(this, SIGNAL(sStop()), &tmpAnimator, SLOT(stop()), Qt::QueuedConnection);
        connect(this, SIGNAL(sSetPaused(bool)), &tmpAnimator, SLOT(setPaused(bool)), Qt::QueuedConnection);
        connect(&tmpAnimator, SIGNAL(finished()), this, SLOT(quit()), Qt::QueuedConnection);
        tmpAnimator.setFrameRange(mAnimatePara->mFrameStart, mAnimatePara->mFrameEnd);
        mAnimatePara->mFrameCount = mAnimatePara->mFrameEnd - mAnimatePara->mFrameStart;
        tmpAnimator.setDuration(mAnimatePara->mAnimateTime);
        tmpAnimator.setCurveShape(mAnimatePara->mAnimateType);
        tmpAnimator.setLoopCount(mAnimatePara->mLoopCount);
        tmpAnimator.start();
        mRunMode = QTimeLine::Running;
        exec();
        emit sFinished();
    }
    mRunMode = QTimeLine::NotRunning;
}

void UiOperateParser::slot_Finished()  //�����ô�
{
    IDE_TRACE();
    slot_FrameChanged(mAnimatePara->mFrameEnd);
    if(isRunning())
    {
        //emit sStop();
        quit();
    }
}

void UiOperateParser::SetPara(UiOperateParser::ParaType pType, int pPara)
{
    switch(pType)
    {
        case E_DIRECTION:
        {
            mAnimatePara->mDirection = (DIRECTION)pPara;
            break;
        }
        case E_ORIENTATION:
        {
            mAnimatePara->mOrientation = (ORIENTATION)pPara;
            break;
        }
        case E_LOOP:
        {
            mAnimatePara->mLoopCount = pPara;
            break;
        }
        case E_FRASTART:
        {
            mAnimatePara->mFrameStart = pPara;
            mAnimatePara->mFrameCount = mAnimatePara->mFrameEnd - mAnimatePara->mFrameStart -1;
            break;
        }
        case E_FRAEND:
        {
            mAnimatePara->mFrameEnd = pPara;
            mAnimatePara->mFrameCount = mAnimatePara->mFrameEnd - mAnimatePara->mFrameStart -1;
            break;
        }
        case E_TIME:
        {
            mAnimatePara->mAnimateTime = pPara;
            break;
        }
        case E_DUTYRATIO:
        {
            mAnimatePara->mDutyRatio = pPara;
            break;
        }
        case E_CURVETYPE:
        {
            mAnimatePara->mAnimateType = (QTimeLine::CurveShape)pPara;
            break;
        }
        default:
            break;
    }
}

int  UiOperateParser::GetPara(UiOperateParser::ParaType pType)
{
    switch(pType)
    {
        case E_DIRECTION:
        {
            return mAnimatePara->mDirection;
        }
        case E_ORIENTATION:
        {
            return mAnimatePara->mOrientation;
        }
        case E_LOOP:
        {
            return mAnimatePara->mLoopCount;
        }
        case E_FRASTART:
        {
            return mAnimatePara->mFrameStart;
        }
        case E_FRAEND:
        {
            return mAnimatePara->mFrameEnd;
        }
        case E_TIME:
        {
            return mAnimatePara->mAnimateTime;
        }
        case E_DUTYRATIO:
        {
            return mAnimatePara->mDutyRatio;
        }
        case E_CURVETYPE:
        {
            return mAnimatePara->mAnimateType;
        }
        default:
            break;
    }
    return -1;
}

UiOperateListParser::UiOperateListParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mElement = pElement;
    QDomNodeList tmpNodeList = pElement.childNodes();
    for(int i=0;i<tmpNodeList.count();i++)
    {
        QDomElement tmpElement = tmpNodeList.at(i).toElement();
        QString tmpName = tmpElement.tagName();
        AREA_OPERATE tmpOperate = GetOperateType(tmpName);
        if(tmpOperate == OPERATE_NONE)
            continue;
        EffectType tmpType = GetEffectType(tmpElement.attribute("Type"));
        if(tmpType == EffectTypeNone)
            continue;
        UiOperateParser* tmpInfo = mOperations.take(tmpOperate);
        if(tmpInfo)
            delete tmpInfo;
        mOperations.insert(tmpOperate, new UiOperateParser(tmpType, tmpElement, mComParser));
    }
    QStringList tmpParaList = pElement.attribute(D_PARA).split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParseOperationPara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
}

UiOperateListParser::~UiOperateListParser()
{
    foreach(UiOperateParser* tmpInfo, mOperations)
    {
        if(tmpInfo)
            delete tmpInfo;
    }
    mOperations.clear();
}

UiOperateParser* UiOperateListParser::GetOperate(AREA_OPERATE pKey)
{
    return mOperations.value(pKey);
}

AREA_ANIMATE* UiOperateListParser::GetOperatePara(AREA_OPERATE pKey)
{
    UiOperateParser *tmpInfo = mOperations.value(pKey);
    if(tmpInfo)
        return tmpInfo->mAnimatePara;
    return (AREA_ANIMATE*)0;
}

UiOperateParser * UiOperateListParser::AddOperate(AREA_OPERATE pKey, XmlParser* pParser)
{
    UiOperateParser *tmpParser = GetOperate(pKey);
    if(!tmpParser)
    {
        QDomElement tmpElement = pParser->createElement(GetOperateString(pKey));
        EffectType tmpType = EffectTypeNone;
        if(tmpElement.isNull() || ((tmpType = GetEffectType(tmpElement.attribute("Type"))) == EffectTypeNone))
        {
            return (UiOperateParser *)0;
        }
        //>@˳�����
        QList<AREA_OPERATE> tmpList = mOperations.keys();
        qSort(tmpList.begin(), tmpList.end());
        QDomElement tmpAfter;
        foreach(AREA_OPERATE tmpKey, tmpList)
        {
            if(pKey < tmpKey)
            {
                tmpAfter = mOperations.value(tmpKey)->mElement;
                break;
            }
        }
        mElement.insertBefore(tmpElement, tmpAfter);
        //
        tmpParser = new UiOperateParser(tmpType, tmpElement, mComParser);
        mOperations.insert(pKey, tmpParser);
    }
    return tmpParser;
}

void UiOperateListParser::AddOperate(AREA_OPERATE pKey, UiOperateParser* pInfo)
{
    if(pKey <= OPERATE_NONE || !pInfo)
        return;
    UiOperateParser* tmpInfo = mOperations.take(pKey);
    if(tmpInfo)
    {
        mElement.removeChild(tmpInfo->mElement);
        delete tmpInfo;
    }
    //>@˳�����
    QList<AREA_OPERATE> tmpList = mOperations.keys();
    qSort(tmpList.begin(), tmpList.end());
    QDomElement tmpAfter;
    foreach(AREA_OPERATE tmpKey, tmpList)
    {
        if(pKey < tmpKey)
        {
            tmpAfter = mOperations.value(tmpKey)->mElement;
            break;
        }
    }
    mElement.insertBefore(pInfo->mElement, tmpAfter);
    mOperations.insert(pKey, pInfo);
}

void UiOperateListParser::DelOperate(AREA_OPERATE pKey)
{
    UiOperateParser *tmpInfo = mOperations.take(pKey);
    if(tmpInfo)
    {
        mElement.removeChild(tmpInfo->mElement);
        delete tmpInfo;
    }
}

UiComParser::UiComParser() : UiParaParser(this)
{
    mUiParser = 0;
    Clear(false);

    mLayout = 0;
    mStyle = 0;
}

UiComParser::~UiComParser()
{
    Clear(!mExternPara);
}

void UiComParser::Clear(bool pSync)
{
    if(pSync)
    {
        if(mComRect)
        {
            delete mComRect;
            mComRect = 0;
        }
        if(mComResource)
        {
            delete mComResource;
            mComResource = 0;
        }
        if(mComOperation)
        {
            delete mComOperation;
            mComOperation = 0;
        }
    }
    else
    {
        mComRect = 0;
        mComResource = 0;
        mComOperation = 0;
    }
}

bool UiComParser::Init(QDomElement pElement, UiParser *pParser)
{
    mUiParser = pParser;
    if(pElement.isNull())
        return false;
    mElement = pElement;
    mLayout = pElement.attribute("Layout").toInt();
    mStyle = pElement.attribute("Style").toInt();
    //>@Rect
    QDomElement tmpRectElement = pElement.firstChildElement(QString("Rect"));
    if(!tmpRectElement.isNull())
    {
        if(!mExternPara && mComRect)
            delete mComRect;
        mComRect = new UiRectListParser(tmpRectElement, this);
    }
    //>@Resource
    QDomElement tmpResourceElement = pElement.firstChildElement(QString("Resource"));
    if(!tmpResourceElement.isNull())
    {
        if(!mExternPara && mComResource)
            delete mComResource;
        mComResource = new UiRcListParser(tmpResourceElement, this);
    }
    //>@Operation
    QDomElement tmpOperateElement = pElement.firstChildElement(QString("Operation"));
    if(!tmpOperateElement.isNull())
    {
        if(!mExternPara && mComOperation)
            delete mComOperation;
        mComOperation = new UiOperateListParser(tmpOperateElement, this);
    }
    //>@SubUI
    QDomElement tmpSubUiElement = pElement.firstChildElement(QString("SubUI"));
    if(!tmpSubUiElement.isNull())
    {
        QDomNodeList tmpList = tmpSubUiElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            InitSubUI(tmpElement, pParser);
        }
    }
    return true;
}

bool UiComParser::InitSubUI(QDomElement pElement, UiParser *pParser)
{

}

bool UiComParser::ParseSubOperationPara(QString pLabel, QString pContent, AREA_ANIMATE* pAreaEffect)
{
    if(pAreaEffect == 0)
        return false;

    PreprocPara(pContent);

    if(pLabel.compare(PARA_FRAMERANGE, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10����
        if(ok)
        {
            pAreaEffect->mFrameStart = 0;
            pAreaEffect->mFrameEnd = dec;
        }
    }
    else if(pLabel.compare(PARA_INTERVAL, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10����
        if(ok)
            pAreaEffect->mAnimateTime = dec;
    }
    else if(pLabel.compare(PARA_DUTYRATIO, Qt::CaseInsensitive) == 0)  //>@0~100
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10����
        if(ok)
            pAreaEffect->mDutyRatio = dec;
    }
    else if(pLabel.compare(PARA_CURVESHAPE, Qt::CaseInsensitive) == 0)
    {
        pAreaEffect->mAnimateType = GetTimelineType(pContent);
    }
    else if(pLabel.compare(PARA_DIRECTION, Qt::CaseInsensitive) == 0)
    {
        if(pContent.compare(QString("H"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mDirection = Horizontal;
        else if(pContent.compare(QString("V"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mDirection = Vertical;
    }
    else if(pLabel.compare(PARA_ORIENTATION, Qt::CaseInsensitive) == 0)
    {
        if(pContent.compare(QString("P"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mOrientation = Positive;
        else if(pContent.compare(QString("N"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mOrientation = Negative;
    }
    return true;
}


UiParser::UiParser(QString fileName) :
    XmlParser(fileName)
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

bool UiParser::NewUI()
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

    tmpElement = createElement("Ui");
        QDomElement tmpChildElement = createElement("SubUI");
        tmpElement.appendChild(tmpChildElement);
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();

    GetUiGroup();
    return true;
}

bool UiParser::OpenUI()
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
    GetUiGroup();
    return true;
}

bool UiParser::CloseUI()
{
    SaveUI();
    return true;
}

bool UiParser::DelUI()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool UiParser::SaveUI()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    return Save();
}

bool UiParser::RenameUI(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(UiSuffix))
        tmpString.append(UiSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@����������ɹ������޸�m_FileNameΪ���������ֵ��
    return flag;
}

bool UiParser::isValid()
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

int UiParser::GetLayout()
{
    if(!m_Valid)
        return D_TYPE_USER_DEFINE;
    bool flag = GetItemElement(QString("/Layout"));
    if(flag)
    {
        QString tmpNum = itemElement.text();
        if(tmpNum.isEmpty())
            return D_TYPE_USER_DEFINE;
        return tmpNum.toInt();
    }
    return D_TYPE_USER_DEFINE;
}

bool UiParser::SetLayout(int pLayout)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Layout", QString::number(pLayout));
}

int UiParser::GetStyle()
{
    if(!m_Valid)
        return D_TYPE_USER_DEFINE;
    bool flag = GetItemElement(QString("/Style"));
    if(flag)
    {
        QString tmpNum = itemElement.text();
        if(tmpNum.isEmpty())
            return D_TYPE_USER_DEFINE;
        return tmpNum.toInt();
    }
    return D_TYPE_USER_DEFINE;
}

bool UiParser::SetStyle(int pStyle)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Style", QString::number(pStyle));
}

//>@���ڸ���Global���е�m_ComElementGroup
void UiParser::GetUiGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return;
    }

    mLayout = GetLayout();
    mStyle = GetStyle();

    bool flag = GetItemElement("/Ui");
    if(flag)
    {
        m_ComElementGroup.clear();

        QDomElement tmpElement;
        QDomNodeList UiNodeList = itemElement.childNodes();
        for(int i=0;i<UiNodeList.count();i++)
        {
            tmpElement = UiNodeList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            UI_TYPE tmpComType = GetUiType(tmpString);
            if(tmpComType < UI_DESKTOP)
                continue;
            m_ComElementGroup.insert(tmpComType, tmpElement);//>@����uiԪ��

            QString tmpSubUIString = QString("/Ui/%1/SubUI").arg(tmpString);
            flag = GetItemElement(tmpSubUIString);
            if(flag)
            {
                QDomNodeList SubUINodeList = itemElement.childNodes();
                for(int i=0;i<SubUINodeList.count();i++)              //>@����ui����Ԫ��
                {
                    tmpElement = SubUINodeList.at(i).toElement();
                    QString tmpSubString = tmpElement.tagName();
                    UI_TYPE tmpSubComType = GetUiType(tmpSubString);
                    if(tmpSubComType < UI_DESKTOP)
                        continue;
                    m_ComElementGroup.insert(tmpSubComType, tmpElement);

                    QString tmpSubSubUIString = QString("/Ui/%1/SubUI/%2/SubUI").arg(tmpString).arg(tmpSubString);
                    flag = GetItemElement(tmpSubSubUIString);
                    if(flag)
                    {
                        QDomNodeList SubSubUINodeList = itemElement.childNodes();
                        for(int i=0;i<SubSubUINodeList.count();i++)              //>@����subui����Ԫ��
                        {
                            tmpElement = SubSubUINodeList.at(i).toElement();
                            QString tmpSubSubString = tmpElement.tagName();
                            UI_TYPE tmpSubSubComType = GetUiType(tmpSubSubString);
                            if(tmpSubSubComType < UI_DESKTOP)
                                continue;
                            m_ComElementGroup.insert(tmpSubSubComType, tmpElement);
                        }
                    }
                }
            }
        }
    }
}

QUiParser::QUiParser(QString pFileName) :
    XmlParser(pFileName)
{
    m_Valid = Open();
    if(m_Valid)
        m_Valid = GetItemElement("/widget/widget");
    if(!m_Valid)
    {
        IDE_TRACE();
        return;
    }

    GetUiGroup(itemElement);
}

void QUiParser::GetUiGroup(QDomElement pElement)
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
            //if(tmpClass.startsWith("Widget"))
            {
                UI_TYPE tmpComType = GetUiType(tmpElement.attribute("name"));
                if(tmpComType < UI_FLOOR)
                    continue;
                m_ComElementGroup.insert(tmpComType, tmpElement);
            }
//            else if(tmpClass.compare("QScrollArea") ==0)
//            {
//                GetpUiGroup(tmpElement);
//            }
        }
    }
}

QRect QUiParser::GetRect(UI_TYPE pType)
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

void QUiParser::ModifyRect(UI_TYPE pType, QRect pRect)
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



#include "uifloor.h"
#include "define.h"
#include "mainwindow.h"

UiFloor::UiFloor(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    mHundred = 0;
    mTen = 0;
    mSingle = 0;
}

void UiFloor::Init(QDomElement pElement)
{
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    this->setGeometry(tmpRect);

    mHundLabel = this->findChild<QLabel *>("Hundred");
    if(mHundLabel)
        mHundLabel->setScaledContents(true);
    mTenLabel = this->findChild<QLabel *>("Ten");
    if(mTenLabel)
        mTenLabel->setScaledContents(true);
    mSingleLabel = this->findChild<QLabel *>("Single");
    if(mSingleLabel)
        mSingleLabel->setScaledContents(true);

    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomNodeList tmpList = tmpRcElement.childNodes();
    for(int i=0; i<tmpList.count(); i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpTagName = tmpElement.tagName();
        int tmpNum = tmpTagName.mid(2).toInt();
        mRcHash.insert(tmpNum, tmpThemePath + tmpElement.text());
    }

    updateFloor();//确保更改图片后，能立即刷新到当前的显示内容上
}

void UiFloor::updateFloor()
{
//    if(mHundred == 48)
//    {
//        mHundLabel->setPixmap(QPixmap());
//    }
//    else
//    {
//        mHundLabel->setPixmap(mRcHash.value(mHundred));
//    }

    if(mTen == 48)
    {
        mTenLabel->setPixmap(mRcHash.value(mSingle));
        mSingleLabel->setPixmap(QPixmap());
    }
    else
    {
        mTenLabel->setPixmap(mRcHash.value(mTen));
        mSingleLabel->setPixmap(mRcHash.value(mSingle));
    }


    //mSingleLabel->setStyleSheet("font:bold larger\"Arial\"; font-size:100px;color:white");
}

void UiFloor::setFloor(int pGe, int pShi, int pBai)
{
    mHundred = pBai+48;
    mTen = pShi+48;
    mSingle = pGe+48;

    updateFloor();
}

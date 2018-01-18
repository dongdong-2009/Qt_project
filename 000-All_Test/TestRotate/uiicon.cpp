#include "uiicon.h"
#include "mainwindow.h"

UiIcon::UiIcon(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    mIconNum = -1;
    mIcon = 0;

    mTextLine1 = 0;
    mTextLine2 = 0;

    mBigTextLine1 = 0;
    mBigTextLine2 = 0;
    mBigTextLine3 = 0;
    mBigTextLine4 = 0;
    tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());

}

void UiIcon::Init(QDomElement pElement)
{
    mDomElement = pElement;

    mIcon = this->findChild<QLabel *>("label_image");
    mTextLine1 = this->findChild<QLabel *>("label_text_1");
    mTextLine2 = this->findChild<QLabel *>("label_text_2");
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(tmpStackedWidget)
    {
        QWidget *tmpBigPage = tmpStackedWidget->widget(1);
        mBigTextLine1 = tmpBigPage->findChild<QLabel *>("label_bigtext_1");
        mBigTextLine2 = tmpBigPage->findChild<QLabel *>("label_bigtext_2");
        mBigTextLine3 = tmpBigPage->findChild<QLabel *>("label_bigtext_3");
        mBigTextLine4 = tmpBigPage->findChild<QLabel *>("label_bigtext_4");
    }

    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    IDE_TRACE_INT(mTextLine1->height());
    IDE_TRACE_INT(mTextLine2->height());
    IDE_TRACE_INT(tmpRect.height());
    tmpRect.setHeight(tmpRect.height() + mTextLine1->height() + mTextLine1->height() + 12);
    this->setGeometry(tmpRect);

    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomNodeList tmpList = tmpRcElement.childNodes();
    for(int i=0;i<tmpList.count();i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpTagName = tmpElement.tagName();
        IDE_TRACE_STR(tmpTagName);
        int tmpNum = tmpTagName.mid(2).toInt();
        IDE_TRACE_INT(tmpNum);
        IDE_TRACE_STR(tmpThemePath + tmpElement.text());
        mRcHash.insert(tmpNum, tmpThemePath + tmpElement.text());
    }
    mTextLine1->setAlignment(Qt::AlignCenter);
    mTextLine2->setAlignment(Qt::AlignCenter);
    updateIcon(); //ȷ������ͼƬ��������ˢ�µ���ǰ����ʾ������
}

void UiIcon::updateIcon()
{
    mIcon->setPixmap(mRcHash.value(mIconNum));

    //�����С���ı�������Ҫͬ����ʾ����ͼ���Ӧ���ı�
//    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(tmpStackedWidget && tmpStackedWidget->currentIndex() == 0)
    {
        QDomElement tmpRcElement = mDomElement.firstChildElement("resource");
        QDomElement tmpElement =tmpRcElement.firstChildElement(QString("rc%1").arg(mIconNum));
        if(!tmpElement.isNull())
        {
            QString tmpText = tmpElement.attribute("text");
            QStringList tmpList = tmpText.split(";", QString::SkipEmptyParts);  //���Կո�
            if(tmpList.count() >= 1)
            {
                mTextLine1->setText(tmpList.at(0));
            }
            if(tmpList.count() >= 2)
            {
                mTextLine2->setText(tmpList.at(1));
            }
        }
    }
}

void UiIcon::setIcon(int pIcon)
{
    mIconNum = pIcon;
    updateIcon();
}

void UiIcon::setText(QString pText1, QString pText2, QString pText3, QString pText4)
{
    mTextLine1->setText(pText1);
    mTextLine2->setText(pText2);

    mBigTextLine1->setText(pText1);
    mBigTextLine2->setText(pText2);
    mBigTextLine3->setText(pText3);
    mBigTextLine4->setText(pText4);
}

void UiIcon::setBigSrnMode(bool pFlag)
{
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
        return;
    if(pFlag)
    {
        tmpStackedWidget->setCurrentIndex(1);
    }
    else
    {
        tmpStackedWidget->setCurrentIndex(0);
    }
}

void UiIcon::setBlackGround(int index)
{
    IDE_TRACE_INT(index);
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
    {
        return;
    }
    if (index >2)
    {
        index = 2;
    }
    tmpStackedWidget->setCurrentIndex(index);
}


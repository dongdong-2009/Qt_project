#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "define.h"

class UiClass
{
public:
    virtual void SetFlag(quint32 pFlag, QVariant pPara = QVariant()) { m_Flag.insert(pFlag, pPara); }
public:
    QHash<quint32, QVariant>   m_Flag;
};

class FlickCharmPrivate;
class FlickCharm: public QObject
{
    Q_OBJECT
public:
    FlickCharm(QObject *parent = 0);
    ~FlickCharm();
    void activateOn(QWidget *widget, int rate = 1);
    void deactivateFrom(QWidget *widget);
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void sFinished();

protected:
    void timerEvent(QTimerEvent *event);

private:
    FlickCharmPrivate *d;
};

#define D_ITEM_LINE_LEN     1

class ItemScrollContent : public QWidget
{
    Q_OBJECT
public:
    ItemScrollContent(QWidget *parent = 0);
    ~ItemScrollContent();

    void Highlight(QPainter *p, QString pString, QRect pRect, bool pEnable);
    void paintEvent(QPaintEvent *);

    void Init(QStringList pList, int pIndex);
    void SetCurrentIndex(int pIndex, bool pSync = true);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void Resize(QSize pSize);

signals:
    void sClicked(int pIndex);  //>@���͸�ItemScrollArea���ź�

public slots:
    void slot_Clicked(int pIndex);  //ItemScrollArea�������źŲ��

public:
    int                  mIndex;
    QStringList          mStringList;
    QList<QRect>         mRectList;

    int                  mItemHeight;
    QImage               mBuffer;  //һ������
    QPixmap              mBuffer2;

    QFont                mFont[2];
    QColor               mColor[2];  //>@������ɫ��0��ʾѡ�еģ�1��ʾδѡ�е�
    //�Ի��Ʊ���
    QColor               mBgColor[2];
    QColor               mFrameColor;
    //ʹ��ͼƬ��Ϊ����
    QPixmap              mBgImg[2];

    //ʹ��ͼƬ��Ϊÿ����ǩͼƬ
    QPixmap              mImage[2];

    QPoint               mPressPos;
    QPoint               mReleasePos;

    bool                 mButtonMode;
};

class ItemScrollArea : public QScrollArea, public UiClass
{
    Q_OBJECT
public:
    ItemScrollArea(QWidget *parent = 0);
    ~ItemScrollArea();

    enum{
        FLAG_SELECTOR,      //>@��ѡģʽ��Ĭ����ѡһ������ȴ���������������Ϻ���ܺ�����
        FLAG_ITEMBUTTON,    //>@��ťģʽ����ģʽ�£����Ե��ĳ��item��ʹ֮����������Item������
        FLAG_ITEMHEIGHT,
        //
        FLAG_FOCUS_FONTTYPE,
        FLAG_UNFOCUS_FONTTYPE,
        //
        FLAG_FOCUS_FONTSIZE,
        FLAG_UNFOCUS_FONTSIZE,
        //
        FLAG_RECT_COLOR,    //>@item�߿���ɫ
        //
        FLAG_FOCUS_COLOR,
        FLAG_UNFOCUS_COLOR,
        //
        FLAG_FOCUS_BGCOLOR,
        FLAG_UNFOCUS_BGCOLOR,
        //
        FLAG_FOCUS_BGIMG,
        FLAG_UNFOCUS_BGIMG
    };

    void Init();                                //ִ����new���������ִ��һ��
    virtual void Init(QStringList pList, int pIndex);   //�������е�setflag������ִ��һ��
    void SetFlag(quint32 pFlag, QVariant pPara = QVariant());

    QString GetCurrentText();
    int GetCurrentIndex();
    void SetCurrentIndex(int pIndex);  //>@��������ĳһ��Ϊ����������������

    void resizeEvent(QResizeEvent *e);

signals:
    void sClicked(int pIndex); //���͸�ItemScrollContent���ź�
    void sItemClicked(int);

public slots:
    void slot_Clicked(int pIndex);  //ItemScrollContent�������źŲ��
    void slot_ScrollFinished();

public:
    ItemScrollContent    *mWidget;
    FlickCharm           mFlickCharm;

    bool                 mSelectorMode;
};

class TextTerminal : public QWidget, public UiClass
{
    Q_OBJECT
public:
    TextTerminal(QWidget *parent = 0);
    ~TextTerminal();

    enum{
        FLAG_TITLE_EN = 0,
        FLAG_FONT_SIZE,
        FLAG_FONT_FAMILY,
        FLAG_FONT_WEIGHT,
        FLAG_INFO_COLOR,
        FLAG_WARN_COLOR,
        FLAG_ERROR_COLOR
    };

    struct TEXT_INFO{
        E_MSG_LEVEL  mLevel;
        QString      mText;
        int          mCount;
    };

    void SetFlag(quint32 pFlag, QVariant pPara = QVariant());

    void Append(E_MSG_LEVEL pLevel, QString pText);
    void Replace(int pIndex, QString pText);  //>@�޸����һ�е��ַ���

    void Refresh();

    void UpdatePara(QSize pSize);
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

public:
    QList<TEXT_INFO*>   mTextList;
    int                 mMaxCount;
    int                 mMaxRows;
    int                 mDiffHeight;
    int                 mItemHeight;

    QFont               mFont;
    Qt::GlobalColor     mColor[3];
    QRect               mTextRect;

    QImage              mImage;
    QImage              mBuffer;

    //����ͼƬ
    QBrush       m_BgBrush, m_FgBrush,
                 m_BGlowBrush,                      //>@�ײ�����
                 m_TGlassBrush, m_TGlassBrush2,     //>@��������ĥɰЧ��
                 m_LightBrush;      //>@ѡ�и���Ч��
    QColor       m_BgPenColor, m_FgPenColor;
    QRect        m_BgRect, m_FgRect,
                 m_BGlowRect,      //>@�ײ�����
                 m_TGlassRect,     //>@��������ĥɰЧ��
                 m_LightRect;      //>@ѡ�и���Ч��
    QString      m_Title;         //����
    QFont        m_TitleFont;
    QColor       m_TitleColor;
    bool         m_ShowTitle;
};

#endif // GUI_H

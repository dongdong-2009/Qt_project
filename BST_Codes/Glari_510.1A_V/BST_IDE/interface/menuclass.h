#ifndef MENUBASE_H
#define MENUBASE_H

#include "global.h"
#include "ilistview.h"

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(QWidget *parent = 0);
    ~MenuItem();

    virtual void SetComponent(int pType, QWidget *pWidget);
    virtual bool IsChanged();

    virtual QVariant GetValue();
    virtual bool SetValue(QVariant pValue);

    void SetFocus(bool pFlag);

    void paintEvent(QPaintEvent *e);

public:
    bool                    mIsChanged;
    QHash<int,QWidget*>     mComponents; //int��ʾitem�ж����ʾֵ�����������
    int                     mType;      //MenuItem������

private:
    QBrush                  mBgBrush;
    QBrush                  mLineBrush;
    bool                    mFocus;
};

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = 0);
    ~MenuWidget();

    virtual void Initialize();

    bool IsChanged();
    virtual bool IsChanged(int &pType);

    virtual QSize ItemSize(int pIndex);
    bool AddItem(MenuItem *pItem);

    int ItemCount();
    int CurrentIndex();
    MenuItem* CurrentItem();
    QVariant CurrentValue();

    bool eventFilter(QObject *object, QEvent *event);

public slots:
    void slot_Init();

public:
    QList<MenuItem*>    mItemWidgets;
    int                 mIndex;
    bool                mValidPressed;
};

class MenuScroll : public QScrollArea
{
    Q_OBJECT
public:
    explicit MenuScroll(QWidget *parent = 0);
    ~MenuScroll();

    MenuWidget* Widget();

    void resizeEvent(QResizeEvent *e);

    void MoveDown(int pDiff = 0);
    void MoveUp(int pDiff = 0);

public:
    FlickCharm           mFlickCharm;
};

#endif // MENUBASE_H

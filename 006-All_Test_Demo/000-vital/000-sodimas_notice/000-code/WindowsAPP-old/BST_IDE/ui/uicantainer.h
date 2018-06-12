#ifndef UICANTAINER_H
#define UICANTAINER_H

#include "global.h"

typedef enum{
    UI_IDEL = 0,
    UI_LOADING,
    UI_ERROR
}UI_STATUS;

class UiCantainerBase
{
public:
    UiCantainerBase()
    {
        m_InitShow = true;
        m_Status = UI_IDEL;
        m_ExtraClass = false;  //>@
        m_UiManager = 0;
        m_UiDirection = Horizontal;
        m_OriginRotate = 0;
        m_UiLayout = 0;
    }
    ~UiCantainerBase()
    {
        Release();
    }

    //>@�Ƿ�ʹ���ⲿ��ui��Դ
    void UseExtra(bool pEnable) {m_ExtraClass = pEnable;}

    //>@����UI
    virtual bool LoadUI(QString pFileName);

    virtual bool Initialize(uiManager *pManager);
    virtual bool Initialize(QString pFileName);
    virtual void Release();

    DIRECTION GetDirection(int pRotate);

    int OriginRotate();  //>@��ȡԭʼ��Ļ����ת�Ƕȣ���Ҫ�����LINUX_QWS�豸
    virtual bool Rotate(int pNewRotate) {return false;}

    void InitLayout(int pLayout) {m_UiLayout = pLayout;}  //>@�ڽ����ʼ��ǰ��ʹ��InitLayout����Layout
    virtual void ModifyLayout(int pLayout);               //>@�ڽ����ʼ����ʹ��ModifyLayout����Layout
    int GetLayout() {return m_UiLayout;}

    virtual bool UpdateUI() {return false;}  //>@��ʾ�������µ�����ˢ�½���

    //>@���溯��
    virtual bool InitUI();      //>@��ʼ��UI�ļ�
    virtual bool ReleaseUI(){return false;}   //>@�ͷ�UI�ļ�
    void SaveUI() {if(m_UiManager) m_UiManager->SaveUI();}
    virtual bool OperateUI(AREA_OPERATE pOperate, QList<COM_TYPE> pExceptNodes) {return false;}

    virtual void SetGeometory(QRectF pRect) {}
    virtual QRectF GetGeometory() {return QRectF();}

public:
    int                  m_Status;
    bool                 m_ExtraClass;   //>@ָʾ�Ƿ�Ϊ�ⲿ�������Դ�ࣨVIEW��manager��
    int                  m_OriginRotate;
    DIRECTION            m_UiDirection;
    int                  m_UiLayout;   //>@
    uiManager           *m_UiManager;

    bool                 m_InitShow;  //>@ָʾLoadUI���Ƿ���Ҫ��ʾ����
};

#endif // UICANTAINER_H

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "uicantainer.h"
#include "graphicsoperate.h"

class GraphicsScene : public QGraphicsScene, public UiCantainerBase
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = 0);
    ~GraphicsScene();

    bool UpdateUI();
    bool ReleaseUI(){ DeleteAllCom(); return true; }

    virtual bool AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos);
    bool DelComponent(GraphicsOperate *&pComPtr);
    bool SelectComponent(COM_TYPE pComType);
    bool SelectComponent(GraphicsOperate *pComPtr);

    void DeleteAllCom();  //>@�ͷ�Scene�е��������

    virtual void showFrame(GraphicsOperate *pComItem) {}
    virtual void hideFrame(bool pDelFlag = false) {}
    virtual void updateFrame() {}

public:
    GraphicsOperate     *m_SelectComponet;   //>@��ǰѡ�еĿؼ�����
    QHash<COM_TYPE, GraphicsOperate*> m_ComDiagramGroup;   //>@xx.uiҳ���е������subui��control����GraphicsBaseָ��
};

#endif // GRAPHICSSCENE_H

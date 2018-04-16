#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent), UiCantainerBase()
{
    m_SelectComponet = 0;
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::DeleteAllCom()
{
    hideFrame(true);
    //>@ɾ��scene�µ�����item
    QList<COM_TYPE> tmpComList = m_ComDiagramGroup.keys();
    for(int i=tmpComList.count()-1; i>=0; i--)
    {
        GraphicsOperate* tmpComItem = m_ComDiagramGroup.take(tmpComList.at(i));
        if(tmpComItem)
        {
            IDE_TRACE_STR(tmpComItem->m_ComPath);
            removeItem(tmpComItem);
            tmpComItem->deleteLater();
        }
    }
    m_ComDiagramGroup.clear();
}

bool GraphicsScene::AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos)
{
    return true;
}

//>@ɾ��ĳ���ؼ�ʱ��Ҫ��
//>@  1���Ͽ����丸�����ӡ�
//>@  2����Scene/m_UiManager/m_ComDiagramGroup��ɾ���Լ��Լ��ӽڵ�
bool GraphicsScene::DelComponent(GraphicsOperate *&pComPtr)
{
    if(!pComPtr)
        return true;
    if(m_SelectComponet == pComPtr)
        hideFrame();
    QList<QGraphicsItem *> tmpChildItems = pComPtr->childItems();
    for(int i=tmpChildItems.count()-1;i>=0;i--)
    {
        GraphicsOperate *tmpChildItem = (GraphicsOperate *)tmpChildItems.at(i);
        if(!tmpChildItem)
            continue;
        DelComponent(tmpChildItem);
    }
    COM_TYPE tmpType = (COM_TYPE)(pComPtr->data(COMTYPE).toInt());
    m_ComDiagramGroup.remove(tmpType);
    removeItem(pComPtr);
    pComPtr->DeleteComponent();
    pComPtr->deleteLater();
    pComPtr = 0;
    return true;
}

bool GraphicsScene::SelectComponent(COM_TYPE pComType)
{
    GraphicsOperate *tmpComItem = m_ComDiagramGroup.value(pComType);
    return SelectComponent(tmpComItem);
}

bool GraphicsScene::SelectComponent(GraphicsOperate *pComPtr)
{
    if(!pComPtr)
        return false;
    if(pComPtr == m_SelectComponet)
        return true;
    //>@�������ѡ��ؼ�����֮ǰ��ѡ��Ŀؼ���Ҫֹͣ��Ч������
    if(m_SelectComponet)
        m_SelectComponet->StopAnimate();
    m_SelectComponet = pComPtr; //>@���浱ǰѡ�еĿؼ�
    m_SelectComponet->RefreshParameter();
    updateFrame();  //>@��Frame����Ϊ��ǰCOMINFO�Ŀ�
    return true;
}

bool GraphicsScene::UpdateUI()
{
    QList<COM_TYPE> tmpList = m_ComDiagramGroup.keys();
    COM_TYPE tmpType;
    foreach(tmpType, tmpList)
    {
        if(tmpType == COM_DESKTOP)
            continue;
        GraphicsOperate* tmpCom = m_ComDiagramGroup.value(tmpType);
        if(!tmpCom)
            continue;
        QRect tmpHRect, tmpVRect;
        if(!m_UiManager->GetComRect(m_UiLayout, tmpCom->m_ComElement, tmpHRect, tmpVRect))
        {
            IDE_TRACE_INT(tmpType);
            continue;
        }
        tmpCom->m_ComRect[Horizontal] = tmpHRect;
        tmpCom->m_ComRect[Vertical] = tmpVRect;
        if(m_UiDirection == Horizontal)
            tmpCom->ModifyGeomotery(tmpCom->m_ComRect[Horizontal]);
        else
            tmpCom->ModifyGeomotery(tmpCom->m_ComRect[Vertical]);
    }
}

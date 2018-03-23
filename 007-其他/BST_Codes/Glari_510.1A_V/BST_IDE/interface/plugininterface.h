#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include "global.h"
#include <QVariant>
#include <QtPlugin>
#include <QAction>
#include <QWidget>

class MainForm;
class LogicThemeManager;
class EventMap;

typedef void(*EmitReport)(LogicThemeManager* pThemeManager, AREA_OPERATE pOperate, QVariant pPara);  //����ָ�����Ͷ���

class PluginInterface
{
public:
    virtual ~PluginInterface() {}
    virtual QString Name() = 0;// �������
    virtual QString RcPath() = 0;// �����ʾ��������е�ͼ���ļ�·��
    virtual QList<QAction*>* Actions() = 0;// ����ṩ�Ķ�������ӿڼ�
    virtual MainForm* Widget() = 0;// �����������

    virtual void InitUI(EventMap *pEventMap, QString pRcPath = QString()) = 0;

    //>@���ûص�����,���������Qt��������ĺ���ָ�������������ô���ͨѶ����
    virtual void SetReportCallback(LogicThemeManager *pThemeManager, EmitReport pFunc) = 0;

    virtual void ReportOperate(AREA_OPERATE pOperate, QVariant pPara) = 0;  //>@�������򱨸���״̬�����ô˺�������ִ�лص�����
    virtual void ExecOperate(AREA_OPERATE pOperate, QVariant pPara) = 0;   //>@�������������ʵ�ֶ���

public:
    EmitReport              m_EmitReport;   //�ź��ͷź�����ָ��, ����ָ��ص�����
    LogicThemeManager      *m_ThemeManager;   //Qt����ʵ����ָ�룬ָ��Qt�������е���ʵ�������ص�������ʵ�Σ��Ա���Qt�������н����ź��ͷ�
};

Q_DECLARE_INTERFACE(PluginInterface, "PluginInterface");

#endif // BASEPLUGIN_H

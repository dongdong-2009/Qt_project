#ifndef MIDDLEWAREBASE_H
#define MIDDLEWAREBASE_H

#include "devfun.h"

/*
    1�����ݲ����������ýű��ļ�
    2�����ݲ����������ݿ��е��ļ���
    3���ṩ���õ�Ӧ�ó����б�
*/

class MiddlewareDevice : public IODeviceBase
{
    Q_OBJECT
public:
    MiddlewareDevice(QObject *parent = 0);
    ~MiddlewareDevice();

    bool InitDevice(MW_Type pType, QDomElement pElement);
    QString GetSystemPath()  {return D_FSRCPATH+D_SYSTEMDIR+D_MIDDLEWAREDIR+mDevName+QString("/");}
    virtual bool DownloadLibrary();  //>@���ؿ�
    virtual bool GenerateScript();   //>@������������ļ�

    //>@��ȡ���ڵ��е���Ҫ��Ϣ
    virtual QString GetInformation() { return QString("%1").arg(mDevName); }
};

#endif // MIDDLEWAREBASE_H

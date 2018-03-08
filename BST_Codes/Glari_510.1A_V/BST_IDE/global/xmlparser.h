#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "define.h"
#include <QtXml>
#include "filefunc.h"

class DomDocument : public QDomDocument
{
public:
    QString m_FileDir;
    QString m_FileName;
};

class XmlParser : public QObject, public DomDocument
{
    Q_OBJECT
public:
    XmlParser();
    XmlParser(QString fileName);
    ~XmlParser();

    void RemoveAttribute(QDomElement pElement, QString pName);
    void SetAttribute(QDomElement pElement, QString pName, QString pValue);

    //>@����·������
    bool GetItemElement(QString itemPath);
    //>@��Ԫ��itemDomElementΪ��·�����в���
    bool GetItemElement(QDomElement itemDomElement, QString itemPath);

    //>@ģ������
    bool SearchItemElement(QString itemName);

    //>@��ȡԪ��ֵ ��̬
    QString GetItemElementValue();
    QString GetItemElementValue(QString itemPath);

    //>@��������
    static QList<QDomAttr> GetItemElementAttrs(QDomElement itemDomElement);
    QList<QDomAttr> GetItemElementAttrs();
    QList<QDomAttr> GetItemElementAttrs(QString itemPath);

    //����Ԫ�ؽڵ�
    bool CreateItemElement(QString itemPath);
    bool CreateItemElement(QDomElement pParElement, QString pName, QString pValue);

    //>@����ĳһ��Ԫ��ΪXML�ı�
    bool SaveItemElement(QDomElement itemDomElement, QString& pContent);     //>@���浽pContent��
    bool SaveItemElement(QString &pContent);
    bool SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice);   //>@���浽IODevice��
    bool SaveItemElement(QIODevice *pDevice);

    bool ModifyItemElement(QString itemPath, QString value);
    bool ModifyItemElement(QDomElement itemDomElement, QString value);
    bool SetText(QDomElement itemDomElement, QString value);

    bool ClearItemElement(QDomElement itemDomElement);
    bool DeleteItemElement(QDomElement itemDomElement);

    //��ȡ��ǰԪ�����е��ӽڵ�
    QDomNodeList GetChildNodes()
    {
        return itemElement.childNodes();
    }

    bool write(QIODevice *device);
    bool read(QIODevice *device);
    bool Open();
    bool New();
    bool Save();
    bool SaveAs(QString fileName);
    bool Rename(QString fileName);

public:
    static const QString XmlHead;
    static const QString XmlSuffix;

    QString errorStr;
    int errorLine;
    int errorColumn;

    bool m_Valid;

    bool m_IsChanged;
    bool m_ForceSave;

    void SetForcesave(bool pEnable) {m_ForceSave = pEnable;}

    QDomElement itemElement;
};

#endif // XMLPARSER_H

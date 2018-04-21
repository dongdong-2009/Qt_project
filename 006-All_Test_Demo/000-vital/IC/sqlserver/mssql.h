#ifndef MSSQL_H
#define MSSQL_H

#include "sqlbasic.h"

//>@=========================�ؼ���============================================
#define D_DeviceID                              GBKToUnicode("�豸")
#define D_MainBoardID                           GBKToUnicode("����")
#define D_InterfaceBoardID                      GBKToUnicode("�ӿڰ�")
#define D_OrderID                               GBKToUnicode("������")

#define D_ID                                    GBKToUnicode("ID")
#define D_SubmitTime                            GBKToUnicode("�ύʱ��")
#define D_BarCode                               GBKToUnicode("��ά��")
#define D_Describe                              GBKToUnicode("����")
#define D_FileType                              GBKToUnicode("�ĵ�����")
#define D_StoreNetDisk                          GBKToUnicode("�浵����")
//>@=========================��Ʒ��============================================
#define D_OrderTb                               GBKToUnicode("��Ʒ�б�")
    #define D_OrderTb_SystemType                    GBKToUnicode("ϵͳ����")
//>@=========================�豸��============================================
#define D_DeviceTb                              GBKToUnicode("�豸�б�")
#define D_MainBoardTb                           GBKToUnicode("�����б�")
#define D_InterfaceBoardTb                      GBKToUnicode("�ӿڰ��б�")

    #define D_DeviceTb_MainBoardBarcode             GBKToUnicode("�����ά��")
    #define D_DeviceTb_InterfaceBoardBarcode        GBKToUnicode("�ӿڰ��ά��")
//>@==========================��־��===========================================
#define D_DeviceLogTb                           GBKToUnicode("�豸��־")
#define D_MainBoardLogTb                        GBKToUnicode("������־")
#define D_InterfaceBoardLogTb                   GBKToUnicode("�ӿڰ���־")
//>@==========================���===========================================
#define D_MainBoardTypeTb                       GBKToUnicode("�����ͺ�")
#define D_InterfaceBoardTypeTb                  GBKToUnicode("�ӿڰ��ͺ�")
    #define D_BoardType                             GBKToUnicode("�ͺ�")
    #define D_WholesaleType                         GBKToUnicode("������")
//>@==========================�ļ���===========================================
#define D_FileTypeTb                            D_FileType

#define D_MainBoardFileTb                       GBKToUnicode("�����ļ�")
#define D_InterfaceBoardFileTb                  GBKToUnicode("�ӿڰ��ļ�")
    #define D_BoardFile_FileName              GBKToUnicode("����")
    #define D_BoardFile_Version               GBKToUnicode("�汾")
    #define D_BoardFile_HardwareType          GBKToUnicode("�����ͺ�")
    #define D_BoardFile_SrcFile               GBKToUnicode("Դ�ļ�")
    #define D_BoardFile_Author                GBKToUnicode("�����")
//>@==========================����===========================================
#define D_UITb                                  GBKToUnicode("����")
#define D_UIRecordTb                            GBKToUnicode("�����¼")
//>@=====================================================================
#define D_NetDiskAdrTb                          GBKToUnicode("���̵�ַ")
    #define D_NetDiskAdrTb_Name                     GBKToUnicode("����")
    #define D_NetDiskAdrTb_Adr                      GBKToUnicode("��ַ")
//>@==========================�û���==========================================
#define D_RegisteredUserTb                     GBKToUnicode("ע���û�")
#define D_UserPermissionTb                     GBKToUnicode("ע��Ȩ���б�")
    #define D_UserName                          GBKToUnicode("����")
    #define D_UserPermisson                     GBKToUnicode("Ȩ��")
    #define D_UserCode                          GBKToUnicode("���")
    #define D_UserPasswd                        GBKToUnicode("����")
    #define D_UserAddress                       GBKToUnicode("��ַ")
    #define D_UserPhone                         GBKToUnicode("��ϵ��ʽ")
//>@=====================================================================
//>@Key word
#define D_Tb_ID                     GBKToUTF8("ID")
#define D_Keyword_ID                GBKToUTF8("ID")
#define D_Keyword_FileName          GBKToUTF8("�ļ���")
#define D_Keyword_FileType          GBKToUTF8("�ļ���")

#define D_Keyword_SrcFile           GBKToUTF8("Դ�ļ�")
#define D_Keyword_HardwareType      GBKToUTF8("�����ͺ�")
#define D_Keyword_SaveDir           GBKToUTF8("�鵵·��")
#define D_Keyword_Time              GBKToUTF8("�ύʱ��")

//>@Key word--FileType
#define D_SystemImage               GBKToUTF8("ϵͳ����")
#define D_App                       GBKToUTF8("Ӧ�ó���")
#define D_BurnFile                  GBKToUTF8("��¼����")
#define D_BurnDoc                   GBKToUTF8("��¼�ĵ�")
#define D_AssemblyDoc               GBKToUTF8("װ���ĵ�")
#define D_TestDoc                   GBKToUTF8("�����ĵ�")
#define D_NetDisk0                  GBKToUTF8("�����ĵ�")
#define D_NetDisk1                  GBKToUTF8("�����ĵ�")
//>@//>@Key word--scanPoint
#define D_EnterOrderID              GBKToUTF8("�����붩����")
#define D_ScanMainBoardID           GBKToUTF8("��ɨ�������ά��")
#define D_ScanInterfaceBoardID      GBKToUTF8("��ɨ��ӿڰ��ά��")
#define D_EnterOrderID              GBKToUTF8("�����붩����")
#define D_EnterOrderID              GBKToUTF8("�����붩����")

class mssql : public SQLBasic
{
    Q_OBJECT
public:
    explicit mssql(QObject *parent = 0);
    ~mssql();

    bool init();

    //>@�ύĳһ�У����û��ָ������ֵ�������Ҳ�������ֵ��������һ��
    bool SubmitTbRow(QString tbName, QHash<QString, QString>fieldVauleHash, QString pkVaule = QString());
    bool DelTbRow(QString tbName, QString pkVaule);

    //>@�������б���
    QStringList GetAllTbs();
    //>@��ȡĳ������ĳ���ֶεĳ���
    int GetFieldLenth(QString tbName, QString fieldName);
    //>@��ȡ��������
    QStringList GetTbPks(QString tbName);
    //>@��ȡ��һ������
    QString GetTbPk(QString tbName);
    //>@��ȡΨһ�����ڼ���Ψһ����1,2,3����Ψһ���ֶΡ�
    QMultiHash<QString, QString> GetTbUks0(QString tbName);
    //>@��ȡ��һ��Ψһ��
    QStringList GetTbUks1(QString tbName);
    //>@��ȡΨһ����ֵ<����ֵ(1,2,3)������Ψһ���ֶε�ֵ����һ�����Ǳ��еڶ����ֶε�ֵ��>  pNotes�Ƿ��ڵõ���ֵǰ���[�ֶ���]
    QMultiHash<QString, QString> GetTbUksVaules(QString tbName, bool pNotes=true);
    //>@��ȡΨһ�������û�з��صڶ��еģ����ֶ���
    QString GetTbIkey(QString tbName);
    //>@��ȡ�����ֶ�
    QStringList GetTbFields(QString tbName);
    //>@��ȡ�ǿ��ֶ�
    QStringList GetTbUnNullFields(QString tbName);

    //>@��ȡ�������е����ֵ
    QString GetTbMaxPkVaule(QString tbName);
    //>@��ȡ���и����ֶ�(���)����(���)������
    QHash<QString, QString> GetTbParTb(QString tbName);
    //>@��ȡ���н��Լ���ĳ���ֶ���Ϊ����ı���()
    QHash<QString, QString> GetTbCldTbs(QString tbName);
    //>@
    QString GetTbCldTb(QString tbName, QString field);
    //>@��ȡ�����
    void GetTbAllCldTbs(QString tbName, QStringList &ret);
    //>@��ȡһ�ű��ѯ����һ�ű��·����Դ��#���#�ӱ�->Դ��#���#�ӱ�->Դ��#���#Ŀ���
    bool GetTbCldTbLinks(QString srcTb, QString dstTb, QStringList &ret);
    //>@��ȡ�����͵�һ�е�ֵ������ֵ����һ��ֵ��
    QHash<QString, QString> GetTbPkIk(QString tbName);

    //>@����ĳ�����еĵ�һ��Ψһ�����²�ṹ��������-����:Ψһ���б�,�ָ#���->�����Ӧ���ӱ�
    void GetTbFieldsLinks(QString tbName, QStringList fieldList, QStringList &retList, int level=-1);
    //>@��ȡĳ���ֶε�����Ҷ�ӽڵ�·��������-����:Ψһ���б�Ҳ�����ϸ�������ǰ��һ����
    QStringList GetTbFieldLinks(QString tbName, QString field);
    //>@��ȡĳ���ֶε�Ҷ�����ֵ�������Ӧ����ϼ��е��ֶ������ֶ�ֵ��
    QHash<QString, QString> GetTbFieldVaules(QString tbName, QString field);

    //>@
    QSqlQuery ScanTb0(QString tbName, QStringList retFieldList, QString orderField = QString());
    //>@
    QSqlQuery ScanTb1(QString tbName, QString pkVaule, QStringList retFieldList, QString orderField = QString());
    //>@
    QSqlQuery ScanTb2(QString tbName, QStringList srcFieldList, QStringList srcVauleList, QStringList retFieldList, QString orderField = QString());
    //>@
    QSqlQuery ScanTb3(QString tbName, QStringList srcFieldList, QList<QStringList> srcVauleslist, QStringList retFieldList, QString orderField = QString());

    //>@Advance fun

    //>@ͨ�������ҵ������Լ��ӿڰ���ͺ�  ����ֵΪID�����ò���ΪID��Ӧ�ĵڶ���ֵ�� ��һ��Ϊ������ͺţ��ڶ���Ϊ�ӿڰ���ͺţ�
    QStringList GetBoardTypes(QString pCodeID, QString &pMainBoardType, QString &pInterfaceBoardType);
    //>@ͨ����ά����Ҷ���
    QString GetOrder(QString pCodeType, QString pCodeID);
    //>@ͨ������ͺŲ��Ҷ�Ӧ���ļ�
    QSqlQuery GetMainBoardFiles(QString pBoardTypeLink);
    QSqlQuery GetInterfaceBoardFiles(QString pBoardTypeLink);

    //>@��ȡĳ�ű���ĳһ�ж�Ӧ���ļ�������·��
    QString getNetDiskAdr(QString tbName, QString pkVaule);
    QString GetMainBoardFileDir0(QString fileType, QString hardwareType) {return QString();}
    QString GetMainBoardFileDir1(QString fileID) {return QString();}
    QString GetInterfaceFileDir0(QString fileType, QString hardwareType) {return QString();}
    QString GetInterfaceFileDir1(QString fileID) {return QString();}

    QStringList GetAllID(QString tbName);
    QString DeviceIDToOrderID(QString deviceID);
    QString MainBoardIDToOrderID(QString mainBoardID);
    QString InterfaceBoardIDToOrderID(QString interfaceBoardID);

    bool AddRecordToMainBoardTb(QString orderID, QString barcode);
    bool AddRecordToMainBoardLogTb(QString barcode, QString action);

    bool AddRecordToInterfaceBoardTb(QString orderID, QString barcode);
    bool AddRecordToInterfaceBoardLogTb(QString barcode, QString action);

    //>@���һ����Ʒ��¼
    bool AddRecordToDeviceTb(QString barcode, QString mainBoardbarcode, QString interfaceBoardbarcode);
    bool AddRecordToDeviceLogTb(QString barcode, QString action);
};

#endif // MSSQL_H

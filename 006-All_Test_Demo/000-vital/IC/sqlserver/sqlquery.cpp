#include "sqlquery.h"

SqlQuery::SqlQuery(QObject *parent) :
    QObject(parent)
{
    m_SqlQueryModel = 0;
    m_FileProcess = 0;
}

SqlQuery::~SqlQuery()
{
    if(m_FileProcess)
    {
        m_FileProcess->kill();
        m_FileProcess->deleteLater();
    }
    Disconnect();
}

bool SqlQuery::Connect(QString pDatabaseIpAdr, QString pFileserverIpAdr, qint16 pPort, QString pDatabaseName, QString pUsrName, QString pPasswd)
{
    //GlobalPara.updateProgress(QString("Connecting SQL Server ..."),20);//������
    if(OpenDatabase(pDatabaseName, pDatabaseIpAdr, pUsrName, pPasswd) == false)
    {
        //GlobalPara.updateProgress(QString("SQL Server Connect failed!"),0);//������
        QMessageBox::warning(0, tr("Error"),
                             m_LastErrorString,
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return false;
    }
    //GlobalPara.updateProgress(QString("SQL Server Connected!"),40);//������

    m_FileServerAddr = pFileserverIpAdr;
    m_FileServerPort = pPort;
    m_FileProcess = new QProcess;
    connect(m_FileProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_ReadFileOperateOutput()));
    return true;
}

bool SqlQuery::Disconnect()
{
    return CloseDatabase();
}

bool SqlQuery::OpenDatabase(QString pDatabase, QString pServer, QString pUID, QString pPWD)
{
    if(pDatabase.isEmpty() || pServer.isEmpty())
    {
        m_LastErrorString = ("DataBaseName or ServerName is empty!");
        return false;
    }
    CloseDatabase();
    //>@pDatabase�������ֲ�ͬ�����ݿ����ӣ�ͬһConnectName������ֻ������һ�Ρ�
    m_SqlDatabase = QSqlDatabase::addDatabase("QODBC", QString("Query:")+pDatabase);
    m_SqlDatabase.setConnectOptions("SQL_ATTR_PACKET_SIZE=1024;SQL_ATTR_CONNECTION_TIMEOUT=1,SQL_ATTR_LOGIN_TIMEOUT=1");
    m_SqlDatabase.setHostName(pServer);
    m_SqlDatabase.setDatabaseName(pDatabase);
    //��ʹdsn���Ѿ�������UID��PASSWD����Ȼ��Ҫִ��setUserName��setPassword�Ĳ���
    //��Ҫע��������û���Ҫ�д������Ȩ�ޣ���Ȼ��������ı�student�᲻�ɹ���
    m_SqlDatabase.setUserName(pUID);
    m_SqlDatabase.setPassword(pPWD);
    if (!m_SqlDatabase.open())
    {
        m_LastErrorString = m_SqlDatabase.lastError().databaseText();
        return false;
    }
    return true;
}

bool SqlQuery::CloseDatabase()
{
    if(m_SqlDatabase.isOpen())
    {
        m_SqlDatabase.close();
    }
    return true;
}

//>@pRowָ����0��ʾ��β�����
bool SqlQuery::AddRow(QString pTableName, int pRow, QList<QVariant> pPara)
{
    if(pTableName.isEmpty())
    {
        m_LastErrorString = QString("Table name is empty!");
        return false;
    }
    if(m_SqlDatabase.isOpen() == false)
    {
        m_LastErrorString = QString("Database has not open!");
        return false;
    }
    QSqlTableModel *model = new QSqlTableModel(this, m_SqlDatabase);
    model->setTable(pTableName);
    // ��SELECT�Y�ϵ�Model
    model->select();
    // ��Model������һ��
    model->insertRows(pRow, 1);
    for(int i=0; i<pPara.count(); i++)
    {
        model->setData(model->index(pRow, i), pPara.at(i).toString());
    }
    bool ret = model->submitAll();
    if(ret == false)
        m_LastErrorString = model->lastError().text();
    delete model;
    return ret;
}

bool SqlQuery::DelRow(QString pTableName, int pRow)
{
    if(pTableName.isEmpty())
    {
        m_LastErrorString = QString("Table name is empty!");
        return false;
    }
    if(m_SqlDatabase.isOpen() == false)
    {
        m_LastErrorString = QString("Database has not open!");
        return false;
    }
    QSqlTableModel *model = new QSqlTableModel(this, m_SqlDatabase);
    model->setTable(pTableName);
    // ��SELECT�Y�ϵ�Model
    model->select();
    // ��Model��ɾ��һ��
    model->removeRows(pRow, 1);
    bool ret = model->submitAll();
    if(ret == false)
        m_LastErrorString = model->lastError().text();
    delete model;
    return true;
}

bool SqlQuery::UpdateRow(QString pTableName, int pRow, QList<QVariant> pPara)
{
    if(pTableName.isEmpty())
    {
        m_LastErrorString = QString("Table name is empty!");
        return false;
    }
    if(m_SqlDatabase.isOpen() == false)
    {
        m_LastErrorString = QString("Database has not open!");
        return false;
    }
    QSqlTableModel *model = new QSqlTableModel(this, m_SqlDatabase);
    model->setTable(pTableName);
    // ��SELECT�Y�ϵ�Model
    model->select();
    // ��Model�и���һ��
    for(int i=0; i<pPara.count(); i++)
    {
        model->setData(model->index(pRow, i), pPara.at(i).toString());
    }
    bool ret = model->submitAll();
    if(ret == false)
        m_LastErrorString = model->lastError().text();
    delete model;
    return ret;
}

bool SqlQuery::ExecSqlQuery(QString pQuery)
{
    if(pQuery.isEmpty())
    {
        m_LastErrorString = QString("SQL query is empty!");
        return false;
    }
    IDE_TRACE_STR(pQuery);
    if(m_SqlDatabase.isOpen() == false)
    {
        m_LastErrorString = QString("Database has not open!");
        return false;
    }
    if(m_SqlQueryModel != 0)
    {
        delete m_SqlQueryModel;
        m_SqlQueryModel = 0;
    }
    m_SqlQueryModel = new QSqlQueryModel;
    m_SqlQueryModel->setQuery(pQuery, m_SqlDatabase);
    return true;
}

QList<QVariant> SqlQuery::QueryDatas(QString pTableName, QString pColumns, QString pCondition)
{
    QList<QVariant> tmpRetList;
    if(ExecSqlQuery(QString("select %1 from %2 where %3")
                    .arg(pColumns)
                    .arg(pTableName)
                    .arg(pCondition)) == true)
    {
        int count = m_SqlQueryModel->columnCount();
        if(count ==1)
        {
            for(int i=0; i<m_SqlQueryModel->rowCount(); i++)
            {
                QString tmpString = m_SqlQueryModel->data(m_SqlQueryModel->index(i, 0)).toString();
                tmpRetList.append(QVariant(tmpString));
            }
        }
        else if(count > 1)
        {
            for(int i=0; i<m_SqlQueryModel->rowCount(); i++)
            {
                QStringList tmpStringList;
                for(int j=0; j<count; j++)
                    tmpStringList.append(m_SqlQueryModel->data(m_SqlQueryModel->index(i, j)).toString());
                tmpRetList.append(QVariant(tmpStringList));
            }
        }
    }
    else
    {
        QMessageBox::warning(0, tr("Error"),
                             m_LastErrorString,
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
    return tmpRetList;
}

bool SqlQuery::DownLoadFile(QString pServerFilePath, QString pLocalFilePath, quint32 pTimeout)
{
    return false;
}

void SqlQuery::slot_ReadFileOperateOutput()
{

}

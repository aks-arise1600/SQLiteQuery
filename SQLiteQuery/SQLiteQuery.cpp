#include "SQLiteQuery.h"


SQLiteQuery::SQLiteQuery(QString dbName)
{
    strDBname = dbName;
}


QString SQLiteQuery::m_lastError()
{
    return strSqliteError;
}

bool SQLiteQuery::m_Open(QSqlDatabase &dba)
{
    QString strCounter = QString::number(counter++);
    dba = QSqlDatabase::addDatabase("QSQLITE" , "CONN"+QString::number(QDateTime::currentMSecsSinceEpoch())+strCounter);
    dba.setDatabaseName(QDir::currentPath()+QDir::separator()+strDBname+".db");
    if(!dba.open())
    {
        strSqliteError = "Failed to open "+dba.databaseName();
        PRINT() << DT_FUNC(__FUNCTION__) << "Can't Connect to DB !";
        return false;
    }
    else
        PRINT() << DT_FUNC(__FUNCTION__) << "Connected Successfully to DB !";

    return true;
}

bool SQLiteQuery::m_CreateTable(QSqlDatabase &dba, QString TableName, QMap<QString, QString> attributes)
{
    strSqliteError.clear();

    if(m_Open(dba))
    {
        QSqlQuery qry_Creation(QSqlDatabase::database(dba.connectionName()));
        QString str_Columns;
        foreach (QString key,attributes.keys())
        {
            str_Columns += key + " ";
            str_Columns += attributes.value(key) + ", ";
        }
        str_Columns.remove(str_Columns.size()-2,1);

        QString str_CreationQry = "Create Table if not exists " + TableName + " ( " + str_Columns + " );";
        qry_Creation.prepare(str_CreationQry);

        if (! qry_Creation.exec())
        {
            strSqliteError = qry_Creation.lastError().text() +"; Query = "+qry_Creation.lastQuery();
            dba.close();
            return false;
        }
        qry_Creation.finish();

        PRINT() << DT_FUNC(__FUNCTION__) << "Table Creation of " << TableName << " executed successfully";
        dba.close();
        return true;
    }
    return false;
}

bool SQLiteQuery::m_DeleteTable(QSqlDatabase &dba,QString str_TableName)
{
    strSqliteError.clear();
    if(m_Open(dba))
    {
        QSqlQuery qry_Deletion(QSqlDatabase::database(dba.connectionName()));
        QString str_DeleteQry = "Drop table " + str_TableName;

        if (! qry_Deletion.exec(str_DeleteQry))
        {
            strSqliteError = qry_Deletion.lastError().text() +"; Query = "+qry_Deletion.lastQuery();
            dba.close();
            return false;
        }
        else
        {
            PRINT() << DT_FUNC(__FUNCTION__)  <<str_TableName << " table deleted successfully ";
            dba.close();
            return true;
        }
    }
    return false;
}

bool SQLiteQuery::m_Insert(QSqlDatabase &dba, QString TableName, QMap<QString, QString> insertdata)
{
    strSqliteError.clear();
    if(m_Open(dba))
    {
        QSqlQuery qry_Insertion(QSqlDatabase::database(dba.connectionName()));
        QString str_ColumnsName;
        QString str_ColumnsData;

        foreach (QString key, insertdata.keys())
        {
            str_ColumnsName = str_ColumnsName + " "  + key + ", ";
            str_ColumnsData = str_ColumnsData + "\"" + insertdata.value(key) + "\"" + ",";
        }

        str_ColumnsName.remove(str_ColumnsName.lastIndexOf(","),1);
        str_ColumnsData.remove(str_ColumnsData.lastIndexOf(","),1);


        QString str_InsertQry = "Insert into " + TableName + " (" + str_ColumnsName + " )" + " VALUES (" + str_ColumnsData + " );";

        qry_Insertion.prepare(str_InsertQry);

        if (! qry_Insertion.exec())
        {
            strSqliteError = qry_Insertion.lastError().text() +"; Query = "+qry_Insertion.lastQuery();
            dba.close();
            return false;
        }

        PRINT() << DT_FUNC(__FUNCTION__)  << "Insertion qry executed successfully ";
        dba.close();
        return true;
    }
    return false;
}

bool SQLiteQuery::m_UpdationQuery(QSqlDatabase &dba,QString str_TableName, QMap <QString, QString> map_UpdateData
                               , QMap <QString, QString> map_ColumnData
                               , short int i_WhereClause)
{

    strSqliteError.clear();
    if(m_Open(dba))
    {
        QString str_UpdateString;

        QSqlQuery qry_Updation(QSqlDatabase::database(dba.connectionName()));

        foreach (QString key, map_UpdateData.keys())
            str_UpdateString += key + " = " + "\'" + map_UpdateData.value(key) + "\'" + ", ";

        str_UpdateString.remove(str_UpdateString.lastIndexOf(","), 1);

        QString str_UpdateQry, str_WhereQry;

        str_WhereQry.clear();

        foreach (QString key, map_ColumnData.keys())
            str_WhereQry += key + " = " + "\'" + map_ColumnData.value(key) + "\'" + QString(" and ") ;

        str_WhereQry.remove(str_WhereQry.lastIndexOf(" and "), QString(" and ").size());

        if (i_WhereClause)
            str_UpdateQry = "Update " + str_TableName + " set " + str_UpdateString
                    + " where " + str_WhereQry;
        else
            str_UpdateQry = "Update " + str_TableName + " set " + str_UpdateString;

        if (! qry_Updation.exec(str_UpdateQry))
        {
            strSqliteError = qry_Updation.lastError().text() +"; Query = "+qry_Updation.lastQuery();
            dba.close();
            return false;
        }
        else
        {
            PRINT() << DT_FUNC(__FUNCTION__)  << str_TableName << " table updated successfully ";
            dba.close();
            return true;
        }
    }


    return false;
}

bool SQLiteQuery::m_SelectQueryMap(QSqlDatabase &dba,QString str_TableName, QString str_ColumnName
                                            , QHash<int, QMap<QString,QString>> *hash_TableData
                                            , QStringList str_FieldName, QStringList str_FieldData
                                            , QString OrderBy, QString AdditionalOptions, short i_WhereClause)
{
    strSqliteError.clear();
    if(m_Open(dba))
    {
        short int i_listIndex      = 0;
        QString WhereClause;
        for(int i = 0; i < str_FieldName.count(); i++)
        {
            WhereClause = WhereClause + " " + str_FieldName[i]
                    + "="
                    + "\'"  +  str_FieldData[i] + "\'" + " and ";
        }

        if(WhereClause.count() > 5)
            WhereClause.chop(5);

        if(str_FieldName.count() == 0)
            WhereClause = "1";

        QString str_Order = "";
        if(!OrderBy.contains("ASC"))
        {
            str_Order = " DESC ";
        }

        QMap<QString,QString> Data;
        QSqlQuery qry_Selection(QSqlDatabase::database(dba.connectionName()));
        QString str_SelectQry;

        if (i_WhereClause)
            str_SelectQry = "Select " + str_ColumnName + " from " + str_TableName
                    + " where " + WhereClause + " order by " + OrderBy + str_Order + AdditionalOptions;
        else
            str_SelectQry = "Select " + str_ColumnName + " from " + str_TableName
                    + " order by " + OrderBy + str_Order + AdditionalOptions;

#ifdef DEBUG
        //    qDebug() << "Selection QRY " << str_SelectQry;
#endif

        if (! qry_Selection.exec(str_SelectQry))
        {
            strSqliteError = qry_Selection.lastError().text() +"; Query = "+qry_Selection.lastQuery();
            dba.close();
            return false;
        }

        int RowIndex = 0;

        while (qry_Selection.next())
        {
            Data.clear();
            while (i_listIndex < (qry_Selection.record().count()))
            {
                Data.insert(qry_Selection.record().fieldName(i_listIndex),qry_Selection.record().value(i_listIndex).toString());
                i_listIndex++;
            }
            hash_TableData->insert(RowIndex++,Data);
            i_listIndex      = 0;
        }
        qry_Selection.finish();

        PRINT() << DT_FUNC(__FUNCTION__)  << str_TableName << " table Selected successfully ";
        dba.close();
        return true;
    }
    return false;
}

bool SQLiteQuery::m_CommonForAllQuery(QSqlDatabase &dba, QString Query, QList<QStringList> &Result)
{
    Result.clear();
    strSqliteError.clear();
    if(m_Open(dba))
    {
        QSqlQuery CommonQuery(QSqlDatabase::database(dba.connectionName()));
        if (!CommonQuery.exec(Query))
        {
            strSqliteError = CommonQuery.lastError().text() +"; Query = "+CommonQuery.lastQuery();
            dba.close();
            return false;
        }
        else
        {
            QSqlRecord rec = CommonQuery.record();
#ifdef DEBUG
            //        qDebug() << "Number of columns: " << rec.count();
#endif
            while (CommonQuery.next())
            {
                QStringList list;
                for (int j=0;j<rec.count();j++)
                    list<<CommonQuery.value(j).toString();

                Result.append(list);
            }
            dba.close();
            return true;
        }
    }
    return false;
}

bool SQLiteQuery::m_DeleteQuery(QSqlDatabase &dba,QString str_TableName, QMap <QString, QString> map_DeleteData)
{
    strSqliteError.clear();
    if(m_Open(dba))
    {
        QSqlQuery qry_Deletion(QSqlDatabase::database(dba.connectionName()));

        QString str_DeleteQry = "Delete from " + str_TableName + " where ";

        foreach (QString key, map_DeleteData.keys())
            str_DeleteQry += key + " = " + "\'" + map_DeleteData.value(key) + "\'" + QString (" and ");

        str_DeleteQry.remove(str_DeleteQry.lastIndexOf(" and "), QString(" and ").size());

        qry_Deletion.prepare(str_DeleteQry);

        if (! qry_Deletion.exec())
        {
            strSqliteError = qry_Deletion.lastError().text() +"; Query = "+qry_Deletion.lastQuery();
            dba.close();
            return false;
        }

        qry_Deletion.finish();
        PRINT() << DT_FUNC(__FUNCTION__)  << str_TableName << " table deleted successfully ";
        dba.close();
        return true;
    }
    return false;
}

bool SQLiteQuery::m_IsTableExists(QSqlDatabase &dba,QString str_TableName)
{
    strSqliteError.clear();
    if(m_Open(dba))
    {
        QSqlQuery qry_Creation(QSqlDatabase::database(dba.connectionName()));

        QStringList strList_Data;

//        QString str_CreationQry = "show tables";
        QString str_CreationQry = ".tables";

        qry_Creation.exec(str_CreationQry);
        int i_listIndex = 0;

        while (qry_Creation.next())
        {
            while (i_listIndex < (qry_Creation.record().count()))
                strList_Data.append(qry_Creation.record().value(i_listIndex ++).toString());

            i_listIndex      = 0;
        }
        qry_Creation.finish();

        return (strList_Data.contains(str_TableName));
    }
    return false;
}

int SQLiteQuery::m_GetTotalColumns(QSqlDatabase &dba,QString str_TableName)
{
    strSqliteError.clear();
    if(m_Open(dba))
    {
        QString Query =  "SELECT COUNT(*) FROM information_schema.columns WHERE table_name = '" + str_TableName + "'";
        QString Num_of_Column;
        QSqlQuery CommonQuery(QSqlDatabase::database(dba.connectionName()));
        if (!CommonQuery.exec(Query))
        {
            return (0);
        }
        else
        {
            while (CommonQuery.next()) {
                Num_of_Column = CommonQuery.value(0).toString();
            }

            return Num_of_Column.toInt();
        }
    }
    return 0;
}

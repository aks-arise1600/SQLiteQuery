#ifndef SQLITEQUERY_H
#define SQLITEQUERY_H

//#include "sqlitequery_global.h"
#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QDir>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


#define DT_FUNC(func)           QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+" "+func+" ::"
#define PRINT()                 qDebug().noquote()

class /*SQLITEQUERYSHARED_EXPORT*/ SQLiteQuery: public QObject
{
    Q_OBJECT
public:
    SQLiteQuery(QString dbName = "sample");
    QString m_lastError();
    bool m_Open(QSqlDatabase &dba);
    bool m_CreateTable(QSqlDatabase &dba ,QString TableName,QMap<QString,QString> attributes);
    bool m_DeleteTable(QSqlDatabase &dba, QString str_TableName);
    bool m_Insert(QSqlDatabase &dba,QString TableName,QMap<QString,QString> insertdata);
    bool m_UpdationQuery(QSqlDatabase &dba, QString str_TableName, QMap<QString, QString> map_UpdateData,
                         QMap<QString, QString> map_ColumnData, short int i_WhereClause = 0);
    bool m_SelectQueryMap(QSqlDatabase &dba, QString str_TableName, QString str_ColumnName,
                          QHash<int, QMap<QString, QString> > *hash_TableData, QStringList str_FieldName,
                          QStringList str_FieldData, QString OrderBy="1", QString AdditionalOptions="", short i_WhereClause = 0);
    bool m_CommonForAllQuery(QSqlDatabase &dba, QString Query, QList<QStringList> &Result);
    bool m_DeleteQuery(QSqlDatabase &dba, QString str_TableName, QMap<QString, QString> map_DeleteData);
    bool m_IsTableExists(QSqlDatabase &dba, QString str_TableName);
    int m_GetTotalColumns(QSqlDatabase &dba, QString str_TableName);
private:
    long int counter = 0;
    QString strSqliteError;
    QString strDBname;
};

#endif // SQLITEQUERY_H

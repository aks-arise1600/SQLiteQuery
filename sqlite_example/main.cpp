#include <QCoreApplication>
#include <SQLiteQuery.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString strTableName = "LoginInfo";
    // *** CREATE LOGIN Info TABLE
    SQLiteQuery *mydb = new SQLiteQuery("databaseSample");
    QSqlDatabase db;
    QMap<QString,QString> map;
    map.insert("UserName","varchar(35) NOT NULL");
    map.insert("UserPasscode","varchar(20) NOT NULL");
    map.insert("UserID","varchar(12) NOT NULL");
    if(!mydb->m_CreateTable(db,strTableName,map))
        qDebug()<<DT_FUNC(__FUNCTION__)<<mydb->m_lastError();


    // *** INSERT data in LOGIN Info TABLE
    map.clear();
    map.insert("UserName","Admin");
    map.insert("UserPasscode","admin");
    map.insert("UserID","001");
    if(!mydb->m_Insert(db,strTableName,map))
        qDebug()<<DT_FUNC(__FUNCTION__)<<mydb->m_lastError();

    map.clear();
    map.insert("UserName","userOne");
    map.insert("UserPasscode","password");
    map.insert("UserID","002");
    if(!mydb->m_Insert(db,strTableName,map))
        qDebug()<<DT_FUNC(__FUNCTION__)<<mydb->m_lastError();


    // *** SELECT data from LOGIN Info TABLE
    QHash<int,QMap<QString,QString>> obj_hash;
    QStringList fName,fData;
    fName << "UserID";
    fData << "001";
    if(!mydb->m_SelectQueryMap(db,strTableName,"*",&obj_hash,fName,fData,"1",0,1)) // SELECT * FROM LoginInfo WHERE UserID='001'
        qDebug()<<DT_FUNC(__FUNCTION__)<<mydb->m_lastError();
    else
        qDebug()<<DT_FUNC(__FUNCTION__)<<obj_hash;

    if(!mydb->m_SelectQueryMap(db,strTableName,"*",&obj_hash,fName,fData,"1",0)) // SELECT * FROM LoginInfo
        qDebug()<<DT_FUNC(__FUNCTION__)<<mydb->m_lastError();
    else
        qDebug()<<DT_FUNC(__FUNCTION__)<<obj_hash;


    return a.exec();
}

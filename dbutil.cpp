#include "dbutil.h"
#include <QtSql>

const QString DBUtil::DBUtil::DB_NAME = "./websearch.db";
const QString DBUtil::SEARCH_ENGINE_NAME = "search_engine";
const QString DBUtil::KEY_WORD_NAME = "key_word";
const QString DBUtil::CLICK_TABLE_NAME = "click";
const QString DBUtil::CREATE_SEARCH_ENGINE_SQL= "create table search_engine(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(512), url varchar(1024))";
const QString DBUtil::CREATE_KEYWORD_SQL = "create table key_word(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(512))";
const QString DBUtil::CREATE_CLICK_SQL = "create table click(search_engine_id integer, key_word_id integer, click_times integer)";
const QString DBUtil::DROP_SEARCH_ENGINE_SQL = "drop table search_engine";
const QString DBUtil::DROP_KEYWORD_SQL = "drop table key_word";
const QString DBUtil::DROP_CLICK_SQL = "drop table click";

DBUtil::DBUtil()
{
}

QSqlDatabase DBUtil::getDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_NAME);
    db.open();
    return db;
}

void DBUtil::closeDB(QSqlDatabase db)
{
    if (db.isValid() && db.isOpen()) db.close();
}

QSqlQuery DBUtil::getQuery(const QSqlDatabase &db)
{
    return QSqlQuery(db);
}

void DBUtil::initTables()
{
    QSqlDatabase db = getDB();
    if (db.isOpen()) {
        QSqlQuery query = getQuery(db);
        bool result = query.exec(CREATE_SEARCH_ENGINE_SQL);
        printInfo(query, result);
        result = query.exec(CREATE_KEYWORD_SQL);
        printInfo(query, result);
        result = query.exec(CREATE_CLICK_SQL);
        printInfo(query, result);
        closeDB(db);
    }else {
        qDebug() << "error db is not open";
    }
}
void DBUtil::clearTables()
{
    QSqlDatabase db = getDB();
    if (db.isOpen()) {
        QSqlQuery query = getQuery(db);
        bool result = query.exec(DROP_SEARCH_ENGINE_SQL);
        printInfo(query, result);
        result = query.exec(DROP_KEYWORD_SQL);
        printInfo(query, result);
        result = query.exec(DROP_CLICK_SQL);
        printInfo(query, result);
        closeDB(db);
    }else {
        qDebug() << "error db is not open";
    }
}
void DBUtil::printInfo(const QSqlQuery &query, bool result)
{
    if (!result) qDebug() << query.lastError();
}

void DBUtil::initValues()
{
    QSqlDatabase db = getDB();
    if (db.isOpen()) {
        QSqlQuery query = getQuery(db);
        bool result = query.prepare("insert into search_engine(name, url) values(?,?)");
        query.addBindValue("百度");
        query.addBindValue("http://www.baidu.com");
        result = query.exec();
        db.commit();
        printInfo(query, result);
        result = query.prepare("insert into search_engine(name, url) values(?,?)");
        query.addBindValue("手机百度");
        query.addBindValue("http://m.baidu.com");
        result = query.exec();
        db.commit();
        printInfo(query, result);
        //keyword
        result = query.prepare("insert into key_word(name) values(?)");
        query.addBindValue("musper");
        query.exec();
        printInfo(query, result);
        db.commit();
        closeDB(db);
    }else {
        qDebug() << "error db is not open";
    }
}

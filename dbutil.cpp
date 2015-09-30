#include "dbutil.h"
#include "engineinfo.h"
#include <QtSql>
#include <QtCore>

const QString DBUtil::DBUtil::DB_NAME = "websearch.db";
const QString DBUtil::SEARCH_ENGINE_NAME = "search_engine";
const QString DBUtil::KEY_WORD_NAME = "key_word";
const QString DBUtil::CLICK_TABLE_NAME = "click";
const QString DBUtil::CREATE_SEARCH_ENGINE_SQL= "create table search_engine(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(512), url varchar(1024))";
const QString DBUtil::CREATE_KEYWORD_SQL = "create table key_word(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(512))";
const QString DBUtil::CREATE_CLICK_SQL = "create table click(id INTEGER PRIMARY KEY AUTOINCREMENT, engine_url varchar(1024), key_word varchar(512), click_times long)";
const QString DBUtil::DROP_SEARCH_ENGINE_SQL = "drop table search_engine";
const QString DBUtil::DROP_KEYWORD_SQL = "drop table key_word";
const QString DBUtil::DROP_CLICK_SQL = "drop table click";
const QString DBUtil::CONNECTION_NAME = "my_sqlite_connection";
const QString DBUtil::INSERT_KEY_WORDS_SQL = "insert into key_word(name) values(?)";
const QString DBUtil::INSERT_ENGINE_SQL = "insert into search_engine(name, url) values(?,?)";
const QString DBUtil::DELETE_ENGINE_SQL = "delete from search_engine where id=?";
const QString DBUtil::DELETE_KEY_WORDS_SQL = "delete from key_word where id=?";
const QString DBUtil::QUERY_KEY_WORDS_SQL = "select name from key_word";
const QString DBUtil::QUERY_SEARCH_ENGINE_SQL = "select name, url from search_engine";

DBUtil::DBUtil()
{
}

QSqlDatabase DBUtil::getDB()
{
    if (!QSqlDatabase::contains(CONNECTION_NAME)) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
        db.setDatabaseName(DB_NAME);
        db.open();
        return db;
    }
    else {
        QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
        if (db.isValid() && db.isOpen()){
            return db;
        }else
        {
            db.open();
            return db;
        }
    }
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
        QVariantList nameList;
        nameList << QObject::tr("百度") << QObject::tr("手机百度");
        QVariantList urlList;
        urlList << QObject::tr("http://www.baidu.com") << QObject::tr("http://m.baidu.com");
        query.addBindValue(nameList);
        query.addBindValue(urlList);
        result = query.execBatch();
        printInfo(query, result);
        //keyword
        query = getQuery(db);
        result = query.prepare("insert into key_word(name) values(?)");
        query.bindValue(0, "musper");
        query.exec();
        printInfo(query, result);
        db.commit();
        closeDB(db);
    }else {
        qDebug() << "error db is not open";
    }
}
void DBUtil::test()
{
    QSqlDatabase db = getDB();
    QSqlQuery query = getQuery(db);
    query.exec("select * from search_engine");
    while(query.next()) {
        qDebug() << query.value(0).toInt()
                << "," << query.value(1).toString()
                << "," << query.value(2).toString();
    }
}
QList<QString> DBUtil::getKeyWords()
{
    QList<QString> keyWords;
    QSqlDatabase db = getDB();
    QSqlQuery query = getQuery(db);
    query.exec(QUERY_KEY_WORDS_SQL);
    while(query.next()) {
        keyWords << query.value(0).toString();
    }
    return keyWords;
}

QList<EngineInfo> DBUtil::getEngineInfos()
{
    QList<EngineInfo> engineInfos;
    QSqlDatabase db = getDB();
    QSqlQuery query = getQuery(db);
    query.exec(QUERY_SEARCH_ENGINE_SQL);
    while(query.next()) {
        engineInfos << EngineInfo(query.value(0).toString(), query.value(1).toString());
    }
    return engineInfos;
}

bool DBUtil::insertKeyWords(const QVariantList &words)
{
    qDebug() << "start insert words size:" << words.size();
    QSqlDatabase db = getDB();
    QSqlQuery query = getQuery(db);
    bool result = query.prepare(INSERT_KEY_WORDS_SQL);
    printInfo(query, result);
    if (!result) return result;
    query.addBindValue(words);
    result = query.execBatch();
    printInfo(query, result);
    return result;

}
bool DBUtil::insertEngines(const QVariantList& engineNames, const QVariantList& engineUrls)
{
    qDebug() << "start insert engines size:" + engineNames.size();
    QSqlDatabase db = getDB();
    QSqlQuery query = getQuery(db);
    bool result = query.prepare(INSERT_ENGINE_SQL);
    printInfo(query, result);
    if (!result) return result;
    query.addBindValue(engineNames);
    query.addBindValue(engineUrls);
    result = query.execBatch();
    printInfo(query, result);
    return result;
}

bool DBUtil::deleteByIds(const QString &sql, const QVariantList &ids)
{
    qDebug() << "delete count: " << ids.count();
    QSqlDatabase db = getDB();
    QSqlQuery query = getQuery(db);
    bool result = query.prepare(sql);
    printInfo(query, result);
    if (!result) return result;
    query.addBindValue(ids);
    result = query.execBatch();
    printInfo(query, result);
    return result;
}

bool DBUtil::deleteEngines(const QVariantList &ids)
{
    return deleteByIds(DELETE_ENGINE_SQL, ids);
}
bool DBUtil::deleteKeyWords(const QVariantList &ids)
{
    return deleteByIds(DELETE_KEY_WORDS_SQL, ids);
}

#ifndef DBUTIL_H
#define DBUTIL_H

class QSqlDatabase;
class QSqlQuery;
class QString;

class DBUtil
{
public:
    static QSqlDatabase getDB();
    static void closeDB(QSqlDatabase db);
    static QSqlQuery getQuery(const QSqlDatabase& db);
    static void initTables();
    static void clearTables();
    static void initValues();
    static void printInfo(const QSqlQuery& query, bool result);
    static void test();
private:
    DBUtil();
public:
    static const QString DB_NAME;
    static const QString SEARCH_ENGINE_NAME;
    static const QString KEY_WORD_NAME;
    static const QString CLICK_TABLE_NAME;
    static const QString CREATE_SEARCH_ENGINE_SQL;
    static const QString CREATE_KEYWORD_SQL;
    static const QString CREATE_CLICK_SQL;
    static const QString DROP_SEARCH_ENGINE_SQL;
    static const QString DROP_KEYWORD_SQL;
    static const QString DROP_CLICK_SQL;
    static const QString CONNECTION_NAME;
};

#endif // DBUTIL_H

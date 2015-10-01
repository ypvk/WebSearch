#ifndef DBUTIL_H
#define DBUTIL_H

#include<QVariantList>
#include <QList>
#include <QString>
#include <QPair>

class QSqlDatabase;
class QSqlQuery;
class QString;
class EngineInfo;


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
    static bool insertKeyWords(const QVariantList& words);
    static bool deleteKeyWords(const QVariantList& ids);
    static bool insertEngines(const QVariantList& engineNames, const QVariantList& engineUrls);
    static bool deleteEngines(const QVariantList& ids);
    static bool insertProxys(const QVariantList& ips, const QVariantList& ports);
    static bool deleteProxys(const QVariantList& ids);
    static QList<QString> getKeyWords();
    static QList<EngineInfo> getEngineInfos();
    static QList<QPair<QString, int> > getProxys();
    static bool incWorkClick(const QString& keyWord, const QString& url);
    static void test();
private:
    DBUtil();
    static bool deleteByIds(const QString& sql, const QVariantList& ids);
public:
    static const QString DB_NAME;
    static const QString SEARCH_ENGINE_NAME;
    static const QString KEY_WORD_NAME;
    static const QString PROXY_NAME;
    static const QString CLICK_TABLE_NAME;
    static const QString CREATE_SEARCH_ENGINE_SQL;
    static const QString CREATE_KEYWORD_SQL;
    static const QString CREATE_CLICK_SQL;
    static const QString CREATE_PROXY_SQL;
    static const QString DROP_SEARCH_ENGINE_SQL;
    static const QString DROP_KEYWORD_SQL;
    static const QString DROP_CLICK_SQL;
    static const QString DROP_PROXY_SQL;
    static const QString CONNECTION_NAME;
    static const QString INSERT_KEY_WORDS_SQL;
    static const QString INSERT_ENGINE_SQL;
    static const QString INSERT_PROXY_SQL;
    static const QString DELETE_KEY_WORDS_SQL;
    static const QString DELETE_ENGINE_SQL;
    static const QString DELETE_PROXY_SQL;
    static const QString QUERY_KEY_WORDS_SQL;
    static const QString QUERY_SEARCH_ENGINE_SQL;
    static const QString QUERY_PROXY_SQL;
    static const QString EXISTS_CLICK_SQL;
    static const QString INSERT_CLICK_SQL;
    static const QString UPDATE_CLICK_SQL;
};

#endif // DBUTIL_H

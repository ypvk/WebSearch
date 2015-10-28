#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
#include <QList>

class QNetworkAccessManager;
class NetWorkCookieJar;
class QTimer;
class ClickInfo;
class QNetworkRequest;
class UpdateInfo;

class QueryThread : public QThread
{
    Q_OBJECT
public:
    explicit QueryThread(QObject *parent = 0);
    QueryThread(QObject* parent, const QList<ClickInfo>& clickInfos, int id);
    ~QueryThread();
signals:
    void updateClickInfo(const UpdateInfo& updateInfo);
private:
    void clearCookies();
    void setRequestHeader(QNetworkRequest& request, QString host, QString refer);
    QString buildUrl(QString mainWord, QString assistWord);
    int queryWord(QString mainWord, QString assistWord, QNetworkRequest& request, QString currentIp);
    void updateCookie();
public:
    void run();
private:
    QNetworkAccessManager* networkManager;
    NetWorkCookieJar* cookieJar;
    QTimer* timer;
    QList<ClickInfo> clickInfos;
    int id;
    QString lastUrl;
};

#endif // QUERYTHREAD_H
